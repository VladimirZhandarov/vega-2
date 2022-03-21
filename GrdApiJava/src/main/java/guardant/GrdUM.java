/*
 * GrdUM.java
 *
 * Created on 23 Ноябрь 2007 г., 1:44
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Методы обновления защищенных ячеек
 */
public class GrdUM {

    /**
     * Overwrites current value of protected item
     */
    public static final GrdUM MOV = new GrdUM(0);
    /**
     * Applies logical XOR to previously stored value of PI and the one being written
     */
    public static final GrdUM XOR = new GrdUM(1);

    int mValue;

    GrdUM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
