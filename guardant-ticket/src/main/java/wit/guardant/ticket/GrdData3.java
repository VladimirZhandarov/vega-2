package wit.guardant.ticket;

import guardant.Api;
import guardant.GrdAM;
import guardant.GrdE;
import guardant.Handle;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.zip.CRC32;

/**
 * Лицензия Ticket.<br />
 *
 * alg0 - ECC160 с ключом активации (1558217874)<br />
 * alg1 - RND64<br />
 * alg2 - AES128, соль берется в pi3<br />
 * pi3 - защищенная на чтение ячека (1558217874) 16 байт, соль для AES<br />
 *
 * @author vneverov
 */
public class GrdData3 implements Serializable {

    private static final long serialVersionUID = -1L;
    /**
     * Параметр донгла по которому ищем.
     */
    public static final byte SEARCH_DONGLE_VER = 1;
    /**
     * Прибавить к кодам.
     */
    public static final int C2 = 0x15DCB;
    /**
     * Длина публ ключа ECC160.
     */
    private static final int SZ_PUB_KEY_ECC = 40;
    
    private static List<String> valueNameList = new ArrayList();
    
    /**
     * vendor - index 0
     * Клиент - компания - index 1
     * Клиент - контактное лицо - index 2
     * Клиент - телефон - index 3
     * Клиент - email - index 4
     * Имя ПО - index 5
     * Версия - index 6
     * Дата выдачи - index 7
     * SN ПО - index 8
     * licence ANPR - index 9
     */
    private List<String> dataList = new ArrayList();

    // 579BCBFC51C14A1D9AF483C59EE496D24BCB3439D0B9EA1962CB9B1C4C4FD6BD77CCDEFA4FD40C6B
    /**
     * Pub key, sign ECC-160, alg-0.
     */
    private byte[] publicEccKey;
    
    // 1558217874
    /**
     * Ключ чтения/активации PI
     */
    private int rdKey;
    
    static {
        valueNameList.add("vendor           : ");
        valueNameList.add("customer         : ");
        valueNameList.add("customer - fio   : ");
        valueNameList.add("customer - phone : ");
        valueNameList.add("customer - email : ");
        valueNameList.add("product          : ");
        valueNameList.add("version          : ");
        valueNameList.add("date             : ");
        valueNameList.add("serial           : ");
        valueNameList.add("licence ANPR     : ");
    }
    
    /**
     * CTOR.
     *
     * для загрузки библиотеки в GF.
     */
    public GrdData3() {
    }

    public GrdData3(String hex) throws GrdException {
        byte[] bb = GrdDongle.hex2byte(hex);
        deserialize(bb);
    }

    public GrdData3(List<String> dataList, String publicEccKeyHex, int rdKey) {
        this.dataList = dataList;
        this.publicEccKey = GrdDongle.hex2byte(publicEccKeyHex);
        this.rdKey = rdKey;
    }

    public List<String> getDataList() {
        return dataList;
    }

    // alg-0    ECC-160
    // 1        RND64
    // 2        AES128
    public byte[] serialize() throws GrdException {
        if (publicEccKey == null || publicEccKey.length != SZ_PUB_KEY_ECC
            || rdKey == 0) {
            throw new GrdException("[GrdData#serialize] wrong param");
        }

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream w = new DataOutputStream(baos);
        Iterator<String> iterator = dataList.iterator();
        while (iterator.hasNext()) {
            serizalizeString(w, iterator.next());
        }

        byte[] bb = baos.toByteArray();
        if (GrdDongle.DEBUG) {
            System.out.println("* lic\n" + GrdDongle.byte2hex(bb, 0));
        }

        // смешиваем через один байт публ ключ ECC-160 alg0
        byte[] bb2 = mix(bb, publicEccKey);
        if (GrdDongle.DEBUG) {
            System.out.println("* mixed lic & pub-key\n" + GrdDongle.byte2hex(bb2, 0));
        }

        // вычисляем контрольную
        byte[] bb3;
        {
            CRC32 cr = new CRC32();
            cr.update(bb2);
            long ncrc = cr.getValue();
            if (GrdDongle.DEBUG) {
                System.out.println("* crc32\n" + ncrc);
            }

            bb3 = mix(bb2, ByteBuffer.allocate(8).putLong(ncrc).array());
        }

        if (GrdDongle.DEBUG) {
            System.out.println("* mixed lic & pub-key & crc\n" + GrdDongle.byte2hex(bb3, 0));
        }

        // шифруем AES128
        {
            Handle h = GrdDongle.getHandle(SEARCH_DONGLE_VER);

            byte[] iv = new byte[16];
            GrdE errorCode = Api.GrdPI_Read(h, 0x3, 0, iv, rdKey);
            GrdDongle.errorHandling(errorCode);

            errorCode = Api.GrdPI_Activate(h, 0x2, rdKey);
            GrdDongle.errorHandling(errorCode);

            errorCode = Api.GrdCryptEx(h, 0x2, bb3, GrdAM.Encode.getValue() + GrdAM.OFB.getValue(), iv);
            GrdDongle.errorHandling(errorCode);
        }

        if (GrdDongle.DEBUG) {
            System.out.println("* enc\n" + GrdDongle.byte2hex(bb3, 0));
        }

        // смешиваем через один ключ активации alg-3 в d7
        byte[] bb4 = mix(bb3, ByteBuffer.allocate(4).putInt(rdKey).array());
        if (GrdDongle.DEBUG) {
            System.out.println("* enc with rd key\n" + GrdDongle.byte2hex(bb4, 0));
        }

        return bb4;
    }

    private static final int MIN_LIC_LEN = SZ_PUB_KEY_ECC + 8 + 4 + 8 + 1;

