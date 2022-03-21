package guardant;

/**
 * Интерфейс прикладного программирования Guardant (Guardant Java API) – это набор функций, используемых прикладными программами на платформе Java при
 * выполнении операций с электронными ключами Guardant.
 * 
* Guardant Java API позволяет осуществить с ключами все действия, необходимые для создания системы защиты:<br><br>
 * 1. поиск и проверка наличия ключа с заданными параметрами<br>
 * 2. запись и считывание данных из памяти ключа<br>
 * 3. преобразование информации с помощью аппаратных алгоритмов<br>
 * 4. подсчет CRC<br>
 * 5. установка аппаратных запретов на чтение/запись памяти ключа и т. п. <br><br>
 *
 * Для организации работы приложения с электронным ключом через Guardant Java API нужно вставить вызовы функций API в исходные тексты программы, и загрузить
 * нужную библиотеку: для windows GrdApiJni32.dll/GrdApiJni64.dll для linux libGrdApiJni32.so/libGrdApiJni64.so
 * 
*/
public class Api {

    /**
     * Размер памяти, выделяемой под хендл ключа
     */
    final public static int StartCRC = 0xFFFFFFFF;
    final public int GrdContainerSize = 0x4000;

    /**
     * Текущее значение адреса ассоциированное с хэндлом; используется для организации потоковой записи/чтения
     */
    final public long GrdSeekCur = 0xFFFFFFFF;

    private Api() {
    }

    // API and handle initialization
    private static native int Startup(int remoteMode);

    /// <summary>
    /// Функция <b>GrdStartup</b> инициализирует данную копию "Guardant API".
    /// </summary>
    /// <param name="remoteMode"> комбинация флагов <see cref="GrdFMR"/>, задающих режим поиска локальных и/или удаленных ключей </param>
    /// <returns>Стандартный набор ошибок <seealso cref="GrdE"/> </returns>
    public static GrdE GrdStartup(GrdFMR remoteMode) {
        try {
            return new GrdE(Startup(remoteMode.mValue));
        } catch (UnsatisfiedLinkError e) {
            return GrdE.DLLNotFound;
        }
    }

    private static native int StartupEx(int remoteMode, String networkClientIniPath, int rcsMode);

    /// <summary>
    /// Функция <b>GrdStartupEx</b> инициализирует данную копию "Guardant API" и позволяет задать путь к файлу сетевых настроек клиента gnсlient.ini.
    /// </summary>
    /// <param name="remoteMode"> Комбинация флагов <see cref="GrdFMR"/>, задающих режим поиска локальных и/или удаленных ключей</param>
    /// <param name="networkClientIniPath"> Абсолютный путь, по которому должен располагаться gnclient.ini. Если параметр равен NULL, то приложение будет искать файл gnclient.ini в директории исполняемого файла, как при использовании GrdStartup.</param>
    /// <returns>Стандартный набор ошибок <seealso cref="GrdE"/> </returns>
    public static GrdE GrdStartupEx(GrdFMR remoteMode, String networkClientIniPath) {
        if (networkClientIniPath == null) {
            networkClientIniPath = "";
        }
        return new GrdE(StartupEx(remoteMode.mValue, networkClientIniPath, 0));
    }

    /// <summary>
    /// Функция <b>GrdStartupEx</b> инициализирует данную копию "Guardant API" и позволяет задать путь к файлу сетевых настроек клиента gnсlient.ini.
    /// </summary>
    /// <param name="remoteMode"> Комбинация флагов <see cref="GrdFMR"/>, задающих режим поиска локальных и/или удаленных ключей</param>
    /// <param name="networkClientIniPath"> Абсолютный путь, по которому должен располагаться gnclient.ini. Если параметр равен NULL, то приложение будет искать файл gnclient.ini в директории исполняемого файла, как при использовании GrdStartup.</param>
    /// <returns>Стандартный набор ошибок <seealso cref="GrdE"/> </returns>
    public static GrdE GrdStartupEx(GrdFMR remoteMode, String networkClientIniPath, GrdRCS rcsMode) {
        if (networkClientIniPath == null) {
            networkClientIniPath = "";
        }
        return new GrdE(StartupEx(remoteMode.mValue, networkClientIniPath, rcsMode.mValue));
    }

    private static native int Cleanup();

    /// <summary>
    /// Функция <b>GrdCleanup</b> деинициализирует данную копию Guardant API.<br/>
    /// Функция <b>GrdCleanup</b> должна быть вызвана однократно перед закрытием приложения, или когда более не планируется вызывать Guardant API.
    /// </summary>
    /// <returns></returns>
    public static GrdE GrdCleanup() {
        return new GrdE(Cleanup());
    }

    private static native long CreateHandle(long hGrd, int mode);

    /// <summary>
    /// Функция <b>GrdCreateHandle</b> создает защищенный контейнер.
    /// </summary>
    /// <param name="Mode"></param>
    /// <returns></returns>
    public static Handle GrdCreateHandle(GrdCHM Mode) {
        return new Handle(CreateHandle(0, Mode.mValue));
    }

    /// <summary>
    /// Функция <b>GrdCloseHandle</b> уничтожает хэндл.
    /// </summary>
    /// <param name="grdHandle"></param>
    /// <returns></returns>
    private static native int CloseHandle(long hGrd);

    public static GrdE GrdCloseHandle(Handle grdHandle) {
        return new GrdE(CloseHandle(grdHandle.getAddress()));
    }

    /**
     * Функция <b>GrdSetAccessCodes</b> сохраняет коды доступа к ключу в защищенном контейнере.
     */
    private static native int SetAccessCodes(long hGrd, int publicCode, int privateRD, int privateWR, int privateMST);

    public static GrdE GrdSetAccessCodes(Handle grdHandle, int publicCode) {
        return new GrdE(SetAccessCodes(grdHandle.getAddress(), publicCode, 0, 0, 0));
    }

    public static GrdE GrdSetAccessCodes(Handle grdHandle, int publicCode, int privateRD) {
        return new GrdE(SetAccessCodes(grdHandle.getAddress(), publicCode, privateRD, 0, 0));
    }

