package wit.vega.net;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import wit.vega.frame.VegaHeaderFrame;
import wit.vega.frame.VegaMsgTag;
import wit.vega.VegaException;

/**
 *
 * @author vneverov
 */
public class TcpServerHandler implements Runnable {

    private static final Logger LOG = Logger.getLogger(TcpServerHandler.class.getName());
    private final Socket socket;

    public TcpServerHandler(Socket socket) {
        this.socket = socket;
        Thread t = new Thread(this);
        t.start();
    }

    public void run() {
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        try {
            //
            // Read a message sent by client application
            //
            bis = new BufferedInputStream(socket.getInputStream());
            for (int i = 0; i < 10; i++) {
                if (bis.available() > 0) {
                    LOG.info(String.format("Received %d bytes", bis.available()));

                    VegaHeaderFrame header = new VegaHeaderFrame();
                    header.deserialize(bis);

                    VegaMsgTag frame = new VegaMsgTag(header);
                    try {
                        frame.deserialize(bis);
                    } catch (VegaException ex) {
                        LOG.log(Level.SEVERE, null, ex);
                    }
                    break;
                }
                try {
                    Thread.sleep(500);
                } catch (InterruptedException ex) {
                    LOG.log(Level.SEVERE, null, ex);
                }
            }
        } catch (IOException e) {
            LOG.log(Level.SEVERE, null, e);
        } finally {
            try {
                if (bis != null) {
                    bis.close();
                }

                if (bos != null) {
                    bos.close();
                }
                socket.close();
            } catch (IOException ex) {
                LOG.log(Level.SEVERE, null, ex);
            }
        }
    }
}
