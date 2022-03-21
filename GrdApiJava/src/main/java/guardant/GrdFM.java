package guardant;

/**
 * Флаги, используемые в функции GrdSetFindMode, разрешающие использовать при поиске параметры, записанные в обязательных полях ключа
 */
public class GrdFM {

    /**
     * Учитывать при поиске "Номер программы"
     */
    public static final GrdFM NProg = new GrdFM(0x0001);
    /**
     * Учитывать при поиске "ID"
     */
    public static final GrdFM ID = new GrdFM(0x0002);
    /**
     * Учитывать при поиске "SN"
     */
    public static final GrdFM SN = new GrdFM(0x0004);
    /**
     * Учитывать при поиске "Версию"
     */
    public static final GrdFM Ver = new GrdFM(0x0008);
    /**
     * Учитывать при поиске "Маску"
     */
    public static final GrdFM Mask = new GrdFM(0x0010);
    /**
     * Учитывать при поиске "Тип"
     */
    public static final GrdFM Type = new GrdFM(0x0020);
    /**
     * Не учитывать при поиске никаких свойств.
     */
    public static final GrdFM ALL = new GrdFM(0x0000);

    int mValue;

    GrdFM(int value) {
        mValue = value;
    }

    public GrdFM(GrdFM[] flags) {
        mValue = 0;
        for (int i = 0; i < flags.length; ++i) {
            mValue = mValue | flags[i].mValue;
        }
    }

    public GrdFM(GrdFM f1, GrdFM f2) {
        mValue = f1.mValue | f2.mValue;
    }

    public GrdFM(GrdFM f1, GrdFM f2, GrdFM f3) {
        mValue = f1.mValue | f2.mValue | f3.mValue;
    }

    public GrdFM(GrdFM f1, GrdFM f2, GrdFM f3, GrdFM f4) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue;
    }

    public GrdFM(GrdFM f1, GrdFM f2, GrdFM f3, GrdFM f4, GrdFM f5) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue | f5.mValue;
    }

    public GrdFM(GrdFM f1, GrdFM f2, GrdFM f3, GrdFM f4, GrdFM f5, GrdFM f6) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue | f5.mValue | f6.mValue;
    }

    public int getValue() {
        return mValue;
    }

}
