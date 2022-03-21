/*
 * GrdSHA256.java
 *
 * Created on 23 Ноябрь 2007 г., 1:20
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Определяет размер дайджеста и контекста для алгоритма SHA256
 */
public class GrdSHA256 {

    /**
     * Размер дайджеста
     */
    public static final GrdSHA256 DIGEST_SIZE = new GrdSHA256(32);
    /**
     * Должно быть >=sizeof(SHA256_CONTEXT)
     */
    public static final GrdSHA256 CONTEXT_SIZE = new GrdSHA256(0x200);

    int mValue;

    GrdSHA256(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }

}
