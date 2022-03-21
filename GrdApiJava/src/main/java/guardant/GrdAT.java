/*
 * GrdAT.java
 *
 * Created on 23 Ноябрь 2007 г., 1:41
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Алгоритмы ключей Guardant Stealth
 */
public class GrdAT {

    /**
     * Basic method
     */
    public static final GrdAT Algo0 = new GrdAT(0);
    /**
     * Character method
     */
    public static final GrdAT AlgoASCII = new GrdAT(1);
    /**
     * File method
     */
    public static final GrdAT AlgoFile = new GrdAT(2);

    int mValue;

    GrdAT(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
