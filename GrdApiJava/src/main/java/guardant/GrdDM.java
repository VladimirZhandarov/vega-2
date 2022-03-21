package guardant;

/**
 * Модели ключей
 */
public class GrdDM {

    /**
     * Guardant Stealth LPT
     */
    public static final GrdDM GS1L = new GrdDM(0);
    /**
     * Guardant Stealth USB
     */
    public static final GrdDM GS1U = new GrdDM(1);
    /**
     * Guardant Fidus LPT
     */
    public static final GrdDM GF1L = new GrdDM(2);
    /**
     * Guardant Stealth II LPT
     */
    public static final GrdDM GS2L = new GrdDM(3);
    /**
     * Guardant Stealth II USB
     */
    public static final GrdDM GS2U = new GrdDM(4);
    /**
     * Guardant Stealth III USB
     */
    public static final GrdDM GS3U = new GrdDM(5);
    /**
     * Guardant Fidus USB
     */
    public static final GrdDM GF1U = new GrdDM(6);
    /**
     * Guardant Stealth Sign/Time USB
     */
    public static final GrdDM GS3SU = new GrdDM(7);
    /**
     * Guardant Stealth Code USB
     */
    public static final GrdDM GCU = new GrdDM(8);
    /**
     * Guardant SP SoftWare
     */
    public static final GrdDM GSP = new GrdDM(9);
    /**
     * Обшее количество моделей
     */
    public static final GrdDM Total = new GrdDM(10);

    /* Режимы функционирования ключа GrdSetDriverMode() */
    /**
     * Guardant driver
     */
    public static final GrdDM GRD_DRV = new GrdDM(0);
    /**
     * HID driver
     */
    public static final GrdDM USB_HID = new GrdDM(1);

    int mValue;

    public GrdDM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