    public static GrdE GrdSetAccessCodes(Handle grdHandle, int publicCode, int privateRD, int privateWR) {
        return new GrdE(SetAccessCodes(grdHandle.getAddress(), publicCode, privateRD, privateWR, 0));
    }

    public static GrdE GrdSetAccessCodes(Handle grdHandle, int publicCode, int privateRD, int privateWR, int privateMST) {
        return new GrdE(SetAccessCodes(grdHandle.getAddress(), publicCode, privateRD, privateWR, privateMST));
    }

    /**
     *
     * Функция <b>GrdLogin</b> позволяет копии защищенного приложения получить лицензию для дальнейшей работы на сервере Guardant Net или на локальном ключе
     * Guardant.
     */
    private static native int Login(long hGrd, int moduleLMS, int loginFlags);

    public static GrdE GrdLogin(Handle grdHandle, GrdLM loginFlags) {
        return new GrdE(Login(grdHandle.getAddress(), -1, loginFlags.mValue));
    }

    public static GrdE GrdLogin(Handle grdHandle, int moduleLMS, GrdLM loginFlags) {
        return new GrdE(Login(grdHandle.getAddress(), moduleLMS, loginFlags.mValue));
    }

    /**
     *
     * Функция <b>GrdLogout</b> закрывает сеанс работы с локальным или удаленным ключом.
     */
    private static native int Logout(long hGrd, int moduleLms);

    public static GrdE GrdLogout(Handle grdHandle) {
        return new GrdE(Logout(grdHandle.getAddress(), 0));
    }

    public static GrdE GrdLogout(Handle grdHandle, int moduleLms) {
        return new GrdE(Logout(grdHandle.getAddress(), moduleLms));
    }

    /**
     * Функция <b>GrdFormatMessage</b> преобразует код ошибки в текстовую строку, соответствующую данному коду.
     */
    private static native int FormatMessage(long hGrd, int errorCode, int language, java.lang.String[] outErrorMsg);

    public static GrdE GrdFormatMessage(GrdE errorCode, GrdLNG language, java.lang.String[] outErrorMsg) {
        // Translate error code to text message
        return new GrdE(FormatMessage(0L, errorCode.mValue, language.mValue, outErrorMsg));
    }

    public static GrdE GrdFormatMessage(Handle grdHandle, GrdE errorCode, GrdLNG language, java.lang.String[] outErrorMsg) {
        // Translate error code to text message
        return new GrdE(FormatMessage(grdHandle.getAddress(), errorCode.mValue, language.mValue, outErrorMsg));
    }

    /**
     * Функция <b>GrdSetFindMode</b> устанавливает условия поиска ключей для функций <b>GrdFind</b> и <b>GrdLogin</b>, а также режимы работы для хэндла.
     */
    private static native int SetFindMode(long hGrd, int remoteMode, int flags,
        byte prog, int id, short sn, byte ver, short mask, int type, int models, int interfaces);

    public static GrdE GrdSetFindMode(Handle grdHandle, GrdFMR remoteMode, GrdFM flags,
        byte prog, int id, short sn, byte ver, short mask, GrdDT type, GrdFMM models, GrdFMI interfaces) {
        return new GrdE(SetFindMode(grdHandle.getAddress(), remoteMode.mValue, flags.mValue,
            prog, id, sn, ver, mask, type.mValue, models.mValue, interfaces.mValue));
    }

    /**
     * Функция <b>GrdFind</b> осуществляет поиск локальных и удаленных ключей, удовлетворяющим установленным критериям поиска.
     */
    private static native int Find(long hGrd, int mode, GrdDongleID id, GrdFindInfo findInfo);

    public static GrdE GrdFind(Handle grdHandle, GrdF mode, GrdDongleID id, GrdFindInfo findInfo) {
        return new GrdE(Find(grdHandle.getAddress(), mode.mValue, id, findInfo));
    }

    /**
     * Функция <b>GrdLock</b> блокирует выполнение операций с ключом, давая возможность работать с ключом в монопольном режиме.
     */
    private static native int Lock(long hGrd, int timeoutAutoUnlock, int timeoutWaitForUnlock, int mode);

    public static GrdE GrdLock(Handle grdHandle, int timeoutAutoUnlock, int timeoutWaitForUnlock, GrdLockMode lockMode) {
        return new GrdE(Lock(grdHandle.getAddress(), timeoutAutoUnlock, timeoutWaitForUnlock, lockMode.mValue));
    }

    /**
     *
     * Функция <b>GrdUnlock</b> разблокирует ключ, ранее заблокированный функцией <b>GrdLock</b>.
     */
    private static native int Unlock(long hGrd);

    public static GrdE GrdUnlock(Handle grdHandle) {
        return new GrdE(Unlock(grdHandle.getAddress()));
    }

    /**
     * Функция <b>GrdGetLastError</b> получает код последней ошибки для указанного хэндла.
     */
    private static native int GetLastError(long hGrd);

    public static GrdE GrdGetLastError(Handle grdHandle) {
        return new GrdE(GetLastError(grdHandle.getAddress()));
    }

    /**
     * Функция <b>GrdIsValidHandle</b> проверяет целостность защищенного контейнера.
     */
    private static native boolean IsValidHandle(long hGrd);

    public static boolean GrdIsValidHandle(Handle grdHandle) {
        return IsValidHandle(grdHandle.getAddress());
    }

    /**
     *
     * Функция <b>GrdCheck</b> производит поиск и проверку доступности ключа.
     */
    private static native int Check(long hGrd);

    public static GrdE GrdCheck(Handle grdHandle) {
        return new GrdE(Check(grdHandle.getAddress()));
    }

    /**
     *
     * Функция <b>GrdDecGP</b> производит поиск и проверку доступности ключа, а также вычитание счетчика запусков/времени GP.
     */
    private static native int DecGP(long hGrd);

    public static GrdE GrdDecGP(Handle grdHandle) {
        return new GrdE(DecGP(grdHandle.getAddress()));
    }

    /**
     *
     * Функция <b>GrdSetWorkMode</b> устанавливает режим работы для хэндла.
     */
    private static native int SetWorkMode(long hGrd, int flagsWork, int flagsMode);

