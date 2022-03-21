package guardant;

/**
 * Флаги, задающие список возможных моделей ключа, участвующих в поиске
 */
public class GrdFMM {

    /**
     * Guardant Stealth LPT
     */
    public static final GrdFMM GS1L = new GrdFMM((1 << GrdDM.GS1L.mValue));
    /**
     * Guardant Stealth USB
     */
    public static final GrdFMM GS1U = new GrdFMM((1 << GrdDM.GS1U.mValue));
    /**
     * Guardant Fidus LPT
     */
    public static final GrdFMM GF1L = new GrdFMM((1 << GrdDM.GF1L.mValue));
    /**
     * Guardant Stealth II LPT
     */
    public static final GrdFMM GS2L = new GrdFMM((1 << GrdDM.GS2L.mValue));
    /**
     * Guardant Stealth II USB
     */
    public static final GrdFMM GS2U = new GrdFMM((1 << GrdDM.GS2U.mValue));
    /**
     * Guardant Stealth III USB
     */
    public static final GrdFMM GS3U = new GrdFMM((1 << GrdDM.GS3U.mValue));
    /**
     * Guardant Fidus USB
     */
    public static final GrdFMM GF1U = new GrdFMM((1 << GrdDM.GF1U.mValue));
    /**
     * Guardant Sign/Time USB
     */
    public static final GrdFMM GS3SU = new GrdFMM((1 << GrdDM.GS3SU.mValue));
    /**
     * Guardant Code USB
     */
    public static final GrdFMM GCU = new GrdFMM((1 << GrdDM.GCU.mValue));
    /**
     * Guardant SP Software
     */
    public static final GrdFMM GSP = new GrdFMM((1 << GrdDM.GSP.mValue));
    /**
     * Guardant Stealth I с любым интерфейсом
     */
    public static final GrdFMM GS1 = new GrdFMM(GS1L.mValue | GS1U.mValue);
    /**
     * Guardant Fidus с любым интерфейсом
     */
    public static final GrdFMM GF = new GrdFMM(GF1L.mValue | GF1U.mValue);
    /**
     * Guardant Stealth II с любым интерфейсом
     */
    public static final GrdFMM GS2 = new GrdFMM(GS2L.mValue | GS2U.mValue);
    /**
     * Guardant Stealth III с любым интерфейсом
     */
    public static final GrdFMM GS3 = new GrdFMM(GS3U.mValue);
    /**
     * Guardant Sign/Time of any interface
     */
    public static final GrdFMM GS3S = new GrdFMM(GS3SU.mValue);
    /**
     * Guardant Code
     */
    public static final GrdFMM GC = new GrdFMM(GCU.mValue);

    /**
     * Любой ключ из семейства Guardant Stealth или Fidus
     */
    public static final GrdFMM ALL = new GrdFMM(0);

    int mValue;

    GrdFMM(int value) {
        mValue = value;
    }

    public GrdFMM(GrdFMM[] flags) {
        mValue = 0;
        for (int i = 0; i < flags.length; ++i) {
            mValue = mValue | flags[i].mValue;
        }
    }

    public GrdFMM(GrdFMM f1, GrdFMM f2) {
        mValue = f1.mValue | f2.mValue;
    }

    public GrdFMM(GrdFMM f1, GrdFMM f2, GrdFMM f3) {
        mValue = f1.mValue | f2.mValue | f3.mValue;
    }

    public GrdFMM(GrdFMM f1, GrdFMM f2, GrdFMM f3, GrdFMM f4) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue;
    }

    public GrdFMM(GrdFMM f1, GrdFMM f2, GrdFMM f3, GrdFMM f4, GrdFMM f5) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue | f5.mValue;
    }

    public GrdFMM(GrdFMM f1, GrdFMM f2, GrdFMM f3, GrdFMM f4, GrdFMM f5, GrdFMM f6) {
        mValue = f1.mValue | f2.mValue | f3.mValue | f4.mValue | f5.mValue | f6.mValue;
    }

    public int getValue() {
        return mValue;
    }
}
