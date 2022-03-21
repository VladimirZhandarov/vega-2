/*
 * GrdSA.java
 *
 * Created on 23 Ноябрь 2007 г., 1:14
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Базовый программный алгоритм
 */
public class GrdSA {

    /**
     * Hardware implemented cryptographic or hashing algorithm flag
     */
    public static final GrdSA SoftAlgo = new GrdSA(0x80000000);

    int mValue;

    GrdSA(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
