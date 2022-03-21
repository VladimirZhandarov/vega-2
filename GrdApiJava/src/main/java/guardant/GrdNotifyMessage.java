package guardant;

/**
 * Флаги для функции <b>GrdLogin</b>
 */
public class GrdNotifyMessage {

    /// Обнаруженно подключение USB ключа
    public static final GrdNotifyMessage DongleArrived = new GrdNotifyMessage(0x00000000);
    /// Обнаруженно отключение USB ключа
    public static final GrdNotifyMessage DongleRemoved = new GrdNotifyMessage(0x00000001);
    /// Lost connection with the Guardant License Server
    public static final GrdNotifyMessage ConnectionLost = new GrdNotifyMessage(0x00000002);
    /// Restore connection with the Guardant License Server
    public static final GrdNotifyMessage ConnectionRestore = new GrdNotifyMessage(0x00000002);

    int mValue;

    public GrdNotifyMessage(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }

    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if ((obj == null) || (obj.getClass() != this.getClass())) {
            return false;
        }

        // тут мы уже точно знаем тип объекта
        GrdNotifyMessage m = (GrdNotifyMessage) obj;

        return mValue == m.mValue;
    }

}
