package wit.krm.admin;

/**
 *
 * @author vneverov
 */
public class Utils {

    private static EnOS os;

    public enum EnOS {

        OS_WND,
        OS_LIN,
        OS_UNK
    }

    public static EnOS getOs() {
        if (os == null || os == EnOS.OS_UNK) {
            String osName = System.getProperty("os.name");
            if (osName.startsWith("Window")) {
                os = EnOS.OS_WND;
            } else if (osName.startsWith("Linux")) {
                os = EnOS.OS_LIN;
            } else {
                os = EnOS.OS_UNK;
            }
        }
        return os;
    }
}
