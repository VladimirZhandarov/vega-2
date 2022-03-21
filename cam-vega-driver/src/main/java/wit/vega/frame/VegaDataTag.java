package wit.vega.frame;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Calendar;
import java.util.Date;
import wit.vega.net.TcpClient;
import wit.vega.VegaException;

/**
 * Элемент блока данных.
 *
 * @author vneverov
 */
public class VegaDataTag {

    public static final int HEADER_SZ = 8;
    private FourByteNumber dataCode;
    private FourByteNumber dataLen;
    private byte[] data;

    public VegaDataTag() {
    }

    public VegaDataTag(int dataCode, byte[] data) {
        this.dataCode = new FourByteNumber(dataCode);
        this.dataLen = new FourByteNumber(data != null ? data.length : 0);
        this.data = data;
    }

    public VegaDataTag(int dataCode, int data) {
        this.dataCode = new FourByteNumber(dataCode);
        this.dataLen = new FourByteNumber(4);
        FourByteNumber fd = new FourByteNumber(data);
        this.data = fd.getBytes();
    }

    public VegaDataTag(int dataCode, String s) {
        byte[] b = s.getBytes();
        int n = b.length % 4;
        if (n > 0) {
            int len1 = b.length;
            int len2 = b.length + 4 - n;
            byte[] bb = new byte[len2];
            System.arraycopy(b, 0, bb, 0, len1);
            b = bb;
        }

        this.dataCode = new FourByteNumber(dataCode);
        this.dataLen = new FourByteNumber(b.length);
        this.data = b;
    }

    public VegaDataTag(int dataCode, Date d) {
        data = new byte[12];
        dataLen = new FourByteNumber(12);
        this.dataCode = new FourByteNumber(dataCode);

        Calendar c = Calendar.getInstance();
        c.setTime(d);

        int i = 0;
        data[i++] = (byte) c.get(Calendar.DAY_OF_MONTH);
        data[i++] = (byte) c.get(Calendar.MONTH + 1);

        i = int2bytes(data, i, c.get(Calendar.YEAR));
        i = int2bytes(data, i, c.get(Calendar.HOUR));
        i = int2bytes(data, i, c.get(Calendar.MINUTE));
        i = int2bytes(data, i, c.get(Calendar.SECOND));
        int2bytes(data, i, c.get(Calendar.MILLISECOND));
    }

    public int getLength() {
        return dataLen.toInt() + 8;
    }

    public byte[] getData() {
        return data;
    }

    public int getDataCodeInt() {
        return dataCode.toInt();
    }

    public void serialize(OutputStream ous) throws IOException {
        dataCode.serialize(ous);
        dataLen.serialize(ous);
        if (data != null) {
            ous.write(data);
        }
    }

    /**
     *
     * @param ins
     * @param restDataSize
     * @return остаток не вошедший в десериализацию.
     * @throws IOException
     * @throws VisiterException
     */
    public Integer deserialize(InputStream ins, Integer restDataSize) throws IOException, VegaException {
        if (restDataSize < HEADER_SZ) {
            return 0;
        }

        TcpClient.waitStream(ins, HEADER_SZ);
        dataCode = new FourByteNumber();
        dataCode.deserialize(ins);

        dataLen = new FourByteNumber();
        dataLen.deserialize(ins);
        int realDataLen = dataLen.toInt();

        restDataSize -= HEADER_SZ;
        if (restDataSize < realDataLen) {
            throw new VegaException("[VegaDataTag#deserialize] Protocol error, rest data less then expected, need: " + realDataLen + ", found: " + restDataSize);
        }

        TcpClient.waitStream(ins, realDataLen);
        data = new byte[realDataLen];
        int n = ins.read(data);
        if (n < realDataLen) {
            throw new VegaException("[VegaDataTag#deserialize] Error download data, need: " + realDataLen + ", found: " + n);
        }

        int skipLen = realDataLen % 4;
        if (skipLen != 0) {
            ins.skip(4 - skipLen);
            realDataLen += skipLen;
        }

        restDataSize -= realDataLen;
        if (restDataSize < 4) {
            restDataSize = 0;
        }
        return restDataSize;
    }

