package wit.vega.frame;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import wit.vega.net.TcpClient;
import wit.vega.VegaException;

/**
 * Элемент блока данных.
 *
 * @author vneverov
 */
public class VegaDataFrame {

    public static final int HEADER_SZ = 8;
    private FourByteNumber dataCode;
    private FourByteNumber dataLen;
    private byte[] data;

    public VegaDataFrame() {
    }

    public VegaDataFrame(int dataCode, int dataLen, byte[] data) {
        this.dataCode = new FourByteNumber(dataCode);
        this.dataLen = new FourByteNumber(dataLen);
        this.data = data;
    }

    public VegaDataFrame(int dataCode, String s) {
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

    public int getLength() {
        return dataLen.toInt() + 8;
    }

    public byte[] getData() {
        return data;
    }

    public FourByteNumber getDataCode() {
        return dataCode;
    }

    public void serialize(OutputStream ous) throws IOException {
        dataCode.serialize(ous);
        dataLen.serialize(ous);
        if (data != null) {
            ous.write(data);
        }
    }

    public Integer deserialize(InputStream ins, Integer restDataSize) throws IOException, VegaException {
        if (restDataSize < HEADER_SZ) {
            return 0;
        }

        TcpClient.waitStream(ins, HEADER_SZ);
        dataCode = new FourByteNumber();
        dataCode.deserialize(ins);

        dataLen = new FourByteNumber();
        dataLen.deserialize(ins);
        int lenData = dataLen.toInt();

        restDataSize -= HEADER_SZ;
        if (restDataSize < lenData) {
            return 0;
        }

        TcpClient.waitStream(ins, lenData);
        data = new byte[lenData];
        int n = ins.read(data);
        if (n < lenData) {
            throw new RuntimeException("Error download data, need: " + lenData + ", found: " + n);
        }

        int skipLen = lenData % 4;
        if (skipLen != 0) {
            ins.skip(skipLen);
            lenData += 4 - skipLen;
        }

        restDataSize -= lenData;
        if (restDataSize < 4) {
            restDataSize = 0;
        }
        return restDataSize;
    }
}
