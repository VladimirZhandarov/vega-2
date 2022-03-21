package guardant;

/**
 * Флаги, задающие режим поиска локальных или удаленных ключей
 */
public class GrdRCS {

    /**
     * Локальный ключ
     */
    public static final GrdRCS UserDefined = new GrdRCS(0);
    /**
     * Удаленный ключ
     */
    public static final GrdRCS ProgramData = new GrdRCS(0x80000001);
    /**
     * Все ключи
     */
    public static final GrdRCS EnvVar = new GrdRCS(0x80000002);

    int mValue;

    GrdRCS(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
