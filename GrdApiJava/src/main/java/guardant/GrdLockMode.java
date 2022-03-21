/*
 * GrdLM.java
 *
 * Created on 23 Ноябрь 2007 г., 0:41
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Флаги для функции <b>GrdLock</b>
 */
public class GrdLockMode {

    /**
     * Блокируется только вызов GrdLock из другого потока, процесса или даже компьютера = new GrdLM(при работе в сети)
     */
    public static final GrdLockMode Nothing = new GrdLockMode(0x00000000);
    /**
     * Блокируются операции Init
     */
    public static final GrdLockMode Init = new GrdLockMode(0x00000001);
    /**
     * Блокируются операции Protect
     */
    public static final GrdLockMode Protect = new GrdLockMode(0x00000002);
    /**
     * Блокируются операции Transform
     */
    public static final GrdLockMode Transform = new GrdLockMode(0x00000004);
    /**
     * Блокируются операции Read
     */
    public static final GrdLockMode Read = new GrdLockMode(0x00000008);
    /**
     * Блокируются операции Write
     */
    public static final GrdLockMode Write = new GrdLockMode(0x00000010);
    /**
     * Блокируются операции Activate
     */
    public static final GrdLockMode Activate = new GrdLockMode(0x00000020);
    /**
     * Блокируются операции Deactivate
     */
    public static final GrdLockMode Deactivate = new GrdLockMode(0x00000040);
    /**
     * Блокируются операции ReadItem
     */
    public static final GrdLockMode ReadItem = new GrdLockMode(0x00000080);
    /**
     * Блокируются операции UpdateItem
     */
    public static final GrdLockMode UpdateItem = new GrdLockMode(0x00000100);
    /**
     * Блокируются все вышеперечисленные операции
     */
    public static final GrdLockMode All = new GrdLockMode(0xFFFFFFFF);

    int mValue;

    GrdLockMode(int value) {
        mValue = value;
    }

    public GrdLockMode(GrdLockMode[] flags) {
        mValue = 0;
        for (int i = 0; i < flags.length; i++) {
            mValue = mValue | flags[i].mValue;
        }
    }

    public GrdLockMode(GrdLockMode f1, GrdLockMode f2) {
        mValue = f1.mValue | f2.mValue;
    }

    public GrdLockMode(GrdLockMode f1, GrdLockMode f2, GrdLockMode f3) {
        mValue = f1.mValue | f2.mValue | f3.mValue;
    }

    public GrdLockMode(GrdLockMode f1, GrdLockMode f2, GrdLockMode f3, GrdLockMode f4) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue;
    }

    public int getValue() {
        return mValue;
    }

}
