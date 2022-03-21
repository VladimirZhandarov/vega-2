/*
 * GrdADS.java
 *
 * Created on 23 Ноябрь 2007 г., 1:10
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Guardant Stealth III: размеры определителей алгоритмов и защищенных ячеек по умолчанию
 */
public class GrdADS {

    /**
     * GSII64 for automatic protection and use in API
     */
    public static final GrdADS GSII64 = new GrdADS(16);
    /**
     * HASH64 for automatic protection and use in API
     */
    public static final GrdADS HASH64 = new GrdADS(16);
    /**
     * RAND64 for automatic protection and use in API
     */
    public static final GrdADS RAND64 = new GrdADS(16);
    /**
     * Protected Item for read only data. Can update via Safety Guardant Remote Update
     */
    public static final GrdADS READ_ONLY = new GrdADS(8);
    /**
     * Protected Item for read/write data. Can update at protected application runtime
     */
    public static final GrdADS READ_WRITE = new GrdADS(8);
    /**
     * GSII64 demo algo for use in guardant examlpes
     */
    public static final GrdADS GSII64_DEMO = new GrdADS(16);
    /**
     * HASH64 demo algo for use in guardant examlpes
     */
    public static final GrdADS HASH64_DEMO = new GrdADS(16);
    /**
     * ECC160 for automatic protection & use in API
     */
    public static final GrdADS ECC160 = new GrdADS(20);
    /**
     * AES128 for automatic protection & use in API
     */
    public static final GrdADS AES128 = new GrdADS(16);

    int mValue;

    GrdADS(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
