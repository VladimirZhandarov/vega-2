package wit.vegaproto;

/**
 *
 * @author vneverov
 */
public class VegaUtils {

    public static String getHex(byte b) {
        return String.format("%02X", b);
    }

    public static String getHex(byte[] b) {
        return getHex(b, 0, b.length);
    }

    public static String getHex(byte[] b, int beg, int count) {
        String s = "";
        if (null != b) {
            int end = beg + count;
            for (int i = beg; i < end; i++) {
                s += String.format("%02X", b[i]) + " ";
            }
        }
        return s;
    }
}
