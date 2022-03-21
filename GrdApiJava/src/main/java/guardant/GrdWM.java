/*
 * GrdWM.java
 *
 * Created on 23 Ноябрь 2007 г., 0:44
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Режим работы, задается в функции GrdSetWorkMode
 */
public class GrdWM {

    /**
     * Задает режим адресации UAM (User Address Mode)в операциях чтения/записи. Режим по умолчанию
     */
    public static final GrdWM UAM = new GrdWM(0x0000);
    /**
     * Задает режим адресации SAM (System Address Mode) в операциях чтения/записи (по умолчанию - режим UAM)
     */
    public static final GrdWM SAM = new GrdWM(0x0080);
    /**
     * Зарезервировано
     */
    public static final GrdWM CodeIsString = new GrdWM(0x0100);
    /**
     * Запретить автоподстройку протокола обмена с ключом
     */
    public static final GrdWM NoRetry = new GrdWM(0x0200);
    /**
     * При обращении к ключу не захватывать ресурс параллельного порта полностью
     */
    public static final GrdWM NoFullAccess = new GrdWM(0x0400);
    /**
     * Искать ключ только в параллельном порту LPT1 (адрес 0x378)
     */
    public static final GrdWM OnlyStdLPT1 = new GrdWM(0x0800);
    /**
     * Искать ключ только в параллельном порту LPT2 (адрес 0x278)
     */
    public static final GrdWM OnlyStdLPT2 = new GrdWM(0x1000);
    /**
     * Искать ключ только в параллельном порту LPT3 (адрес 0x3BC). Если ни один из этих 3 флагов не установлен), поиск ключа будет производиться во всех
     * имеющихся параллельных портах
     */
    public static final GrdWM OnlyStdLPT3 = new GrdWM(0x2000);
    /**
     * Указывает), что сегмент данных отличается от стандартного
     */
    public static final GrdWM NoAutoMem32 = new GrdWM(0x4000);
    /**
     * Зарезервировано
     */
    public static final GrdWM UseOldCRC = new GrdWM(0x8000);
    /**
     * Зарезервировано. Искать ключ в параллельном порту с нестандартным адресом
     */
    public static final GrdWM NotStdLPTAddr = new GrdWM(0x02000000);

    int mValue;

    GrdWM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
