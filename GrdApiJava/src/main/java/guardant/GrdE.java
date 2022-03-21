package guardant;

/**
 * Стандартный набор ошибок
 */
public class GrdE {

    /**
     * Операция выполнена успешно
     */
    public static final GrdE OK = new GrdE(0);
    /**
     * Не найден ключ, отвечающий заданным условиям поиска
     */
    public static final GrdE DongleNotFound = new GrdE(1);
    /**
     * Указанный адрес слишком велик
     */
    public static final GrdE AddressTooBig = new GrdE(3);
    /**
     * Счетчик запусков GP исчерпан = new GrdE(значение равно нулю)
     */
    public static final GrdE GPis0 = new GrdE(5);
    /**
     * Неверная команда обращения к ключу
     */
    public static final GrdE InvalidCommand = new GrdE(6);
    /**
     * Ошибка верификации при записи в память ключа
     */
    public static final GrdE VerifyError = new GrdE(8);
    /**
     * Сетевой протокол не найден
     */
    public static final GrdE NetProtocolNotFound = new GrdE(9);
    /**
     * Сетевой ресурс ключа Guardant Net исчерпан
     */
    public static final GrdE NetResourceExhaust = new GrdE(10);
    /**
     * Потеряно соединение с сервером Guardant Net
     */
    public static final GrdE NetConnectionLost = new GrdE(11);
    /**
     * Сервер Guardant Net не найден
     */
    public static final GrdE NetDongleNotFound = new GrdE(12);
    /**
     * Ошибка распределения памяти сервера Guardant Net
     */
    public static final GrdE NetServerMemory = new GrdE(13);
    /**
     * Ошибка DPMI
     */
    public static final GrdE DPMI = new GrdE(14);
    /**
     * Внутренняя ошибка сервера Guardant Net
     */
    public static final GrdE Internal = new GrdE(15);
    /**
     * Сервер Guardant Net был перезагружен
     */
    public static final GrdE NetServerReloaded = new GrdE(16);
    /**
     * Данная команда не поддерживается данной версией ключа = new GrdE(ключ старой версии)
     */
    public static final GrdE VersionTooOld = new GrdE(17);
    /**
     * Необходим драйвер Windows NT
     */
    public static final GrdE BadDriver = new GrdE(18);
    /**
     * Ошибка сетевого протокола
     */
    public static final GrdE NetProtocol = new GrdE(19);
    /**
     * Получен сетевой пакет недопустимого формата
     */
    public static final GrdE NetPacket = new GrdE(20);
    /**
     * Необходима регистрация на сервере Guardant Net
     */
    public static final GrdE NeedLogin = new GrdE(21);
    /**
     * Необходимо снять регистрацию на сервере Guardant
     */
    public static final GrdE NeedLogout = new GrdE(22);
    /**
     * Ключ Guardant Net занят другим приложением
     */
    public static final GrdE DongleLocked = new GrdE(23);
    /**
     * Драйвер не может захватить порт
     */
    public static final GrdE DriverBusy = new GrdE(24);
    /**
     * Ошибка CRC при обращении к ключу
     */
    public static final GrdE CRCError = new GrdE(30);
    /**
     * Ошибка CRC при чтении данных из ключа
     */
    public static final GrdE CRCErrorRead = new GrdE(31);
    /**
     * Ошибка CRC при записи данных в ключ
     */
    public static final GrdE CRCErrorWrite = new GrdE(32);
    /**
     * Выход за границу памяти ключа
     */
    public static final GrdE Overbound = new GrdE(33);
    /**
     * Аппаратный алгоритм с таким номером в ключе не найден
     */
    public static final GrdE AlgoNotFound = new GrdE(34);
    /**
     * Ошибка CRC аппаратного алгоритма
     */
    public static final GrdE CRCErrorFunc = new GrdE(35);
    /**
     * Все ключи перебраны
     */
    public static final GrdE AllDonglesFound = new GrdE(36);
    /**
     * Слишком старая версия Guardant API
     */
    public static final GrdE ProtocolNotSup = new GrdE(37);
    /**
     * Задан несуществующий метод взаимообратного преобразования
     */
    public static final GrdE InvalidCnvType = new GrdE(38);
    /**
     * Неизвестная ошибка при работе с алгоритмом/ячейкой); операция могла не завершиться
     */
    public static final GrdE UnknownError = new GrdE(39);
    /**
     * Неверный пароль доступа к защищенной ячейке
     */
    public static final GrdE AccessDenied = new GrdE(40);
    /**
     * Статус защищенной ячейки изменить нельзя
     */
    public static final GrdE StatusUnchangeable = new GrdE(41);
    /**
     * Для алгоритма/ячейки сервис не предусмотрен
     */
    public static final GrdE NoService = new GrdE(42);
    /**
     * Алгоритм/ячейка находятся в состоянии Inactive); команда не выполнена
     */
    public static final GrdE InactiveItem = new GrdE(43);
    /**
     * Попытка выполнить операцию); которую не поддерживает текущая версия сервера Guardant Net
     */
    public static final GrdE DongleServerTooOld = new GrdE(44);
    /**
     * В данный момент ключ не может выполнять никаких операций
     */
    public static final GrdE DongleBusy = new GrdE(45);
    /**
     * Задано недопустимое значение одного из аргументов функции
     */
    public static final GrdE InvalidArg = new GrdE(46);
    /**
     * Ошибка распределения памяти
     */
    public static final GrdE MemoryAllocation = new GrdE(47);
    /**
     * Недопустимый хендл
     */
    public static final GrdE InvalidHandle = new GrdE(48);
    /**
     * Этот защищенный контейнер уже используется
     */
    public static final GrdE ContainerInUse = new GrdE(49);
    /**
     * Зарезервировано
     */
    public static final GrdE Reserved50 = new GrdE(50);
    /**
     * Зарезервировано
     */
    public static final GrdE Reserved51 = new GrdE(51);
    /**
     * Зарезервировано
     */
    public static final GrdE Reserved52 = new GrdE(52);
    /**
     * Нарушена целостность системных данных
     */
    public static final GrdE SystemDataCorrupted = new GrdE(53);
    /**
     * Вопрос для удаленного обновления не был сгенерирован
     */
    public static final GrdE NoQuestion = new GrdE(54);
    /**
     * Недопустимый формат данных для удаленного обновления
     */
    public static final GrdE InvalidData = new GrdE(55);
    /**
     * Вопрос для удаленного обновления уже сгенерирован
     */
    public static final GrdE QuestionOK = new GrdE(56);
    /**
     * Процедура записи при удаленном обновлении не завершена
     */
    public static final GrdE UpdateNotComplete = new GrdE(57);
    /**
     * Неверное значение хеша данных удаленного обновления
     */
    public static final GrdE InvalidHash = new GrdE(58);
    /**
     * Внутренняя ошибка
     */
    public static final GrdE GenInternal = new GrdE(59);
    /**
     * Данная копия Guardant API уже инициализирована
     */
    public static final GrdE AlreadyInitialized = new GrdE(60);
    /**
     * Real Time Clock error
     */
    public static final GrdE RTC_Error = new GrdE(61);
    /**
     * Real Time Clock battery low error
     */
    public static final GrdE BatteryError = new GrdE(62);
    /**
     * Duplicate items/algorithms names
     */
    public static final GrdE DuplicateNames = new GrdE(63);
    /**
     * Address in AAT table is out of range
     */
    public static final GrdE AATFormatError = new GrdE(64);
    /**
     * Session key not generated
     */
    public static final GrdE SessionKeyNtGen = new GrdE(65);
    /**
     * Invalid public key
     */
    public static final GrdE InvalidPublicKey = new GrdE(66);
    /**
     * Invalid digital sign
     */
    public static final GrdE InvalidDigitalSign = new GrdE(67);
    /**
     * Session key generation error
     */
    public static final GrdE SessionKeyGenError = new GrdE(68);
    /**
     * Invalid session key
     */
    public static final GrdE InvalidSessionKey = new GrdE(69);
    /**
     * Session key too old
     */
    public static final GrdE SessionKeyTooOld = new GrdE(70);
    /**
     * Initialization is required
     */
    public static final GrdE NeedInitialization = new GrdE(71);
    /**
     * Error while operating with functional of "Guardant Code"
     */
    public static final GrdE gcProhibitCode = new GrdE(72);
    /**
     * Error while operating with GrdCodeRun (Timeout)
     */
    public static final GrdE gcLoadableCodeTimeOut = new GrdE(73);
    /**
     * Flash-size from descriptor for "user firmware" too small
     */
    public static final GrdE gcFlashSizeFromDescriptorTooSmall = new GrdE(74);
    /**
     * Reserved
     */
    public static final GrdE Reserved1SFU = new GrdE(75);
    /**
     * Reserved
     */
    public static final GrdE Reserved2SFU = new GrdE(76);
    /**
     * Reserved
     */
    public static final GrdE Reserved3SFU = new GrdE(77);
    /**
     * Reserved
     */
    public static final GrdE Reserved4SFU = new GrdE(78);
    /**
     * Reserved
     */
    public static final GrdE Reserved5SFU = new GrdE(79);
    /**
     * TGrdLoadableCodeData structure exceeds space reserved for determinant in item descriptor
     */
    public static final GrdE gcIncorrectMask = new GrdE(80);
    /**
     * Incorrect RAM area specified in loadable code descriptor
     */
    public static final GrdE gcRamOverbound = new GrdE(81);
    /**
     * Incorrect FLASH memory area specified in loadable code descriptor
     */
    public static final GrdE gcFlashOverbound = new GrdE(82);
    /**
     * Allocation of intersecting FLASH memory areas for different loadable code modules
     */
    public static final GrdE gcIntersectionOfCodeAreas = new GrdE(83);
    /**
     * BMAP file is too long
     */
    public static final GrdE gcBmapFileTooBig = new GrdE(84);
    /**
     * The loadable code has zero length
     */
    public static final GrdE gcZeroLengthProgram = new GrdE(85);
    /**
     * Data verification failed
     */
    public static final GrdE gcIncorrectHash = new GrdE(86);
    /**
     * Error in Guardant Code protocol
     */
    public static final GrdE gcProtocolError = new GrdE(87);
    /**
     * Loadable code not found
     */
    public static final GrdE gcGCEXENotFound = new GrdE(88);
    /**
     * IO buffer size specified in loadable code is not enough for transmitting/receiving data
     */
    public static final GrdE gcNotEnoughRAM = new GrdE(89);
    /**
     * Security violation in Guardant Code virtual environment
     */
    public static final GrdE gcException = new GrdE(90);
    /**
     * IO buffer specified in loadable code exceeds the bounds of allowed RAM area
     */
    public static final GrdE gcRamOverboundInCodeLoad = new GrdE(91);
    /**
     * Loadable code exceeds the bounds of allowed FLASH memory area
     */
    public static final GrdE gcFlashOverboundInCodeLoad = new GrdE(92);
    /**
     * Allocation of intersecting RAM areas for different loadable code modules (Init operation is required)
     */
    public static final GrdE gcIntersectionOfCodeAreasInCodeLoad = new GrdE(93);
    /**
     * Incorrect GCEXE file format
     */
    public static final GrdE gcGCEXEFormatError = new GrdE(94);
    /**
     * Incorrect RAM area specified in loadable code for GcaCodeRun
     */
    public static final GrdE gcRamAccessViolation = new GrdE(95);
    /**
     * Too many nested calls of GcaCodeRun.
     */
    public static final GrdE gcCallDepthOverflow = new GrdE(96);

    public static final GrdE DLLNotFound = new GrdE(97);

    public static final GrdE LastError = new GrdE(98);

    int mValue;

    GrdE() {
        mValue = 0;
    }

    public GrdE(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }

    static GrdE fromValue(int _value) {
        if ((_value < 0) || (_value > LastError.mValue)) {
            throw new java.lang.IllegalArgumentException();
        }

        return new GrdE(_value);
    }

    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if ((obj == null) || (obj.getClass() != this.getClass())) {
            return false;
        }
        // тут мы уже точно знаем тип объекта
        GrdE h = (GrdE) obj;

        return mValue == h.mValue;
    }

}