    public void deserialize(byte[] bb4) throws GrdException {
        if (bb4 == null || bb4.length < MIN_LIC_LEN) {
            throw new GrdException("[GrdData#deserialize] wrong lic len");
        }

        // ключ активации alg-3
        byte[] bb3;
        {
            byte[] b7 = new byte[4];
            bb3 = new byte[bb4.length - 4];
            demix(bb4, bb3, b7);
            if (GrdDongle.DEBUG) {
                System.out.println("* enc\n" + GrdDongle.byte2hex(bb3, 0));
            }
            rdKey = ByteBuffer.wrap(b7).getInt();
        }

        // дешифруем AES128
        {
            Handle h = GrdDongle.getHandle(SEARCH_DONGLE_VER);

            GrdE errorCode = Api.GrdPI_Activate(h, 0x2, rdKey);
            GrdDongle.errorHandling(errorCode);

            byte[] iv = new byte[16];
            errorCode = Api.GrdPI_Read(h, 0x3, 0, iv, rdKey);
            GrdDongle.errorHandling(errorCode);

            errorCode = Api.GrdCryptEx(h, 0x2, bb3, GrdAM.Decode.getValue() + GrdAM.OFB.getValue(), iv);
            GrdDongle.errorHandling(errorCode);
            if (GrdDongle.DEBUG) {
                System.out.println("* mixed lic & pub-key & crc\n" + GrdDongle.byte2hex(bb3, 0));
            }
        }

        // проверяем контрольную
        byte[] bb2;
        {
            byte[] bcrc = new byte[8];
            bb2 = new byte[bb3.length - 8];
            demix(bb3, bb2, bcrc);
            if (GrdDongle.DEBUG) {
                System.out.println("* mixed lic & pub-key\n" + GrdDongle.byte2hex(bb2, 0));
            }

            long ncrc = ByteBuffer.wrap(bcrc).getLong();
            CRC32 cr = new CRC32();
            cr.update(bb2);
            long n2 = cr.getValue();
            if (ncrc != n2) {
                if (GrdDongle.DEBUG) {
                    System.out.println("* crc32\n" + ncrc + ", but " + n2);
                }
                throw new GrdException("[GrdData#deserialize] wrong crc");
            }
        }

        // публ ключ ECC160
        publicEccKey = new byte[SZ_PUB_KEY_ECC];
        byte bb[] = new byte[bb2.length - SZ_PUB_KEY_ECC];
        demix(bb2, bb, publicEccKey);

        if (GrdDongle.DEBUG) {
            System.out.println("* lic\n" + GrdDongle.byte2hex(bb, 0));
        }

        ByteArrayInputStream bais = new ByteArrayInputStream(bb);
        DataInputStream r = new DataInputStream(bais);
        String item = null;
        do {
            item = deserizalizeString(r);
            if(item != null) {
                dataList.add(item);
            }
        }
        while(item != null);
    }

    private void serizalizeString(DataOutputStream w, String s) throws GrdException {
        try {
            if (s != null && !s.isEmpty()) {
                byte[] bb = s.getBytes("UTF-8");
                w.writeInt(bb.length);
                w.write(bb);
            } else {
                w.writeInt(0);
            }
        } catch (UnsupportedEncodingException ex) {
            throw new GrdException(ex);
        } catch (IOException ex) {
            throw new GrdException(ex);
        }
    }

    private String deserizalizeString(DataInputStream r) throws GrdException {
        try {
            int len = r.readInt();
            if (len > 0) {
                byte[] bb = new byte[len];
                if (r.read(bb) == len) {
                    return new String(bb, "UTF-8");
                }
            } else {
                return "";
            }
        } catch (IOException ex) {
            return null;
        }
        return null;
    }

    public byte[] getPublicEccKey() {
        return publicEccKey;
    }

    public int getRdKey() {
        return rdKey;
    }
    
    private static final String T1 = "*** lic *************************************************\r\n";

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("\r\n" + T1);
        Iterator<String> nameIterator = valueNameList.iterator();
        Iterator<String> iterator = dataList.iterator();
        while (nameIterator.hasNext() && iterator.hasNext()) {
           sb.append(nameIterator.next() + iterator.next()).append("\r\n");
        }
        sb.append(T1);
        return sb.toString();
    }

    /**
     * Смешивание через один bb1 и bb2.
     *
     * @param bb1
     * @param bb2
     * @return
     */
    private byte[] mix(byte[] bb1, byte[] bb2) throws GrdException {
        if (bb1 == null || bb2 == null) {
            throw new GrdException("[GrdData#mix] wrong parameters");
        }

        byte[] bb3 = new byte[bb1.length + bb2.length];
        int i = 0, j = 0;
        for (; i < bb1.length && i < bb2.length; i++) {
            bb3[j++] = bb1[i];
            bb3[j++] = bb2[i];
        }

        if (bb1.length > bb2.length) {
            for (; i < bb1.length; i++, j++) {
                bb3[j] = bb1[i];
            }
        } else {
            for (; i < bb2.length; i++, j++) {
                bb3[j] = bb2[i];
            }
        }
        return bb3;
    }

    private void demix(byte[] bin, byte[] bout1, byte[] bout2) throws GrdException {
        if (bin == null || bout1 == null || bout2 == null
            || bin.length != bout1.length + bout2.length) {
            throw new GrdException("[GrdData#demix] wrong parameters");
        }

        for (int i = 0, j = 0; i < bin.length; j++) {
            if (j < bout1.length) {
                bout1[j] = bin[i++];
            }

            if (j < bout2.length) {
                bout2[j] = bin[i++];
            }
        }
    }
}
