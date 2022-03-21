/*
 * GrdTRU.java
 *
 * Created on 23 Ноябрь 2007 г., 1:40
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Режим работы GrdTRU_PrepareData
 */
public class GrdTRU {

    /**
     * '1' - Execute Init before Update
     */
    public static final GrdTRU GrdTRU_Flags_Init = new GrdTRU(1);
    /**
     * '2' - Execute Protect before Update
     */
    public static final GrdTRU GrdTRU_Flags_Protect = new GrdTRU(2);

    int mValue;

    GrdTRU(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