    public static GrdE GrdSetWorkMode(Handle grdHandle, GrdWM flagsWork) {
        return new GrdE(SetWorkMode(grdHandle.getAddress(), flagsWork.mValue, 0));
    }

    /**
     *
     * Функция <b>GrdSeek</b> устанавливает текущий указатель памяти ключа на заданную позицию.
     */
    private static native int Seek(long hGrd, int addr);

    public static GrdE GrdSeek(Handle grdHandle, int addr) {
        return new GrdE(Seek(grdHandle.getAddress(), addr));
    }

    /**
     * Функция <b>GrdWrite</b> производит запись данных в память ключа	по адресу задаваемому <seealso cref="GrdUAM"/> или <seealso cref="GrdSAM".
     */
    private static native int Write(long hGrd, int addr, byte[] Data);

    private static native int Write(long hGrd, int addr, short[] Data);

    private static native int Write(long hGrd, int addr, int[] Data);

    private static native int Write(long hGrd, int addr, long[] Data);

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, byte[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, byte[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, short[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, short[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, int[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, int[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, long[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, long[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Write(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, byte data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        byte[] buf = new byte[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), samAddr.mValue, buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, byte data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        byte[] buf = new byte[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), uamAddr.mValue, buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, short data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        short[] buf = new short[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), samAddr.mValue, buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, short data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        short[] buf = new short[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), uamAddr.mValue, buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, int data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        int[] buf = new int[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), samAddr.getValue(), buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, int data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        int[] buf = new int[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), uamAddr.getValue(), buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdSAM samAddr, long data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        long[] buf = new long[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), samAddr.getValue(), buf));
    }

    public static GrdE GrdWrite(Handle grdHandle, GrdUAM uamAddr, long data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        long[] buf = new long[1];
        buf[0] = data;
        return new GrdE(Write(grdHandle.getAddress(), uamAddr.getValue(), buf));
    }

    /**
     *
     * Функция <b>GrdRead</b> производит считывание данных из ключа задаваемому задаваемому <seealso cref="GrdUAM"/> или <seealso cref="GrdSAM".
     */
    private static native int Read(long hGrd, int addr, byte[] data);

    private static native int Read(long hGrd, int addr, short[] data);

    private static native int Read(long hGrd, int addr, int[] data);

    private static native int Read(long hGrd, int addr, long[] data);

    public static GrdE GrdRead(Handle grdHandle, GrdSAM samAddr, byte[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdUAM uamAddr, byte[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE((Read(grdHandle.getAddress(), uamAddr.mValue, data)));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdSAM samAddr, short[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdUAM uamAddr, short[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdSAM samAddr, int[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdUAM uamAddr, int[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdSAM samAddr, long[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.SAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), samAddr.mValue, data));
    }

    public static GrdE GrdRead(Handle grdHandle, GrdUAM uamAddr, long[] data) {
        GrdE nGrdE = GrdSetWorkMode(grdHandle, GrdWM.UAM);
        if (nGrdE != GrdE.OK) {
            return nGrdE;
        }

        return new GrdE(Read(grdHandle.getAddress(), uamAddr.mValue, data));
    }

    /**
     *
     * Функция <b>GrdGetInfo</b> получает регистрационную информацию о текущем ключе, устанавливаемую функцией <b>GrdLogin</b>
     */
    private static native int GetInfo(long hGrd, int infoCode, byte[] InfoData);

    private static native int GetInfo(long hGrd, int infoCode, short[] InfoData);

    private static native int GetInfo(long hGrd, int infoCode, int[] InfoData);

    private static native int GetInfo(long hGrd, int infoCode, long[] InfoData);

    public static GrdE GrdGetInfo(Handle grdHandle, GrdInfo infoCode, byte[] infoData) {
        return new GrdE(GetInfo(grdHandle.getAddress(), infoCode.mValue, infoData));
    }

    public static GrdE GrdGetInfo(Handle grdHandle, GrdInfo infoCode, short[] infoData) {
        return new GrdE(GetInfo(grdHandle.getAddress(), infoCode.mValue, infoData));
    }

    public static GrdE GrdGetInfo(Handle grdHandle, GrdInfo infoCode, int[] infoData) {
        return new GrdE(GetInfo(grdHandle.getAddress(), infoCode.mValue, infoData));
    }

    public static GrdE GrdGetInfo(Handle grdHandle, GrdInfo infoCode, long[] infoData) {
        return new GrdE(GetInfo(grdHandle.getAddress(), infoCode.mValue, infoData));
    }

    /**
     * Функция <b>GrdCRC</b> позволяет подсчитать 32-битный CRC целого или массива. Данная функция удобна для экспресс-анализа и контроля целостности кода или
     * данных.
     */
    private static native int CRC(byte[] data, int prevCRC);

    private static native int CRC(short[] data, int prevCRC);

    private static native int CRC(int[] data, int prevCRC);

    private static native int CRC(long[] data, int prevCRC);

    public static int GrdCRC(byte[] data) {
        return CRC(data, StartCRC);
    }

    public static int GrdCRC(byte[] data, int prevCRC) {
        return CRC(data, prevCRC);
    }

    public static int GrdCRC(short[] data) {
        return CRC(data, StartCRC);
    }

    public static int GrdCRC(short[] data, int prevCRC) {
        return CRC(data, prevCRC);
    }

    public static int GrdCRC(int[] data) {
        return CRC(data, StartCRC);
    }

    public static int GrdCRC(int[] data, int prevCRC) {
        return CRC(data, prevCRC);
    }

    public static int GrdCRC(long[] data) {
        return CRC(data, StartCRC);
    }

    public static int GrdCRC(long[] data, int prevCRC) {
        return CRC(data, prevCRC);
    }

    /**
     *
     * Функция <b>GrdInit</b> производит инициализацию памяти ключа.
     */
    private static native int Init(long hGrd);

    public static GrdE GrdInit(Handle grdHandle) {
        return new GrdE(Init(grdHandle.getAddress()));
    }

    /**
     *
     * Функция <b>GrdProtect</b> устанавливает аппаратные запреты на чтение и запись, а также количество аппаратных алгоритмов и адрес таблицы лицензий LMS.
     */
    private static native int Protect(long hGrd, int wrProt, int rdProt, int numFunc, int tableLMS, int globalFlags);

    public static GrdE GrdProtect(Handle grdHandle, int writeProt, int readProt, int numFunc, int tableLMS) {
        return new GrdE(Protect(grdHandle.getAddress(), writeProt, readProt, numFunc, tableLMS, 0));
    }

    public static GrdE GrdProtect(Handle grdHandle, int writeProt, int readProt, int numFunc, int tableLMS, GrdGF globalFlags) {
        return new GrdE(Protect(grdHandle.getAddress(), writeProt, readProt, numFunc, tableLMS, globalFlags.mValue));
    }

    /**
     * <summary>
     * Функция <b>GrdCryptEx</b>
     * Использовать только для аппартных алгоритмов GSII64/AES128 или програмного алгоритма AES256 Размер вектора инициализации для GSII64 8 байт Размер вектора
     * инициализации для AES128/AES256 16 байт
     * </summary>
     */
    private static native int CryptEx(long hGrd, int algoNum, byte[] data, int method, byte[] iv, byte[] key, byte[] context);

    public static GrdE GrdCryptEx(Handle grdHandle, int algoNum, byte[] data, int method, byte[] iv) {
        return new GrdE(CryptEx(grdHandle.getAddress(), algoNum, data, method, iv, null, null));
    }

    public static GrdE GrdCryptEx(Handle grdHandle, int algoNum, byte[] data, int method, byte[] iv, byte[] key) {
        return new GrdE(CryptEx(grdHandle.getAddress(), algoNum, data, method, iv, key, null));
    }

    public static GrdE GrdCryptEx(Handle grdHandle, int algoNum, byte[] data, int method, byte[] iv, byte[] key, byte[] context) {
        return new GrdE(CryptEx(grdHandle.getAddress(), algoNum, data, method, iv, key, context));
    }

    /**
     * <summary>
     * Функция <b>GrdTransformEx</b>
     * Использовать только для аппартных алгоритмов GSII64/AES128 Размер вектора инициализации для GSII64 8 байт Размер вектора инициализации для AES128 16 байт
     * </summary>
     */
    private static native int TransformEx(long hGrd, int algoNum, byte[] data, int method, byte[] iv);

    public static GrdE GrdTransformEx(Handle grdHandle, int algoNum, byte[] data, GrdAM method, byte[] iv) {
        return new GrdE(TransformEx(grdHandle.getAddress(), algoNum, data, method.mValue, iv));
    }

    /**
     * Функция <b>GrdSign</b> вычисляет электронную цифровую подпись блока байт при помощи аппаратного алгоритма ECC160.
     */
    private static native int Sign(long hGrd, int algoNum, byte[] data, byte[] sign);

    public static GrdE GrdSign(Handle grdHandle, int algNum, byte[] data, byte[] sign) {
        return new GrdE(Sign(grdHandle.getAddress(), algNum, data, sign));
    }

    /**
     * Функция GrdVerifySign проверяет электронную цифровую подпись блока байт при помощи аппаратного алгоритма ECC160.
     */
    private static native int VerifySign(long hGrdHandle, byte[] publicKey, byte[] data, byte[] signResult);

    public static GrdE GrdVerifySign(Handle grdHandle, byte[] publicKey, byte[] data, byte[] sign) {
        return new GrdE(VerifySign(grdHandle.getAddress(), publicKey, data, sign));
    }

    /**
     * Функция <b>GrdHashEx</b>
     */
    private static native int HashEx(long hGrd, int hash, byte[] data, int method, byte[] digest, byte[] context);

    public static GrdE GrdHashEx(Handle grdHandle, int hash, byte[] data, GrdSC method, byte[] digest) {
        return new GrdE(HashEx(grdHandle.getAddress(), hash, data, method.mValue, digest, null));
    }

    public static GrdE GrdHashEx(Handle grdHandle, int hash, byte[] data, GrdSC method, byte[] digest, byte[] context) {
        return new GrdE(HashEx(grdHandle.getAddress(), hash, data, method.mValue, digest, context));
    }

    /// \defgroup GRD_ALG_OLG Устаревшие функции для работы с алгоритмами
    /// \{
    /**
     *
     * Функция <b>GrdCrypt</b> зашифровывает или расшифровывает блок данных при помощи аппаратного или программно-реализованного алгоритма.
     */
    private static native int Crypt(long hGrd, int algoNum, byte[] data, int Method, byte[] iv, byte[] key, byte[] context);

    /**
     * Функция <b>GrdCrypt</b> зашифровывает или расшифровывает блок данных при помощи аппаратного или программно-реализованного алгоритма. Использовать только
     * для аппартного алгоритма GSII64 Размер вектора инициализации для GSII64 8 байт
     */
    public static GrdE GrdCrypt(Handle grdHandle, int algoNum, byte[] data, int method, byte[] iv) {
        return new GrdE(Crypt(grdHandle.getAddress(), algoNum, data, method, iv, null, null));
    }

    /**
     * Функция <b>GrdCrypt</b> зашифровывает или расшифровывает блок данных при помощи аппаратного или программно-реализованного алгоритма. Использовать только
     * для аппартного алгоритма GSII64 или програмного алгоритма AES256 Размер вектора инициализации для GSII64 8 байт Размер вектора инициализации для AES256
     * 16 байт
     */
    public static GrdE GrdCrypt(Handle grdHandle, int algoNum, byte[] data, int method, byte[] iv, byte[] key) {
        return new GrdE(Crypt(grdHandle.getAddress(), algoNum, data, method, iv, key, null));
    }

    /**
     * Функция <b>GrdCrypt</b> зашифровывает или расшифровывает блок данных при помощи аппаратного или программно-реализованного алгоритма. Использовать только
     * для аппартного алгоритма GSII64 или програмного алгоритма AES256 Размер вектора инициализации для GSII64 8 байт Размер вектора инициализации для AES256
     * 16 байт
     */
    public static GrdE GrdCrypt(Handle grdHandle, int algoNum, byte[] data, int method, byte[] iv, byte[] key, byte[] context) {
        return new GrdE(Crypt(grdHandle.getAddress(), algoNum, data, method, iv, key, context));
    }

    public static GrdE GrdCrypt(Handle grdHandle, int algoNum, byte[] data, int method, long iv) {
        byte[] ba = new byte[8];
        ba[0] = (byte) (iv >> (64 - 8 * 1));
        ba[1] = (byte) (iv >> (64 - 8 * 2));
        ba[2] = (byte) (iv >> (64 - 8 * 3));
        ba[3] = (byte) (iv >> (64 - 8 * 4));
        ba[4] = (byte) (iv >> (64 - 8 * 5));
        ba[5] = (byte) (iv >> (64 - 8 * 6));
        ba[6] = (byte) (iv >> (64 - 8 * 7));
        ba[7] = (byte) (iv >> (64 - 8 * 8));
        return new GrdE(Crypt(grdHandle.getAddress(), algoNum, data, method, ba, null, null));
    }

    private static native int Transform(long hGrd, int algoNum, byte[] data, int method, byte[] iv);

    /**
     * Функция <b>GrdTransform</b>
     * Использовать только для аппартного алгоритма GSII64
     */
    public static GrdE GrdTransform(Handle grdHandle, int algoNum, byte[] data, GrdAM method, byte[] iv) {
        return new GrdE(Transform(grdHandle.getAddress(), algoNum, data, method.mValue, iv));
    }

    /**
     * Функция <b>GrdHash</b> вычисляет хэш-функцию блока данных.
     */
    private static native int Hash(long hGrd, int hash, byte[] data, int method, byte[] digest, byte[] context);

    // Overloads of GrdHash()
    public static GrdE GrdHash(Handle grdHandle, int hash, byte[] data, GrdSC Method, byte[] digest, byte[] context) {
        return new GrdE(Hash(grdHandle.getAddress(), hash, data, Method.mValue, digest, context));
    }

    public static GrdE GrdHash(Handle grdHandle, int hash, byte[] data, GrdSC Method, byte[] digest) {
        return new GrdE(Hash(grdHandle.getAddress(), hash, data, Method.mValue, digest, null));
    }

    private static native int CodeInit(long hGrd, int cnvType, int addr, byte[] key);

    public static GrdE GrdCodeInit(Handle grdHandle, int cnvType, int addr, byte[] key) {
        return new GrdE(CodeInit(grdHandle.getAddress(), cnvType, addr, key));
    }

    private static native int EnCode(int cnvType, byte[] key, byte[] data);

    public static GrdE GrdEnCode(int cnvType, byte[] key, byte[] data) {
        return new GrdE(EnCode(cnvType, key, data));
    }

    private static native int DeCode(int cnvType, byte[] key, byte[] data);

    public static GrdE GrdDeCode(int cnvType, byte[] key, byte[] data) {
        return new GrdE(DeCode(cnvType, key, data));
    }

	/// \}
    /// \defgroup GRD_PI Функции работы с защищенными ячейками
    /// \{
    /// Ключи <b>Guardant Time/Time Net/Code Time</b> обладают встроенными часами реального времени (Real-Time Clock, RTC), что позволяет ограничивать астрономическое время работы приложения путем установки временных зависимостей от аппаратных алгоритмов ключа. Для работы с возможностями, которые присущи только ключам с RTC, существуют специальные функции Guardant API.
    /**
     * Функция <b>GrdPI_Activate</b> активирует аппаратный алгоритм или защищенную ячейку.
     */
    private static native int PI_Activate(long hGrd, int itemNum, int activatePsw);

    public static GrdE GrdPI_Activate(Handle grdHandle, int itemNum, int activatePsw) {
        return new GrdE(PI_Activate(grdHandle.getAddress(), itemNum, activatePsw));
    }

    /**
     * Функция <b>GrdPI_Deactivate</b> деактивирует аппаратный алгоритм или защищенную ячейку.
     */
    private static native int PI_Deactivate(long hGrd, int itemNum, int deactivatePsw);

    public static GrdE GrdPI_Deactivate(Handle grdHandle, int itemNum, int deactivatePsw) {
        return new GrdE(PI_Deactivate(grdHandle.getAddress(), itemNum, deactivatePsw));
    }

    /**
     * Функция <b>GrdPI_Read</b> позволяет считывать данные защищенной ячейки или определителя алгоритма.
     */
    private static native int PI_Read(long hGrd, int itemNum, int addr, byte[] data, int readPsw);

    public static GrdE GrdPI_Read(Handle grdHandle, int itemNum, int addr, byte[] data) {
        return new GrdE(PI_Read(grdHandle.getAddress(), itemNum, addr, data, 0));
    }

    public static GrdE GrdPI_Read(Handle grdHandle, int itemNum, int addr, byte[] data, int readPsw) {
        return new GrdE(PI_Read(grdHandle.getAddress(), itemNum, addr, data, readPsw));
    }

    /**
     * Функция <b>GrdPI_Update</b> позволяет перезаписывать данные защищенной ячейки или определитель алгоритма. При этом определитель алгоритма можно
     * перезаписать только целиком, а в защищенной ячейке можно перезаписывать любой участок данных.
     */
    private static native int PI_Update(long hGrd, int itemNum, int addr, byte[] data, int updatePsw, int method);

    public static GrdE GrdPI_Update(Handle grdHandle, int itemNum, int addr, byte[] data, int updatePsw, GrdUM method) {
        return new GrdE(PI_Update(grdHandle.getAddress(), itemNum, addr, data, updatePsw, method.mValue));
    }

    /**
     * Функция <b>GrdPI_GetCounter</b> возвращает значение счетчика запусков алгоритма или защищённой ячейки.
     */
    private static native int PI_GetCounter(long hGrd, int algoNum, int[] counter);

    public static GrdE GrdPI_GetCounter(Handle grdHandle, int algoNum, int[] counter) {
        return new GrdE(PI_GetCounter(grdHandle.getAddress(), algoNum, counter));
    }

	/// \}  Функции работы с защищенными ячейками
    /// \defgroup GRD_TRU Функции доверенного удаленного обновления 
    /// \{
    /// Технология Trusted Remote Update может быть реализована не только при помощи утилит, входящих в комплект разработчика. 
    /// При желании разработчики могут встраивать поддержку этой технологии непосредственно в свои приложения, используя набор предназначенных для этой цели функций.
    private static native int TRU_SetKey(long hGrd, byte[] gsii64_Key128);

    /**
     * Функция <b>GrdTRU_SetKey</b>
     *
     * @param grdHandle
     * @param gsii64_Key128 указатель на буфер, содержащий 128-битовый уникальный секретный ключ шифрования алгоритма GSII64/AES128
     * @return
     */
    public static GrdE GrdTRU_SetKey(Handle grdHandle, byte[] gsii64_Key128) {
        return new GrdE(TRU_SetKey(grdHandle.getAddress(), gsii64_Key128));
    }

    private static native int TRU_GenerateQuestionEx(long hGrd, byte[] question, int[] id, byte[] publicCode, byte[] hash);

    public static GrdE GrdTRU_GenerateQuestionEx(Handle grdHandle, byte[] question, int[] id, byte[] publicCode, byte[] hash) {
        return new GrdE(TRU_GenerateQuestion(grdHandle.getAddress(), question, id, publicCode, hash));
    }

    private static native int TRU_DecryptQuestionEx(long hGrd, int algoNum_Decrypt, int algoNum_Hash,
        byte[] question, int id, int publicCode, byte[] hash, int mode);

    public static GrdE GrdTRU_DecryptQuestionEx(Handle grdHandle, int algoNum_Decrypt, int algoNum_Hash,
        byte[] question, int id, int publicCode, byte[] hash, GrdTRU truMode) {
        return new GrdE(TRU_DecryptQuestionEx(grdHandle.getAddress(), algoNum_Decrypt, algoNum_Hash,
            question, id, publicCode, hash, truMode.getValue()));
    }

    ;

	private static native int TRU_EncryptAnswerEx(long hGd, int addr, byte[] data, byte[] question, int algoNum_Encrypt,
        int algoNum_Hash, byte[] answer, int[] answerSize, int mode);

    public static GrdE GrdTRU_EncryptAnswerEx(Handle grdHandle, int addr, byte[] data, byte[] question, int algoNum_Encrypt,
        int algoNum_Hash, byte[] answer, int[] answerSize, GrdTRU truMode) {
        return new GrdE(TRU_EncryptAnswerEx(grdHandle.getAddress(), addr, data, question, algoNum_Encrypt,
            algoNum_Hash, answer, answerSize, truMode.getValue()));
    }

    private static native int TRU_SetAnswerProperties(long hGrd, int truFlags, int wrProt, int rdProt,
        int numFunc, int tableLMS, int globalFlags);

    public static GrdE GrdTRU_SetAnswerProperties(Handle grdHandle, GrdTRU truFlags, int wrProt, int rdProt,
        int numFunc, int tableLMS, GrdGF globalFlags) {
        return new GrdE(TRU_SetAnswerProperties(grdHandle.getAddress(), truFlags.mValue, wrProt, rdProt,
            numFunc, tableLMS, globalFlags.mValue));
    }

    private static native int TRU_ApplyAnswer(long hGrd, byte[] answer);

    public static GrdE GrdTRU_ApplyAnswer(Handle grdHandle, byte[] answer) {
        return new GrdE(TRU_ApplyAnswer(grdHandle.getAddress(), answer));
    }

    /// \defgroup GRD_TRU_TIME Функции доверенного удаленного обновления для ключей Time
    /// \{
    /// Функции, реализующие технологию <b>Trusted Remote Update</b> для ключей с часами реального времени (Guardant Time, Guardant Time Net, Guardant Code Time) имеют некоторые отличия от своих аналогов для обычных ключей, и поэтому выделяются в особую подгруппу
    private static native int TRU_GenerateQuestionTimeEx(long hGrd, byte[] question, int[] id,
        int[] publicCode, long[] dongleTime, long[] deadTimes, long[] deadTimesNumbers, byte[] hash);

    public static GrdE GrdTRU_GenerateQuestionTimeEx(Handle grdHandle, byte[] question, int[] id,
        int[] publicCode, long[] dongleTime, long[] deadTimes, long[] deadTimesNumbers, byte[] hash) {
        return new GrdE(TRU_GenerateQuestionTimeEx(grdHandle.getAddress(), question, id,
            publicCode, dongleTime, deadTimes, deadTimesNumbers, hash));
    }

    private static native int TRU_DecryptQuestionTimeEx(long hGrd, int algoNum_Decrypt, int algoNum_Hash,
        byte[] question, int id, int publicCode, long[] dongleTime, long[] deadTimes, int deadTimesNumbers, byte[] hash, int mode);

    public static GrdE GrdTRU_DecryptQuestionTimeEx(Handle grdHandle, int algoNum_Decrypt, int algoNum_Hash,
        byte[] question, int id, int publicCode, long[] dongleTime, long[] deadTimes, int deadTimesNumbers, byte[] hash, int mode) {
        return new GrdE(TRU_DecryptQuestionTimeEx(grdHandle.getAddress(), algoNum_Decrypt, algoNum_Hash,
            question, id, publicCode, dongleTime, deadTimes, deadTimesNumbers, hash, mode));
    }

    ;
	/// \}	//Функции доверенного удаленного обновления для ключей Time

	
	
	/// \defgroup GRD_TRU_OLD Устаревшие функции доверенного удаленного обновления
	/// \{
	private static native int TRU_GenerateQuestion(long hGrd, byte[] question, int[] id, byte[] publicCode, byte[] hash);

    public static GrdE GrdTRU_GenerateQuestion(Handle grdHandle, byte[] question, int[] id, byte[] publicCode, byte[] hash) {
        return new GrdE(TRU_GenerateQuestion(grdHandle.getAddress(), question, id, publicCode, hash));
    }

    private static native int TRU_GenerateQuestionTime(long hGrd, byte[] question, int[] id,
        int[] publicCode, long[] dongleTime, long[] deadTimes, long[] deadTimesNumbers, byte[] hash);

    public static GrdE GrdTRU_GenerateQuestionTime(Handle grdHandle, byte[] question, int[] id,
        int[] publicCode, long[] dongleTime, long[] deadTimes, long[] deadTimesNumbers, byte[] hash) {
        return new GrdE(TRU_GenerateQuestionTime(grdHandle.getAddress(), question, id,
            publicCode, dongleTime, deadTimes, deadTimesNumbers, hash));
    }

    private static native int TRU_DecryptQuestion(long hGrd, int algoNum_GSII64, int algoNum_HashS3, byte[] question, int id, int publicCode, byte[] hash);

    public static GrdE GrdTRU_DecryptQuestion(Handle grdHandle, int algoNum_GSII64, int algoNum_HashS3,
        byte[] question, int id, int publicCode, byte[] hash) {
        return new GrdE(TRU_DecryptQuestion(grdHandle.getAddress(), algoNum_GSII64, algoNum_HashS3, question, id, publicCode, hash));
    }

    private static native int TRU_DecryptQuestionTime(long hGrd, int algoNum_GSII64, int algoNum_HashS3,
        byte[] question, int id, int publicCode, long[] dongleTime, long[] deadTimes, int deadTimesNumbers, byte[] hash);

    public static GrdE GrdTRU_DecryptQuestionTime(Handle grdHandle, int algoNum_GSII64, int algoNum_HashS3,
        byte[] question, int id, int publicCode, long[] dongleTime, long[] deadTimes, int deadTimesNumbers, byte[] hash) {
        return new GrdE(TRU_DecryptQuestionTime(grdHandle.getAddress(), algoNum_GSII64, algoNum_HashS3,
            question, id, publicCode, dongleTime, deadTimes, deadTimesNumbers, hash));
    }

    ;

	private static native int TRU_EncryptAnswer(long hGrd, int addr, byte[] data, byte[] question, int algoNum_GSII64,
        int algoNum_HashS3, byte[] answer, int[] answerSize);

    public static GrdE GrdTRU_EncryptAnswer(Handle grdHandle, int addr, byte[] data, byte[] question, int algoNum_GSII64,
        int algoNum_HashS3, byte[] answer, int[] answerSize) {
        return new GrdE(TRU_EncryptAnswer(grdHandle.getAddress(), addr, data, question, algoNum_GSII64,
            algoNum_HashS3, answer, answerSize));
    }

	/// \} //Устаревшие функции доверенного удаленного обновления
    /// \defgroup grdTime Функции для ключей Time 
    /// \{
    /// Ключи "Guardant Time/Time Net/Code Time" обладают встроенными часами реального времени (Real-Time Clock, RTC), что позволяет ограничивать астрономическое время работы приложения путем установки временных зависимостей от аппаратных алгоритмов ключа. Для работы с возможностями, которые присущи только ключам с RTC, существуют специальные функции "Guardant API"
    /**
     * Функция <b>GrdSetTime</b> устанавливает время микросхемы таймера ключа Guardant Time.
     */
    private static native int SetTime(long hGrd, GrdSystemTime systemTime);

    public static GrdE GrdSetTime(Handle grdHandle, GrdSystemTime systemTime) {
        return new GrdE(SetTime(grdHandle.getAddress(), systemTime));
    }

    /**
     * Функция <b>GrdGetTime</b> возвращает время микросхемы таймера ключа Guardant Stealth III Time.
     */
    private static native int GetTime(long hGrd, GrdSystemTime systemTime);

    public static GrdE GrdGetTime(Handle grdHandle, GrdSystemTime systemTime) {
        return new GrdE(GetTime(grdHandle.getAddress(), systemTime));
    }

    /**
     * Функция GrdPI_GetTimeLimit возвращает оставшееся время работы алгоритма или защищённой ячейки. Числовое имя алгоритма или защищённой ячейки задается
     * параметром dwItemNum. Для работы требуется предварительно установить Private Read Code при помощи функции GrdSetAccessCode. Если время работы алгоритма
     * неограниченно, возвращается ошибка GrdE_NoService.
     */
    private static native int PI_GetTimeLimit(long hGrd, int itemNum, GrdSystemTime systemTime);

    public static GrdE GrdPI_GetTimeLimit(Handle grdHandle, int itemNum, GrdSystemTime systemTime) {
        return new GrdE(PI_GetTimeLimit(grdHandle.getAddress(), itemNum, systemTime));
    }

    /**
     * Функция <b>GrdMakeSystemTime</b> заполняет поля структуры <b>TGrdSystemTime</b>,
     */
    public static GrdE GrdMakeSystemTime(Handle grdHandle, short year, short month, short dayOfWeek, short day,
        short hour, short minute, short second, short milliseconds, GrdSystemTime systemTime) {
        if (GrdIsValidHandle(grdHandle)) {
            return GrdE.InvalidHandle;
        }

        if (year >= 2000 && year <= 2099) {
            systemTime.mYear = year;
        } else {
            return GrdE.InvalidArg;
        }
        if (month >= 1 && month <= 12) {
            systemTime.mMonth = month;
        } else {
            return GrdE.InvalidArg;
        }
        if (dayOfWeek >= 1 && dayOfWeek <= 7) {
            systemTime.mDayOfWeek = dayOfWeek;
        } else {
            return GrdE.InvalidArg;
        }
        if (day >= 1 && day <= 31) {
            systemTime.mDay = day;
        } else {
            return GrdE.InvalidArg;
        }
        if (hour >= 0 && hour <= 23) {
            systemTime.mHour = hour;
        } else {
            return GrdE.InvalidArg;
        }
        if (minute >= 0 && minute <= 59) {
            systemTime.mMinute = minute;
        } else {
            return GrdE.InvalidArg;
        }
        if (second >= 0 && second <= 59) {
            systemTime.mSecond = second;
        } else {
            return GrdE.InvalidArg;
        }
        if (milliseconds >= 0 && milliseconds <= 999) {
            systemTime.mMilliseconds = milliseconds;
        } else {
            return GrdE.InvalidArg;
        }

        return GrdE.OK;
    }

    /**
     * Функция <b>GrdSplitSystemTime</b> возвращает значения полей структуры <b>TGrdSystemTime</b>.
     */
    public static GrdE GrdSplitSystemTime(Handle grdHandle, GrdSystemTime systemTime, short[] year, short[] month,
        short[] dayOfWeek, short[] day, short[] hour, short[] minute, short[] second, short[] milliseconds) {
        if (GrdIsValidHandle(grdHandle)) {
            return GrdE.InvalidHandle;
        }

        year[0] = systemTime.mYear;
        month[0] = systemTime.mMonth;
        dayOfWeek[0] = systemTime.mDayOfWeek;
        day[0] = systemTime.mDay;
        hour[0] = systemTime.mHour;
        minute[0] = systemTime.mMinute;
        second[0] = systemTime.mSecond;
        milliseconds[0] = systemTime.mMilliseconds;

        return GrdE.OK;
    }
	/// \} //Функции для ключей Time

    /// \defgroup GrdCode Функции для ключа Guardant Code
    /// \{
    /// Технология <b>Загружаемый Код</b> может быть реализована не только при помощи утилит, входящих в комплект разработчика. При желании разработчики могут встраивать поддержку этой технологии непосредственно в свои приложения, используя набор предназначенных для этой цели функций.
    private static native int CodeGetInfo(long hGrd, int algoNum, GrdCodeInfo codeInfo);

    public static GrdE GrdCodeGetInfo(Handle grdHandle, int algoNum, GrdCodeInfo codeInfo) {
        return new GrdE(CodeGetInfo(grdHandle.getAddress(), algoNum, codeInfo));
    }

    private static native int CodeLoad(long hGrd, int algoNum, byte[] data);

    public static GrdE GrdCodeLoad(Handle grdHandle, int algoNum, byte[] data) {
        return new GrdE(CodeLoad(grdHandle.getAddress(), algoNum, data));
    }

    /**
     * Запуск загруженного модуля.
     */
    private static native int CodeRun(long hGrd, int algoNum, int P1, byte[] dataFromDongle, byte[] dataToDongle, int[] retCode);

    private static native int CodeRun(long hGrd, int algoNum, int P1, short[] dataFromDongle, short[] DataToDongle, int[] retCode);

    private static native int CodeRun(long hGrd, int algoNum, int P1, int[] dataFromDongle, int[] DataToDongle, int[] retCode);

    private static native int CodeRun(long hGrd, int algoNum, int P1, long[] dataFromDongle, long[] DataToDongle, int[] retCode);

    private static native int CodeRun(long hGrd, int algoNum, int P1, double[] dataFromDongle, double[] DataToDongle, int[] retCode);

    public static GrdE GrdCodeRun(Handle grdHandle, int algoNum, int P1, int[] retCode) {
        return new GrdE(CodeRun(grdHandle.getAddress(), algoNum, P1, (byte[]) null, (byte[]) null, retCode));
    }

    public static GrdE GrdCodeRun(Handle grdHandle, int algoNum, int P1, byte[] dataFromDongle, byte[] dataToDongle, int[] retCode) {
        return new GrdE(CodeRun(grdHandle.getAddress(), algoNum, P1, dataFromDongle, dataToDongle, retCode));
    }

    public static GrdE GrdCodeRun(Handle grdHandle, int algoNum, int P1, short[] dataFromDongle, short[] dataToDongle, int[] retCode) {
        return new GrdE(CodeRun(grdHandle.getAddress(), algoNum, P1, dataFromDongle, dataToDongle, retCode));
    }

    public static GrdE GrdCodeRun(Handle grdHandle, int algoNum, int P1, int[] dataFromDongle, int[] dataToDongle, int[] retCode) {
        return new GrdE(CodeRun(grdHandle.getAddress(), algoNum, P1, dataFromDongle, dataToDongle, retCode));
    }

    public static GrdE GrdCodeRun(Handle grdHandle, int algoNum, int P1, long[] dataFromDongle, long[] dataToDongle, int[] retCode) {
        return new GrdE(CodeRun(grdHandle.getAddress(), algoNum, P1, dataFromDongle, dataToDongle, retCode));
    }

    public static GrdE GrdCodeRun(Handle grdHandle, int algoNum, int P1, double[] dataFromDongle, double[] dataToDongle, int[] retCode) {
        return new GrdE(CodeRun(grdHandle.getAddress(), algoNum, P1, dataFromDongle, dataToDongle, retCode));
    }

    /**
     *
     * Функция <b>GrdSetDriverMode</b> переключает электронный ключ между режимами работы с драйвером HID и Guardant.
     */
    private static native int SetDriverMode(long hGrd, int mode);

    public static GrdE GrdSetDriverMode(Handle grdHandle, GrdDR mode) {
        return new GrdE(SetDriverMode(grdHandle.getAddress(), mode.mValue));
    }

    private static native int InitializeNotificationAPI();

    public static GrdE GrdInitializeNotificationAPI() {
        return new GrdE(InitializeNotificationAPI());
    }

    private static native int RegisterDongleNotification(long grdHandle, INotification base);

    public static GrdE GrdRegisterDongleNotification(Handle grdHandle, INotification base) {
        return new GrdE(RegisterDongleNotification(grdHandle.getAddress(), base));
    }

    private static native int UnRegisterDongleNotification(long grdHandle);

    public static GrdE GrdUnRegisterDongleNotification(Handle grdHandle) {
        return new GrdE(UnRegisterDongleNotification(grdHandle.getAddress()));
    }

    private static native int UnInitializeNotificationAPI();

    public static GrdE GrdUnInitializeNotificationAPI() {
        return new GrdE(UnInitializeNotificationAPI());
    }

    /**
     *
     * Функция выводит текстовое описание результата выполнения последней функции выполненной на указанном хендле.
     */
    public static String PrintResult(Handle handle) {
        String[] outMessage = new String[1];
        GrdFormatMessage(handle, GrdE.OK, GrdLNG.English, outMessage);
        return outMessage[0];
    }

    /**
     * Функция преобразует код ошибки в его текстовое описание на английском языке.
     */
    public static String PrintResult(int errorCode) {
        String[] outMessage = new String[1];
        GrdFormatMessage(new GrdE(errorCode), GrdLNG.English, outMessage);
        return outMessage[0];
    }

    /**
     * Функция преобразует код ошибки в его текстовое описание на выбранном языке.
     */
    public static String PrintResult(int errorCode, GrdLNG lng) {
        String[] outMessage = new String[1];
        GrdFormatMessage(new GrdE(errorCode), lng, outMessage);
        return outMessage[0];
    }

}
