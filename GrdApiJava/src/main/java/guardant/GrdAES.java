/*
 * GrdAES.java
 *
 * Created on 23 Ноябрь 2007 г., 1:17
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Определяет размер контекста для программного алгоритма AES
 */
public class GrdAES {

    /**
     * Должно быть >=sizeof(AES_CONTEXT)
     */
    public static final GrdAES CONTEXT_SIZE = new GrdAES(0x4000);

    int mValue;

    GrdAES(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
