package guardant;

/**
 * Флаги, задающие режим поиска локальных или удаленных ключей
 */
public class GrdFMR {

    /**
     * Локальный ключ
     */
    public static final GrdFMR Local = new GrdFMR(0x0001);
    /**
     * Удаленный ключ
     */
    public static final GrdFMR Remote = new GrdFMR(0x0002);
    /**
     * Все ключи
     */
    public static final GrdFMR ALL = new GrdFMR(0x0003);

    int mValue;

    GrdFMR(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
