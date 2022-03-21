package guardant;

/**
 * Структура используемая в функции GrdFind() для получения информации об электронном ключе
 */
public class GrdFindInfo {

    /**
     * Общий код доступа
     */
    public int mPublicCode;
    /**
     * Версия прошивки ключа
     */
    public byte mHrwVersion;
    /**
     * Максимальный сетевой ресурс
     */
    public byte mMaxNetRes;
    /**
     * Dongle type flags
     */
    public short mType;
    /**
     * Dongle's ID (unique)
     */
    public int mID;
    // Following fields are available from UAM mode
    /**
     * Номер продукта
     */
    public byte mProg;
    /**
     * Версия продукта
     */
    public byte mVer;
    /**
     * Серийный номер (Serial number)
     */
    public short mSN;
    /**
     * Маска (Bit mask)
     */
    public short mMask;
    /**
     * Счетчик запусков (Executions GP counter/ License time counter)
     */
    public short mGP;
    /**
     * Назначенный сетевой ресурс ключа
     */
    public short mRealNetRes;
    /**
     * Индекс
     */
    public int mIndex;
    // Only Stealth III info
    /**
     * Stealth III write protect address
     */
    public short mWriteProtectS3;
    /**
     * Stealth III read protect address
     */
    public short mReadProtectS3;
    /**
     * Global dongle flags for Sign\Time or higher dongles. See GrdGF_xxx definition.
     */
    public short mGlobalFlags;
    /**
     * Dongle State. See GrdDSF_XXX definition
     */
    public int mDongleState;
    // Available since:
    // 1. Stealth Sign.(Firmware number >= 0x01000011h or 01.00.00.11)
    // 2. Guardant Code.
    /**
     *
     */
    public int mOldMPNum;             // Old firmware number(before SFU).
    // Reserved info from gsA
    // Driver info
    /**
     * Reserved Driver platform (Win32/Win64)
     */
    public int mGrDrv_Platform;
    /**
     * Reserved Driver version (0x04801234=4.80.12.34
     */
    public int mGrDrv_Vers;
    /**
     * Reserved Driver build
     */
    public int mGrDrv_Build;
    /**
     * Reserved reserved
     */
    public int mGrDrv_Reserved;
    // dongle info
    /**
     * Reserved wkmUserAddr
     */
    public int mRkmUserAddr;
    /**
     * Reserved wkmAlgoAddr
     */
    public int mRkmAlgoAddrW;
    /**
     * Reserved Printer port address or 0 if it USB
     */
    public int mPrnPort;
    /**
     * Reserved Dongle client version
     */
    public int mClientVersion;
    // SAP start
    /**
     * Reserved Type of MCU
     */
    public int mRFlags;
    /**
     * Reserved Program version (in MCU)
     */
    public int mRProgVer;
    /**
     * Reserved curr_num & answer code
     */
    public int mRcn_rc;
    /**
     * Reserved Number of compare conditions
     */
    public int mNcmps;
    /**
     * Reserved Client version (low byte - minor, hi - major)
     */
    public int mNSKClientVersion;
    /**
     * Dongle Model
     */
    public int mModel;
    /**
     * Dongle MCU Type
     */
    public int mMcuType;
    /**
     * Dongle Memory Type
     */
    public int mMemoryType;

}
