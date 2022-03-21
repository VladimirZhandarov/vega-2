/*
 * GrdAM.java
 *
 * Created on 23 Ноябрь 2007 г., 1:06
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

public class GrdGF {

    /**
     * Disable RTC modification by GrdSetTime call
     */
    public static final GrdGF ProtectTime = new GrdGF(1);
    /**
     * Enable HID mode
     */
    public static final GrdGF HID = new GrdGF(2);
    /**
     * Allow only one instance of Guardant API Protected application to run
     */
    public static final GrdGF OneSessKey = new GrdGF(4);
    /**
     * Allow only one instance of AutoProtected application to run
     */
    public static final GrdGF TwoSessKey = new GrdGF(8);

    int mValue;

    GrdGF(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
