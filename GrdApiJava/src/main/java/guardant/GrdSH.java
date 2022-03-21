/*
 * GrdSH.java
 *
 * Created on 23 Ноябрь 2007 г., 1:35
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Номера программно-реализованных алгоритмов
 */
public class GrdSH {

    /**
     * Программно-реализованный алгоритм вычисления CRC
     */
    public static final GrdSH CRC32 = new GrdSH(GrdSA.SoftAlgo.mValue + 0);
    /**
     * Программно-реализованный алгоритм хеширования
     */
    public static final GrdSH SHA256 = new GrdSH(GrdSA.SoftAlgo.mValue + 1);

    int mValue;

    GrdSH(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
