/*
 * GrdAP.java
 *
 * Created on 23 Ноябрь 2007 г., 1:21
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Пароли к демонстрационным алгоритмам Guardant GSII64
 */
public class GrdAP {

    /**
     * Demo GSII64 activation passInt16
     */
    public static final GrdAP GSII64_DEMO_ACTIVATION = new GrdAP(0xAAAAAAAA);
    /**
     * Demo GSII64 deactivation passInt16
     */
    public static final GrdAP GSII64_DEMO_DEACTIVATION = new GrdAP(0xDDDDDDDD);
    /**
     * Demo GSII64 read passInt16
     */
    public static final GrdAP GSII64_DEMO_READ = new GrdAP(0xBBBBBBBB);
    /**
     * Demo GSII64 update passInt16
     */
    public static final GrdAP GSII64_DEMO_UPDATE = new GrdAP(0xCCCCCCCC);
    /**
     * Demo HASH64 activation passInt16
     */
    public static final GrdAP HASH64_DEMO_ACTIVATION = new GrdAP(0xAAAAAAAA);
    /**
     * Demo HASH64 deactivation passInt16
     */
    public static final GrdAP HASH64_DEMO_DEACTIVATION = new GrdAP(0xDDDDDDDD);
    /**
     * Demo HASH64 read passInt16
     */
    public static final GrdAP HASH64_DEMO_READ = new GrdAP(0xBBBBBBBB);
    /**
     * Demo HASH64 update passInt16
     */
    public static final GrdAP HASH64_DEMO_UPDATE = new GrdAP(0xCCCCCCCC);

    int mValue;

    GrdAP(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
