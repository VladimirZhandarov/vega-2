package wit.guardant.ticket;

import guardant.Api;
import guardant.GrdAM;
import guardant.GrdCHM;
import guardant.GrdDT;
import guardant.GrdDongleID;
import guardant.GrdE;
import guardant.GrdF;
import guardant.GrdFM;
import guardant.GrdFMI;
import guardant.GrdFMM;
import guardant.GrdFMR;
import guardant.GrdFindInfo;
import guardant.GrdInfo;
import guardant.GrdLM;
import guardant.GrdRCS;
import guardant.Handle;
import java.util.Formatter;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Синглтон доступа к API Guardant.
 *
 * @author vneverov
 */
public class GrdDongle {

    private static final Logger LOG = Logger.getLogger(GrdDongle.class.getName());
    public static final boolean DEBUG = false;
    /**
     * PublicCode.
     */
    private static final int C0 = 0x5B17C98A; // 5B192755;
    /**
     * ReadCode.
     */
    private static final int C1 = 0xA9903C82; // A9919A4D;
    private static final GrdFMR MODE_REMOTE = GrdFMR.Local;
    private static boolean isDllLoaded;
    private static Handle handle;

    static {
        // -----------------------------------------------------------------
        // Load native mobile sdk library GrdApiJni32/GrdApiJni64 
        // -----------------------------------------------------------------
        String os = System.getProperty("os.name");
        if (os.startsWith("Windows")) {
            String gfRoot = System.getProperty("com.sun.aas.installRoot");
            if (gfRoot == null || gfRoot.isEmpty()) {
                // работаем не в контексте GF, похоже на десктопное приложение, загружаем lib
                String libName = "GrdApiJni" + System.getProperty("sun.arch.data.model");

                // откидываем jar
                String appPath = GrdDongle.class.getProtectionDomain().getCodeSource().getLocation().getPath();
                int i = appPath.indexOf("grd-ticket");
                try {
                    if (i > 0) {
                        // из консоли
                        appPath = appPath.substring(0, i);
                        String libPath = appPath + libName + ".dll";
                        System.load(libPath);
                    } else {
                        // из netbeans
//                        String path = System.getProperty("java.library.path");
//                        System.setProperty("java.library.path", ".\\dll" + ";" + (path != null ? path : ""));
                        System.loadLibrary(libName);
                    }
                    isDllLoaded = true;
                } catch (UnsatisfiedLinkError e) {
                    System.out.println("[GrdDongle] GRD native lib not found: " + e.getMessage());
//                    System.out.println("[GrdDongle] path:" + System.getProperty("java.library.path"));
                }
            } else {
                // уже должны были загрузить lib GrdNativeLibLoader
                isDllLoaded = true;
            }
        } else {
            LOG.log(Level.SEVERE, "[GrdDongle] cannot load GRD native lib: unknon OS");
        }

        if (isDllLoaded) {
            // Initialize this copy of GrdAPI. GrdStartup() must be called once before first GrdAPI call at application startup
            // Create gnclient.ini if connect for net dongle
            GrdE err = Api.GrdStartupEx(MODE_REMOTE, null, GrdRCS.ProgramData);
            try {
                errorHandling(err);
                System.out.println("[GrdDongle] GRD native lib init");
            } catch (GrdException ex) {
                LOG.log(Level.SEVERE, null, ex);
            }
        }
    }

    public static byte[] generateRandSoft(int len) {
        byte[] bb = new byte[len];
        Random r = new Random(System.currentTimeMillis());
        r.nextBytes(bb);
        return bb;
    }

    public static byte[] generateRandHard(Handle grdHandle, int len) throws GrdException {
        byte bb[] = new byte[8];
        GrdE errorCode = Api.GrdTransformEx(grdHandle, 0x2, bb, GrdAM.Decrypt, null);
        errorHandling(errorCode);
        return bb;
    }

    public static void errorHandling(GrdE error) throws GrdException {
        if (!error.equals(GrdE.OK)) {
//            LOG.log(Level.SEVERE, "[GrdDongle#ErrorHandling] Error: {0}", Api.PrintResult(error.getValue()));
            throw new GrdException("Dongle error: " + Api.PrintResult(error.getValue()));
//        } else {
//            LOG.info(Api.PrintResult(error.getValue()));
        }
    }

    public static boolean isDllLoaded() {
        return isDllLoaded;
    }

