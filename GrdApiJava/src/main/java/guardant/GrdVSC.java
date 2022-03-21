/*
 * GrdVSC.java
 *
 * Created on 27 Апрель 2009 г., 12:17
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

public class GrdVSC {

    /**
     * ECC160 message size
     */
    public static final GrdVSC ECC160 = new GrdVSC(0);

    int mValue;

    GrdVSC(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
