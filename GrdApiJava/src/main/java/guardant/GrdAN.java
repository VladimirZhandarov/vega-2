/*
 * GrdAN.java
 *
 * Created on 23 Ноябрь 2007 г., 1:05
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Guardant Stealth III: номера алгоритмов и защищенных ячеек в прошивке по умолчанию
 */
public class GrdAN {

    /**
     * GSII64 for automatic protection + use in API
     */
    public static final GrdAN GSII64 = new GrdAN(0);
    /**
     * HASH64 for automatic protection + use in API
     */
    public static final GrdAN HASH64 = new GrdAN(1);
    /**
     * RAND64 for automatic protection + use in API
     */
    public static final GrdAN RAND64 = new GrdAN(2);
    /**
     * Protected Item for read only data. Can be updated via Secured Guardant Remote Update
     */
    public static final GrdAN READ_ONLY = new GrdAN(3);
    /**
     * Protected Item for read/write data. Can be updated at protected application runtime
     */
    public static final GrdAN READ_WRITE = new GrdAN(4);
    /**
     * GSII64 demo algo for use in guardant examlpes
     */
    public static final GrdAN GSII64_DEMO = new GrdAN(5);
    /**
     * HASH64 demo algo for use in guardant examlpes
     */
    public static final GrdAN HASH64_DEMO = new GrdAN(6);
    /**
     * ECC160 for automatic protection & use in API
     */
    public static final GrdAN ECC160 = new GrdAN(8);
    /**
     * AES128 for automatic protection & use in API
     */
    public static final GrdAN AES128 = new GrdAN(9);
    /**
     * GSII64_ENCRYPT for automatic protection & use in API
     */
    public static final GrdAN GSII64_ENCRYPT = new GrdAN(10);
    /**
     * GSII64_DECRYPT for automatic protection & use in API
     */
    public static final GrdAN GSII64_DECRYPT = new GrdAN(11);

    int mValue;

    GrdAN(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
