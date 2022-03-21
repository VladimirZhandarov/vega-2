/*
 * GrdGIL.java
 *
 * Created on 23 Ноябрь 2007 г., 1:02
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Константы для получения информации с помощью функции GrdGetInfo.
 * <br/>Информация о текущем ключе.
 */
public class GrdGIL extends GrdInfo {

    /**
     * Local or remote dongle
     */
    public static final GrdGIL Remote = new GrdGIL(0x3000);
    /**
     * ID of current dongle
     */
    public static final GrdGIL ID = new GrdGIL(0x3001);
    /**
     * Model of current dongle
     */
    public static final GrdGIL Model = new GrdGIL(0x3002);
    /**
     * Interface of current dongle
     */
    public static final GrdGIL Interface = new GrdGIL(0x3003);
    /**
     * Lock counter value for current dongle
     */
    public static final GrdGIL LockCounter = new GrdGIL(0x3005);
    /**
     * Current dongle memory address
     */
    public static final GrdGIL Seek = new GrdGIL(0x3006);
    /**
     * Driver version (0x04801234 = 4.80.12.34)
     */
    public static final GrdGIL DrvVers = new GrdGIL(0x4000);
    /**
     * Driver build
     */
    public static final GrdGIL DrvBuild = new GrdGIL(0x4001);
    /**
     * LPT port address
     */
    public static final GrdGIL PortLPT = new GrdGIL(0x4002);
    /**
     * Software dongle container file name unicode string
     */
    public static final GrdGIL SoftFileName = new GrdGIL(0x4003);

    GrdGIL(int value) {
        super(value);
    }
}
