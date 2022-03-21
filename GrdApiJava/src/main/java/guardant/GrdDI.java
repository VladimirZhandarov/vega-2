package guardant;

/**
 * Интерфейсы ключей
 */
public class GrdDI {

    /**
     * LPT порт
     */
    public static final GrdDI LPT = new GrdDI(0);
    /**
     * USB порт
     */
    public static final GrdDI USB = new GrdDI(1);
    /**
     * Virtual bus
     */
    public static final GrdDI VIRTUAL = new GrdDI(2);

    int mValue;

    GrdDI(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
