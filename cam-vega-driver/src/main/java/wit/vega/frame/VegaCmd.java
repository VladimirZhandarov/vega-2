package wit.vega.frame;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;
import wit.vega.VegaUtils;
import wit.vega.VegaException;

/**
 * Базовый для ответов от камеры.
 *
 * @author vneverov
 */
public abstract class VegaCmd {

    private static final Logger LOG = Logger.getLogger(VegaCmd.class.getName());
    public VegaCmdTag frame;

    public void log(String msg) throws IOException {
        ByteArrayOutputStream baous = new ByteArrayOutputStream();
        frame.serialize(baous);
        byte[] bout = baous.toByteArray();
        String s = VegaUtils.getHex(bout);
        LOG.log(Level.FINE, "{0}, {1} bytes: {2}", new Object[]{msg, bout.length, s});
        baous.close();
    }

    public void serialize(OutputStream ous) throws IOException {
        frame.serialize(ous);
    }

    public void deserialize(InputStream ins) throws IOException, VegaException {
        frame.deserialize(ins);
    }
}
