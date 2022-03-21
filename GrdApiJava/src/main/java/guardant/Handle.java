package guardant;

/**
 * Структура, определяющая хендл не указателем, а 64-разрядным целым числом; ее использование в функциях вместо IntPtr позволяет избежать unsafe-кода
 */
public class Handle {

    /**
     * Создает новый хендл, в качестве параметра принимается его адрес
     */
    public Handle(long handle) {
        GrdHandle = handle;
    }

    /**
     * Создает новый хендл, адрес задан как 0
     */
    public Handle() {
        GrdHandle = 0;
    }

    private long GrdHandle;

    /**
     * Указатель, представляющий собой хендл ключа
     */
    public long getAddress() {
        return GrdHandle;
    }

    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if ((obj == null) || (obj.getClass() != this.getClass())) {
            return false;
        }
        // тут мы уже точно знаем тип объекта
        Handle h = (Handle) obj;

        return GrdHandle == h.GrdHandle;
    }

    public int hashCode() {
        return (int) GrdHandle;
    }
}
