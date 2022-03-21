package wit.krm.admin;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author vneverov
 */
public class ProgK {

    private static final Logger LOG = Logger.getLogger(ProgK.class.getName());
    private static final String CMD_SN_WIN = "wmic cpu get processorid";
    private static final String CMD_SN_LIN = "sudo dmidecode -s baseboard-serial-number";

    public String do0(String s) {
        MessageDigest md;
        try {
            md = MessageDigest.getInstance("SHA-256");
        } catch (NoSuchAlgorithmException ex) {
            LOG.log(Level.SEVERE, null, ex);
            return null;
        }

        md.update(s.getBytes());
        byte bb[] = md.digest();
        return do1(bb);
    }

    /**
     * byte2hex
     * 
     * @param bb
     * @return 
     */
    public String do1(byte[] bb) {
        StringBuilder buf = new StringBuilder();
        for (int i = 0; i < bb.length; i++) {
            String hex = Integer.toHexString(0xff & bb[i]).toUpperCase();
            if (hex.length() == 1) {
                buf.append('0');
            }
            buf.append(hex);
        }
        return buf.toString();
    }
    
    /**
     * Проверка по SN CPU для WIN и SN MB для Linux.
     *
     * @param s зашифрованный серийник
     * @return
     */
    public boolean do2(String s) {
        KrmCtrl ctrl = new KrmCtrl();
        String resp = null;
        try {
            if (Utils.getOs() == Utils.EnOS.OS_LIN) {
//                resp = ctrl.exec(CMD_SN_LIN);
                // TODO: не работает получение серийника платы
                return true;
            } else {
                resp = ctrl.exec(CMD_SN_WIN);
                if (resp != null && !resp.isEmpty()) {
                    int n = resp.indexOf("\n");
                    String s2 = resp.substring(n + 1); // PROCESSOR ID
                    LOG.info("[lic] " + s2);
                }
            }
        } catch (KrmException ex) {
            LOG.severe("[lic] null");
            LOG.log(Level.SEVERE, null, ex);
            return false;
        }

        if (resp != null) {
            String s2 = do0(resp);
            return s2 != null && s2.equals(s);
        }

        LOG.severe("[lic] null");
        return false;
    }
}