    /**
     * Хэндл к API.
     *
     * @return
     * @throws GrdException
     */
    private static Handle createHandle() throws GrdException {
        if (!isDllLoaded()) {
            throw new GrdException("[GrdDongle] GRD native library not loaded");
        }

        // -----------------------------------------------------------------
        // Creating Grd protected container & returning it's handle
        // -----------------------------------------------------------------
        Handle grdHandle = Api.GrdCreateHandle(GrdCHM.MultiThread);
        errorHandling(GrdE.OK);

        // -----------------------------------------------------------------
        // Store dongle codes in Guardant protected container
        // -----------------------------------------------------------------
        GrdE errorCode = Api.GrdSetAccessCodes(grdHandle, C0 + GrdData.C2, C1 + GrdData.C2);
        errorHandling(errorCode);
        return grdHandle;
    }

    /**
     * Вызывать при закрытии приложения.
     */
    public static void deinitApi() {
        try {
            closeHandle();
        } catch (GrdException ex) {
        }

        // -----------------------------------------------------------------
        // Deinitialize this copy of GrdAPI.
        // GrdCleanup() must be called after last GrdAPI call before program termination
        // -----------------------------------------------------------------
        System.out.println("[GrdDongle#deinitApi]");
        Api.GrdCleanup();
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    /**
     * CTOR. Не использовать.
     *
     * @throws GrdException
     */
    private GrdDongle() throws GrdException {
//        if (!isDllLoaded) {
//            throw new GrdException("[GrdDongle] GRD native library not loaded");
//        }
    }

    public static synchronized Handle getHandle(byte version) throws GrdException {
        if (!isHandle() || Api.GrdCheck(handle).getValue() != 0) {
            closeHandle();
            handle = openDongle(version);
        }

        return handle;
    }

    public static String getSN(byte version) throws GrdException {
        Handle h = getHandle(version);
        if (isHandle()) {
            byte[] bb = new byte[4];
            GrdE err = Api.GrdGetInfo(h, new GrdInfo(0x3001), bb);
            errorHandling(err);

            // reverse
            byte tmp;
            for (int i = 0, j = bb.length - 1; j > i; j--, i++) {
                tmp = bb[j];
                bb[j] = bb[i];
                bb[i] = tmp;
            }
            return byte2hex(bb, 0);
        }
        return null;
    }

    /**
     * Получить хэнд dll, найти донгл и залогинится.
     *
     * @param version
     * @return
     * @throws GrdException
     */
    private static Handle openDongle(byte version) throws GrdException {
        Handle h = createHandle();
        if (h != null && h.getAddress() != 0) {
            int LMS = -1;
            byte ProgramNumber = version; // Program number
            byte Version = 0; // Version
            short SerialNumber = 0; // Serial number
            short BitMask = 0; // Bit mask
            int DongleID = 0; // DongelID number

            GrdFM gfmFlags[] = {GrdFM.NProg};
            GrdFM DongleFlags = new GrdFM(gfmFlags);   // Operation mode flags
            GrdDT DongleType = GrdDT.ALL;             // Dongle type
            GrdFMM DongleModel = GrdFMM.ALL;            // Dongle type
            GrdFMI DongleInterface = GrdFMI.USB;            // Dongle interface

//            LOG.info("Setting dongle search conditions : ");
            GrdE err = Api.GrdSetFindMode(h, MODE_REMOTE, DongleFlags, ProgramNumber, DongleID, SerialNumber, Version, BitMask, DongleType, DongleModel, DongleInterface);
            errorHandling(err);

//            printInfo();
            err = Api.GrdLogin(h, LMS, GrdLM.PerStation);
            errorHandling(err);
            return h;
        }
        return null;
    }

    private static void closeHandle() throws GrdException {
        if (isDllLoaded() && isHandle()) {
            // Close hGrd handle. Log out from dongle/server & free allocated memory
//            LOG.info("Closing handle: ");
            GrdE err = Api.GrdCloseHandle(handle);
            errorHandling(err);
        }
        handle = null;
    }

    private static boolean isHandle() {
        return handle != null && handle.getAddress() != 0;
    }

    /**
     * Подпись ECC-160.
     *
     * @param version версия по которой ищется донгл.
     * @param n пароль активации/чтения
     * @param bb0 данные для подриси (20 byte).
     * @return подпись (40 byte).
     * @throws wit.guardant.ticket.GrdException
     */
    public static byte[] sign(byte version, int n, byte[] bb0) throws GrdException {
        Handle h = getHandle(version);

        GrdE err = Api.GrdPI_Activate(h, 0, n);
        GrdDongle.errorHandling(err);

        byte[] bb1 = new byte[40];
        err = Api.GrdSign(h, 0x0, bb0, bb1);
        GrdDongle.errorHandling(err);

        if (DEBUG) {
            System.out.println("sig: \n" + byte2hex(bb1, 32));
        }
        return bb1;
    }

    /**
     * Проверка подписи.
     *
     * @param version версия по которой ищется донгл.
     * @param n пароль активации/чтения
     * @param bb0 данные для подписи.
     * @param bb1 подпись.
     * @param bb2 пуб ключ ECC.
     * @return если 0, то подпись соответствует.
     * @throws GrdException
     */
    public static int verifySign(byte version, int n, byte[] bb0, byte[] bb1, byte[] bb2) throws GrdException {
        Handle h = getHandle(version);

        // проверка подписи
        GrdE err = Api.GrdPI_Activate(h, 0, n);
        GrdDongle.errorHandling(err);

        err = Api.GrdVerifySign(h, bb2, bb0, bb1);
        return err.getValue();
    }

    private static void printInfo() throws GrdException {
        Formatter f = new Formatter();
        try {
            // -----------------------------------------------------------------
            // Search for all specified dongles and print ID's
            // -----------------------------------------------------------------
            System.out.print("Searching for all specified dongles and print info about it : ");
            GrdFindInfo findInfo = new GrdFindInfo();

            GrdDongleID grdDongleID = new GrdDongleID();
            GrdE errorCode = Api.GrdFind(handle, GrdF.First, grdDongleID, findInfo);
            errorHandling(errorCode);

            f.format("\n-------------------------------------------------------------------------------------------------------");
            f.format("\n Public   | HVer  | Net | Type  | ID        | Prog  | Ver | SN    | Mask  | GP    | RealNet | Index    ");
            f.format("\n-------------------------------------------------------------------------------------------------------");

            while (errorCode.equals(GrdE.OK)) {
                if (errorCode.equals(GrdE.OK)) {
                    f.format("\n").toString();
                    f.format("%08XH |", findInfo.mPublicCode).toString();      // Public code
                    f.format(" %02XH   |", findInfo.mHrwVersion).toString();   // Public code
                    f.format(" %3d |", (int) findInfo.mMaxNetRes).toString();      // Maximum LAN license limit
                    f.format(" %04XH |", findInfo.mType).toString();           // Dongle type flags
                    f.format(" %08XH |", findInfo.mID).toString();             // Dongle's ID (unique)
                    f.format(" %3d   |", (int) findInfo.mProg & 0xff).toString();         // Program number
                    f.format(" %3d |", (int) findInfo.mVer & 0xff).toString();            // Version
                    f.format(" %5d |", (int) findInfo.mSN & 0xffff).toString();             // Serial number
                    f.format(" %04XH |", findInfo.mMask).toString();           // Bit mask
                    f.format(" %5d |", (int) findInfo.mGP & 0xffff).toString();             // Executions GP counter/ License time counter
                    f.format(" %5d   |", (int) findInfo.mRealNetRes & 0xffff).toString();   // Current LAN license limit, must be <=kmLANRes
                    f.format(" %08XH ", findInfo.mIndex).toString();           // Index for remote programming
                }
                errorCode = Api.GrdFind(handle, GrdF.Next, grdDongleID, findInfo);
            }

            f.format("\n").toString();
            System.out.print(f.toString());

            if (errorCode.equals(GrdE.AllDonglesFound)) {
                System.out.print("\n\nDongle search is complete with: no errors\n");
            } else {
                errorHandling(errorCode);
            }
        } finally {
            f.close();
        }
    }

    /**
     *
     * @param byteData
     * @param rowLen если != 0 то нарезка на строк указанной длины
     * @return
     */
    public static String byte2hex(byte[] byteData, int rowLen) {
        if (byteData == null || byteData.length == 0) {
            return "";
        }

        StringBuilder buf = new StringBuilder();
        for (int i = 0; i < byteData.length; i++) {
            String hex = Integer.toHexString(0xff & byteData[i]).toUpperCase();
            if (hex.length() == 1) {
                buf.append('0');
            }
            buf.append(hex);
        }

        if (rowLen <= 0) {
            return buf.toString();
        }

//        String s = buf.toString();
        int n = buf.length() / rowLen;
        StringBuilder buf2 = new StringBuilder();
        for (int i = 0; i < n; i++) {
            buf2.append(buf.substring(i * rowLen, (i + 1) * rowLen));
            buf2.append("\r\n");
        }

        if (n % rowLen != 0) {
            buf2.append(buf.substring(n * rowLen));
        }

        return buf2.toString();
    }

    public static byte[] hex2byte(final String encoded) {
        String s = encoded.replace("\r", "").replace("\n", "");

        if ((s.length() % 2) != 0) {
            return null;
        }

        final byte result[] = new byte[s.length() / 2];
        final char enc[] = s.toCharArray();
        for (int i = 0; i < enc.length; i += 2) {
            StringBuilder curr = new StringBuilder(2);
            curr.append(enc[i]).append(enc[i + 1]);
            result[i / 2] = (byte) Integer.parseInt(curr.toString(), 16);
        }
        return result;
    }
}
