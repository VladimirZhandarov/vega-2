/*
 * GrdHASH64.java
 *
 * Created on 23 Ноябрь 2007 г., 1:34
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Определяет размер дайджеста при вычислении HASH64
 */
public class GrdHASH64 {

    /**
     * Размер дайджеста
     */
    public static final GrdHASH64 DIGEST_SIZE = new GrdHASH64(8);

    int mValue;

    GrdHASH64(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
