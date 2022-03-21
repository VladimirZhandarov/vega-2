/*
 * GrdFAM.java
 *
 * Created on 23 Ноябрь 2007 г., 0:54
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * SAM-адреса полей данных для использования в функциях GrdRead и GrdWrite
 */
public class GrdSAM {

    /**
     * 0h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM KeyModelAddr = new GrdSAM(0);
    /**
     * 1h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM KeyMemSizeAddr = new GrdSAM(1);
    /**
     * 2h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM KeyProgVerAddr = new GrdSAM(2);
    /**
     * 3h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM KeyProtocolAddr = new GrdSAM(3);
    /**
     * 4h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства) Example: 0x104 = new GrdSAM(1.4
     */
    public static final GrdSAM ClientVerAddr = new GrdSAM(4);
    /**
     * 6h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM KeyUserAddrAddr = new GrdSAM(6);
    /**
     * 7h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM KeyAlgoAddrAddr = new GrdSAM(7);
    /**
     * 8h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM PrnportAddr = new GrdSAM(8);
    /**
     * Eh, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM PublicCode = new GrdSAM(14);
    /**
     * 12h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM Version = new GrdSAM(18);
    /**
     * 13h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM LANRes = new GrdSAM(19);
    /**
     * 14h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM Type = new GrdSAM(20);
    /**
     * 16h, неизменяемое поле = new GrdSAM(значение прошивается на стадии производства)
     */
    public static final GrdSAM ID = new GrdSAM(22);
    /**
     * 1Ah, данное поле устанавливается только функцией GrdProtect после выполнения GrdInit
     */
    public static final GrdSAM WriteProtect = new GrdSAM(26);
    /**
     * 1Bh, данное поле устанавливается только функцией GrdProtect после выполнения GrdInit
     */
    public static final GrdSAM ReadProtect = new GrdSAM(27);
    /**
     * 1Ch, данное поле устанавливается только функцией GrdProtect после выполнения GrdInit
     */
    public static final GrdSAM NumFunc = new GrdSAM(28);
    /**
     * 1Dh, данное поле устанавливается только функцией GrdProtect после выполнения GrdInit
     */
    public static final GrdSAM TableLMS = new GrdSAM(29);
    /**
     * 1Eh, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM NProg = new GrdSAM(30);
    /**
     * 1Fh, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM Ver = new GrdSAM(31);
    /**
     * 20h, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM SN = new GrdSAM(32);
    /**
     * 22h, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM Mask = new GrdSAM(34);
    /**
     * 24h, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM GP = new GrdSAM(36);
    /**
     * 26h, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM RealLANRes = new GrdSAM(38);
    /**
     * 28h, данное поле может быть перезаписано функцией GrdWrite
     */
    public static final GrdSAM Index = new GrdSAM(40);
    /**
     * 2Ch
     */
    public static final GrdSAM AlgoAddr = new GrdSAM(44);

    int mValue;

    GrdSAM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
