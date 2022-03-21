/*
 * GrdFMI.java
 *
 * Created on 23 Ноябрь 2007 г., 0:34
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Флаги, задающие список возможных интерфейсов ключей, участвующих в поиске
 */
public class GrdFMI {

    /**
     * Любой из возможных для Guardant Stealth и Fidus интерфесов
     */
    public static final GrdFMI ALL = new GrdFMI(0);
    /**
     * LPT порт
     */
    public static final GrdFMI LPT = new GrdFMI(1 << GrdDI.LPT.mValue);
    /**
     * USB порт
     */
    public static final GrdFMI USB = new GrdFMI(1 << GrdDI.USB.mValue);
    /**
     * Virtual bus
     */
    public static final GrdFMI VIRTUAL = new GrdFMI(1 << (int) GrdDI.VIRTUAL.mValue);

    int mValue;

    public GrdFMI(int value) {
        mValue = value;

    }

    public int getValue() {
        return mValue;
    }
}
