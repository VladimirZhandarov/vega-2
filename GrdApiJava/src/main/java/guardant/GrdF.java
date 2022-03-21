/*
 * GrdF.java
 *
 * Created on 23 Ноябрь 2007 г., 0:38
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Режим поиска для функции GrdFind
 */
public class GrdF {

    /**
     * Первый вызов
     */
    public static final GrdF First = new GrdF(1);
    /**
     * Все последующие вызовы
     */
    public static final GrdF Next = new GrdF(0);

    int mValue;

    GrdF(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
