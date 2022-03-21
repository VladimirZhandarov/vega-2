/*
 * GrdAES256.java
 *
 * Created on 23 Ноябрь 2007 г., 1:19
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Определяет размер ключа и блока данных для алгоритма AES256
 */
public class GrdAES256 {

    /**
     * Размер ключа шифрования
     */
    public static final GrdAES256 KEY_SIZE = new GrdAES256(32);
    /**
     * Размер шифруемого блока данных
     */
    public static final GrdAES256 BLOCK_SIZE = new GrdAES256(16);

    int mValue;

    GrdAES256(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
