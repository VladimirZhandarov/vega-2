package guardant;

/**
 * Флаги, задающие режим создания контейнера для функции <b>GrdCreateHandle</b>
 */
public class GrdCHM {

    /**
     * Контейнер создается для работы в монопольном режиме
     */
    public static final GrdCHM SingleThread = new GrdCHM(0x00000000);
    /**
     * Контейнер может быть использован для одновременной работы с ним из нескольких потоков
     */
    public static final GrdCHM MultiThread = new GrdCHM(0x00000001);

    int mValue;

    public GrdCHM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
