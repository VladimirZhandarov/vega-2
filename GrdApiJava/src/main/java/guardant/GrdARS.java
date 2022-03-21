/*
 * GrdARS.java
 *
 * Created on 23 Ноябрь 2007 г., 1:08
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Guardant Stealth III: Размер вопроса к алгоритмам и защищенным ячейкам по умолчанию
 */
public class GrdARS {

    /**
     * GSII64 for automatic protection and use in API
     */
    public static final GrdARS GSII64 = new GrdARS(8);
    /**
     * HASH64 for automatic protection and use in API
     */
    public static final GrdARS HASH64 = new GrdARS(8);
    /**
     * RAND64 for automatic protection and use in API
     */
    public static final GrdARS RAND64 = new GrdARS(8);
    /**
     * Protected Item for read only data. Can update via Safety Guardant Remote Update
     */
    public static final GrdARS READ_ONLY = new GrdARS(8);
    /**
     * Protected Item for read/write data. Can update at protected application runtime
     */
    public static final GrdARS READ_WRITE = new GrdARS(8);
    /**
     * GSII64 demo algo for use in guardant examlpes
     */
    public static final GrdARS GSII64_DEMO = new GrdARS(8);
    /**
     * HASH64 demo algo for use in guardant examlpes
     */
    public static final GrdARS HASH64_DEMO = new GrdARS(8);
    /**
     * ECC160 for automatic protection & use in API
     */
    public static final GrdARS ECC160 = new GrdARS(20);
    /**
     * AES128 for automatic protection & use in API
     */
    public static final GrdARS AES128 = new GrdARS(16);
    /**
     * SHA256 for automatic protection & use in API
     */
    public static final GrdARS HASH_SHA256 = new GrdARS(32);

    int mValue;

    GrdARS(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
