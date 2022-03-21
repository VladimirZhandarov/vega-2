package guardant;

/* Режимы функционирования ключа GrdSetDriverMode() */
public class GrdDR {

    /**
     * Guardant driver
     */
    public static final GrdDR GRD_DRV = new GrdDR(0);
    /**
     * HID driver
     */
    public static final GrdDR USB_HID = new GrdDR(1);

    int mValue;

    GrdDR(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
