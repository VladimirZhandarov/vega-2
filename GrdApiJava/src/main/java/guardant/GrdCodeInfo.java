/*
 * GrdCodeInfo.java
 *
 * Created on 6 Май 2009 г., 18:36
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

public class GrdCodeInfo {

    /**
     * Flash start address for loadable code.
     */
    public int mStartAddr;
    /**
     * Flash size for loadable code.
     */
    public int mCodeSizeMax;
    /**
     * Flash sector size for loadable code.
     */
    public int mCodeSectorSize;
    /**
     * RAM start address for loadable code.
     */
    public int mStartRamAddr;
    /**
     * RAM size for loadable code.
     */
    public int mRamSizeMax;
    /**
     * ...
     */
    GrdCodePublicData mGrdCodePublicData = new GrdCodePublicData();
    /**
     * Hash of loadable code.
     */
    public byte[] mHashLoadableCode = new byte[32];
}
