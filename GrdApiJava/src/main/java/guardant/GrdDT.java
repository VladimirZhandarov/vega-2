package guardant;

/**
 * Типы ключей
 */
public class GrdDT {

    /**
     * Любой ключ
     */
    public static final GrdDT ALL = new GrdDT(0x0000);
    /**
     * Ключ поддерживает защиту приложений); созданных для работы в локальных сетях
     */
    public static final GrdDT LAN = new GrdDT(0x0001);
    /**
     * Ключ имеет возможность ограничивать время работы защищенного приложения
     */
    public static final GrdDT Time = new GrdDT(0x0002);
    /**
     * Ключ содержит алгоритм GSII64: ключи Guardant Stealth II / Net II); Stealth III / Net III
     */
    public static final GrdDT GSII64 = new GrdDT(0x0008);
    /**
     * Ключ поддерживает технологию защищенных ячеек Guardant Stealth III / Net III
     */
    public static final GrdDT PI = new GrdDT(0x0010);
    /**
     * Ключ поддерживает технологию защищенного удаленного программирования Trusted Remote Update Guardant Stealth III / Net III
     */
    public static final GrdDT TRU = new GrdDT(0x0020);
    /**
     * Support of Real Time Clock
     */
    public static final GrdDT RTC = new GrdDT(0x0040);
    /**
     * Support of AES 128 algorithm
     */
    public static final GrdDT AES = new GrdDT(0x0080);
    /**
     * Support of ECC 160 algorithm
     */
    public static final GrdDT ECC = new GrdDT(0x0100);
    /**
     * Support of Loadable Code algorithm
     */
    public static final GrdDT UserFirmware = new GrdDT(0x0400);

    int mValue;

    GrdDT(int value) {
        mValue = value;
    }

    public GrdDT(GrdDT[] flags) {
        mValue = 0;
        for (int i = 0; i < flags.length; i++) {
            mValue = mValue | flags[i].mValue;
        }
    }

    public GrdDT(GrdDT f1, GrdDT f2) {
        mValue = f1.mValue | f2.mValue;
    }

    public GrdDT(GrdDT f1, GrdDT f2, GrdDT f3) {
        mValue = f1.mValue | f2.mValue | f3.mValue;
    }

    public GrdDT(GrdDT f1, GrdDT f2, GrdDT f3, GrdDT f4) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue;
    }

    public GrdDT(GrdDT f1, GrdDT f2, GrdDT f3, GrdDT f4, GrdDT f5) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue | f5.mValue;
    }

    public int getValue() {
        return mValue;
    }
}
