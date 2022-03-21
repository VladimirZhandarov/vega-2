package wit.guardant.ticket;

import guardant.Api;
import guardant.GrdAM;
import guardant.GrdE;
import guardant.Handle;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
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
public class GrdData implements Serializable {

    private static final long serialVersionUID = -1L;
    /**
     * Параметр по которому ищем.
     */
    public static final byte SEARCH_DONGLE_VER = 0;
    /**
     * Прибавить к кодам.
     */
    public static final int C2 = 0x15DCB;

    /**
     * Длина публ ключа ECC160.
     */
    private static final int SZ_PUB_KEY_ECC = 40;
    /**
     * vendor
     */
    private String d0;
    /*
     * Customer.
     */
    private String d1;
    /**
     * Имя ПО.
     */
    private String d2;
    /**
     * Версия.
     */
    private byte d3;
    /**
     * Дата выдачи.
     */
    private long d4;
    /**
     * Product SN.
     */
    private String d5;
    // 579BCBFC51C14A1D9AF483C59EE496D24BCB3439D0B9EA1962CB9B1C4C4FD6BD77CCDEFA4FD40C6B
    /**
     * Pub key, sign ECC-160, alg-0.
     */
    private byte[] d6;
    // 1558217874
    /**
     * Ключ чтения
     */
    private int d7;

//    // SHA256
//    public static byte[] digSha(byte[] bb) throws GrdException {
//        try {
//            MessageDigest md = MessageDigest.getInstance("SHA-256");
//            md.update(bb);
//            byte sha[] = md.digest();
//            return sha;
//        } catch (NoSuchAlgorithmException ex) {
//            throw new GrdException(ex);
//        }
//    }
    /**
     * CTOR.
     *
     * для загрузки библиотеки в GF.
     */
    public GrdData() {
    }

    public GrdData(String hex) throws GrdException {
        byte[] bb = GrdDongle.hex2byte(hex);
        deserialize(bb);
    }

    public GrdData(String d0, String d1, String d2, byte d3, long d4, String d5, String hexD6, int d7) {
        this.d0 = d0;
        this.d1 = d1;
        this.d2 = d2;
        this.d3 = d3;
        this.d4 = d4;
        this.d5 = d5;
        this.d6 = GrdDongle.hex2byte(hexD6);
        this.d7 = d7;
    }

    // alg-0    ECC-160
    // 1        RND64
    // 2        AES128
    public byte[] serialize() throws GrdException {
        if (d1 == null || d1.isEmpty()
            || d2 == null || d2.isEmpty()
            || d5 == null || d5.isEmpty()
            || d6 == null || d6.length != SZ_PUB_KEY_ECC
            || d7 == 0) {
            throw new GrdException("[GrdData#serialize] wrong param");
        }

        ByteArrayOutputStream bous = new ByteArrayOutputStream();
        //
        serStr(bous, d0);
        serStr(bous, d1);
        serStr(bous, d2);
        bous.write(d3);

        {
            try {
                byte bb[] = ByteBuffer.allocate(8).putLong(d4).array();
                bous.write(bb);
            } catch (IOException ex) {
                throw new GrdException(ex);
            }
        }

        serStr(bous, d5);
        //
        byte[] bb = bous.toByteArray();
        if (GrdDongle.DEBUG) {
            System.out.println("* lic\n" + GrdDongle.byte2hex(bb, 0));
        }

        // смешиваем через один байт публ ключ ECC-160 alg0
        byte[] bb2 = mix(bb, d6);
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
            GrdE errorCode = Api.GrdPI_Read(h, 0x3, 0, iv, d7);
            GrdDongle.errorHandling(errorCode);

            errorCode = Api.GrdCryptEx(h, 0x2, bb3, GrdAM.Encode.getValue() + GrdAM.OFB.getValue(), iv);
            GrdDongle.errorHandling(errorCode);
        }

        if (GrdDongle.DEBUG) {
            System.out.println("* enc\n" + GrdDongle.byte2hex(bb3, 0));
        }

        // смешиваем через один ключ активации alg-3 в d7
        byte[] bb4 = mix(bb3, ByteBuffer.allocate(4).putInt(d7).array());
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
            d7 = ByteBuffer.wrap(b7).getInt();
        }

        // дешифруем AES128
        {
            Handle h = GrdDongle.getHandle(SEARCH_DONGLE_VER);

            byte[] iv = new byte[16];
            GrdE errorCode = Api.GrdPI_Read(h, 0x3, 0, iv, d7);
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
        d6 = new byte[SZ_PUB_KEY_ECC];
        byte bb[] = new byte[bb2.length - SZ_PUB_KEY_ECC];
        demix(bb2, bb, d6);

        if (GrdDongle.DEBUG) {
            System.out.println("* lic\n" + GrdDongle.byte2hex(bb, 0));
        }

        //
        ByteArrayInputStream bis = new ByteArrayInputStream(bb);
        d0 = deserStr(bis);
        d1 = deserStr(bis);
        d2 = deserStr(bis);

        {
            byte[] b = new byte[1];
            try {
                int n;
                if ((n = bis.read(b)) != 1) {
                    throw new GrdException("[GrdData#deserialize] wrong len need 1, but: " + n);
                }

                d3 = b[0];
            } catch (IOException ex) {
                throw new GrdException(ex);
            }
        }

        {
            byte[] b = new byte[8];
            try {
                int n;
                if ((n = bis.read(b)) != 8) {
                    throw new GrdException("[GrdData#deserialize] wrong len need 8, but: " + n);
                }

                d4 = ByteBuffer.wrap(b).getLong();
            } catch (IOException ex) {
                throw new GrdException(ex);
            }
        }

        d5 = deserStr(bis);
    }

    private void serStr(ByteArrayOutputStream baos, String s) throws GrdException {
        try {
            byte[] bb = s.getBytes("UTF-8");
            baos.write((byte) bb.length);
            baos.write(bb);
        } catch (UnsupportedEncodingException ex) {
            throw new GrdException(ex);
        } catch (IOException ex) {
            throw new GrdException(ex);
        }
    }

    private String deserStr(ByteArrayInputStream bis) throws GrdException {
        try {
            byte[] bb = new byte[1];
            if (bis.read(bb) == 1) {
                byte len = bb[0];
                byte[] bb2 = new byte[len];
                if (bis.read(bb2) == len) {
                    return new String(bb2, "UTF-8");
                }
            }
        } catch (IOException ex) {
            throw new GrdException(ex);
        }
        return null;
    }

    /**
     * vendor
     *
     * @return
     */
    public String getD0() {
        return d0;
    }

    /*
     * Customer.
     */
    public String getD1() {
        return d1;
    }

    /**
     * Имя ПО.
     *
     * @return
     */
    public String getD2() {
        return d2;
    }

    /**
     * Версия.
     *
     * @return
     */
    public byte getD3() {
        return d3;
    }

    /**
     * Дата выдачи.
     *
     * @return
     */
    public long getD4() {
        return d4;
    }

    /**
     * Product SN.
     *
     * @return
     */
    public String getD5() {
        return d5;
    }

    /**
     * Pub key, sign ECC-160, alg-0.
     *
     * @return
     */
    public String getD6() {
        return GrdDongle.byte2hex(d6, 0);
    }

    /**
     * Ключ чтения/активации
     *
     * @return
     */
    public int getD7() {
        return d7;
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
