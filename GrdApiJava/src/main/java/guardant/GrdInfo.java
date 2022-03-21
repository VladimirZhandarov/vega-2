/*
 * GrdInfo.java
 *
 * Created on 23 Ноябрь 2007 г., 0:59
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Базовый класс для получения информации с помощью функции GrdGetInfo.
 */
public class GrdInfo {

    int mValue;

    public GrdInfo(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
