/*
 * GrdUAM.java
 *
 * Created on 23 Ноябрь 2007 г., 0:51
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * UAM-адреса полей данных для использования в функциях GrdRead и GrdWrite
 */
public class GrdUAM {

    /**
     * 00h Programm number
     */
    public static final GrdUAM NProg = new GrdUAM(30 - 30);
    /**
     * 01h Version
     */
    public static final GrdUAM Ver = new GrdUAM(31 - 30);
    /**
     * 02h Serial number
     */
    public static final GrdUAM SN = new GrdUAM(32 - 30);
    /**
     * 04h Bit mask
     */
    public static final GrdUAM Mask = new GrdUAM(34 - 30);
    /**
     * 06h Counter #1 =new GrdUAM(GP)
     */
    public static final GrdUAM GP = new GrdUAM(36 - 30);
    /**
     * 08h Current network license limit
     */
    public static final GrdUAM RealLANRes = new GrdUAM(38 - 30);
    /**
     * 0Ah Index
     */
    public static final GrdUAM Index = new GrdUAM(40 - 30);
    /**
     * 0Eh User data, algorithm descriptors
     */
    public static final GrdUAM AlgoAddr = new GrdUAM(44 - 30);

    int mValue;

    public GrdUAM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