    @Override
    public String toString() {
        byte[] b = getData();
        if (b != null && b.length > 0) {
            return new String(b);
        }
        return "";
    }

    public int toInt() throws VegaException {
        if (dataLen.toInt() == 4) {
            ByteBuffer bb = ByteBuffer.wrap(data);
            bb.order(ByteOrder.LITTLE_ENDIAN);
            return bb.getInt();
        }
        throw new VegaException("Cannot convert data to int");
    }

    public Date toDate() {
        if (dataLen.toInt() == 12) {
            Calendar c = Calendar.getInstance();
            c.set(bytes2int(data, 2), data[1], data[0], bytes2int(data, 4), bytes2int(data, 6), bytes2int(data, 8));
            c.set(Calendar.MILLISECOND, bytes2int(data, 10));
            return c.getTime();
        }
        return null;
    }

    /**
     *
     * @param bb
     * @param i
     * @param d
     * @return смещение в буфере
     */
    private int int2bytes(byte[] bb, int i, int d) {
        bb[i++] = (byte) d;
        bb[i++] = (byte) (d >> 8);
        return i;
    }

    /**
     * Два байта в int.
     *
     * @param bb
     * @param i
     * @return
     */
    private int bytes2int(byte[] bb, int i) {
        return bb[i] + (bb[i + 1] << 8);
    }

    /**
     * Максимальная зарезервированнпя длина данных в теге.
     *
     * @param tagId
     * @return
     */
//	private int getReservedDataLen(int tagId) {
//		switch (tagId) {
//			case VegaConsts.TCP_MSG_TAG_DATE: return 12;
//			case VegaConsts.TCP_MSG_TAG_TIME: return 16;
//			case VegaConsts.TCP_MSG_TAG_NETBIOSNAME: return 32;
//			case VegaConsts.TCP_MSG_TAG_PLATE: return 24;
//			case VegaConsts.TCP_MSG_TAG_PLATE_STRING: return 20;
//			case VegaConsts.TCP_MSG_TAG_PLATE_COUNTRY: return 4;
//			case VegaConsts.TCP_MSG_TAG_NUM_READ: return 4;
//			case VegaConsts.TCP_MSG_TAG_IMAGENAME: return 128;
//			case VegaConsts.TCP_MSG_TAG_IMAGEPATH: return 128;
//			case VegaConsts.TCP_MSG_TAG_TRANSIT_ID: return 4;
//			case VegaConsts.TCP_MSG_TAG_PLATE_MIN_X: return 4;
//			case VegaConsts.TCP_MSG_TAG_PLATE_MIN_Y: return 4;
//			case VegaConsts.TCP_MSG_TAG_PLATE_MAX_X: return 4;
//			case VegaConsts.TCP_MSG_TAG_PLATE_MAX_Y: return 4;
//			case VegaConsts.TCP_MSG_TAG_SPEED: return 4;
//			case VegaConsts.TCP_MSG_TAG_CLASS: return 4;
//			case VegaConsts.TCP_MSG_TAG_CLASS_STRING: return 32;
//			case VegaConsts.TCP_MSG_TAG_BOARD_SN: return 4;
//			case VegaConsts.TCP_MSG_TAG_DIRECTION: return 12;
//			case VegaConsts.TCP_MSG_TAG_NET_TRIG_ID: return 32;
//			case VegaConsts.TCP_MSG_TAG_IMAGE_BW: return 1;
//			case VegaConsts.TCP_MSG_TAG_IMAGE_COL: return 1;
//			case VegaConsts.TCP_MSG_TAG_OCRSCORE: return 4;
//		}
//		return -1;
//	}
}
