package guardant;

/* Идентификатор языка для функции GrdFormatMessage */
public class GrdLNG {

    /**
     * Английский язык
     */
    public static final GrdLNG English = new GrdLNG(0);
    /**
     * Русский язык
     */
    public static final GrdLNG Russian = new GrdLNG(7);

    int mValue;

    GrdLNG(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
