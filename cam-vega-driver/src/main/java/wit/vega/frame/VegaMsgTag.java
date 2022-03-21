package wit.vega.frame;

import java.io.IOException;
import java.io.InputStream;
import wit.vega.net.TcpClient;
import wit.vega.VegaException;

/**
 * Сообщение от камеры.
 *
 * @author vneverov
 */
public class VegaMsgTag extends VegaCmdTag {

    //private static final Logger LOG = Logger.getLogger(VegaMsgTag.class.getName());
    
    private VegaHeaderFrame header;

    public VegaMsgTag(VegaHeaderFrame header) throws IOException {
        super(header);
        this.header = header;
    }

    @Override
    public void deserialize(InputStream ins) throws IOException, VegaException {
        int restLen = header.getDataDimensionInt();
        header.setDataDimensionInt(0);

        FourByteNumber tag = new FourByteNumber();
        FourByteNumber tagLen = new FourByteNumber();
        while (true) {
            // Тип тега
            if (restLen < 4 && ins.available() < 4) {
                break;
            }

            TcpClient.waitStream(ins, FourByteNumber.SIZE);
            tag.deserialize(ins);
            restLen -= FourByteNumber.SIZE;
            int tagCode = tag.toInt();
            //LOG.log(Level.FINE, "[deserialize] tagCode: {0}", tagCode);
            if (tagCode > 0) {
                int reservedDataLen = getDataLen(tagCode);
                if (reservedDataLen >= 0 && ins.available() > 0) {
                    // длина данных тега
                    tagLen.deserialize(ins);
                    restLen -= FourByteNumber.SIZE;
                    int realDataLen = tagLen.toInt();
                    //LOG.log(Level.FINE, "[deserialize] tag: {0}, len: {1}, available {2}", new Object[]{tagCode, realDataLen, ins.available()});

                    if (realDataLen > 0 && ins.available() > 0) {
                        if (ins.available() < realDataLen) {
                            // Реальная длина меньше указанной в теге
                            if (tagCode == VegaConsts.TCP_MSG_TAG_IMAGE_BW ||
                                    tagCode == VegaConsts.TCP_MSG_TAG_IMAGE_COL) {
                                // для фотки делаем исключение, читаем сколько читается
                                restLen -= realDataLen;
                                realDataLen = ins.available();
                                restLen += realDataLen;
                            } else {
                                return;
                            }
                        }

                        byte[] bdata = new byte[realDataLen];
                        int realDataLen2 = ins.read(bdata);
                        if (realDataLen != realDataLen2) {
                            throw new RuntimeException("Error read stream");
                        }

                        addFrame(new VegaDataTag(tagCode, bdata));
                        //LOG.log(Level.FINE, "[deserialize] add tag: {0}, len: {1}, available {2}", new Object[]{tagCode, realDataLen2, ins.available()});
                        
                        int skipLen = realDataLen % 4;
                        if (skipLen != 0) {
                            ins.skip(4 - skipLen);
                        }

                        //restLen -= (reservedDataLen == 0 ? realDataLen : reservedDataLen);
                        //if (reservedDataLen > realDataLen) {
                        //    ins.skip(reservedDataLen - realDataLen);
                        //}
//				parseBuf(tagCode, bdata);
                    }
                }
            }
        }
    }

    private int getDataLen(int tagId) {
        switch (tagId) {
            case VegaConsts.TCP_MSG_TAG_DATE:
                return 12;
            case VegaConsts.TCP_MSG_TAG_TIME:
                return 16;
            case VegaConsts.TCP_MSG_TAG_NETBIOSNAME:
                return 32;
            case VegaConsts.TCP_MSG_TAG_PLATE:
                return 24;
            case VegaConsts.TCP_MSG_TAG_PLATE_STRING:
                return 20;
            case VegaConsts.TCP_MSG_TAG_PLATE_COUNTRY:
                return 4;
            case VegaConsts.TCP_MSG_TAG_NUM_READ:
                return 4;
            case VegaConsts.TCP_MSG_TAG_IMAGENAME:
                return 128;
            case VegaConsts.TCP_MSG_TAG_IMAGEPATH:
                return 128;
            case VegaConsts.TCP_MSG_TAG_TRANSIT_ID:
                return 4;
            case VegaConsts.TCP_MSG_TAG_PLATE_MIN_X:
                return 4;
            case VegaConsts.TCP_MSG_TAG_PLATE_MIN_Y:
                return 4;
            case VegaConsts.TCP_MSG_TAG_PLATE_MAX_X:
                return 4;
            case VegaConsts.TCP_MSG_TAG_PLATE_MAX_Y:
                return 4;
            case VegaConsts.TCP_MSG_TAG_SPEED:
                return 4;
            case VegaConsts.TCP_MSG_TAG_CLASS:
                return 4;
            case VegaConsts.TCP_MSG_TAG_CLASS_STRING:
                return 32;
            case VegaConsts.TCP_MSG_TAG_BOARD_SN:
                return 4;
            case VegaConsts.TCP_MSG_TAG_DIRECTION:
                return 12;
            case VegaConsts.TCP_MSG_TAG_NET_TRIG_ID:
                return 32;
            case VegaConsts.TCP_MSG_TAG_IMAGE_BW:
                return 0;
            case VegaConsts.TCP_MSG_TAG_IMAGE_COL:
                return 0;
            case VegaConsts.TCP_MSG_TAG_OCRSCORE:
                return 4;
        }
        return -1;
    }
}
