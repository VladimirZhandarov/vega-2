/*
 * GrdDC.java
 *
 * Created on 23 Ноябрь 2007 г., 1:45
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Демонстрационные коды
 */
public class GrdDC {

    /**
     * Demo public code
     */
    public static final GrdDC DEMONVK = new GrdDC(0x519175b7);
    /**
     * Demo private read code
     */
    public static final GrdDC DEMORDO = new GrdDC(0x51917645);
    /**
     * Demo private write code
     */
    public static final GrdDC DEMOPRF = new GrdDC(0x51917603);
    /**
     * Demo private master code
     */
    public static final GrdDC DEMOMST = new GrdDC(0x5191758c);

    int mValue;

    GrdDC(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
