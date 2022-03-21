/*
 * GrdSC.java
 *
 * Created on 23 Ноябрь 2007 г., 1:11
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

public class GrdSC {

    /**
     * First data block
     */
    public static final GrdSC First = new GrdSC(0x100);
    /**
     * Next data block
     */
    public static final GrdSC Next = new GrdSC(0x200);
    /**
     * Last data block
     */
    public static final GrdSC Last = new GrdSC(0x400);
    /**
     * All blocks
     */
    public static final GrdSC All = new GrdSC((First.mValue + Next.mValue + Last.mValue));

    /**
     * Software implemented cryptographic algorithm
     */
    public static final GrdSC AES256 = new GrdSC(GrdSA.SoftAlgo.mValue + 0);

    int mValue;

    GrdSC(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
