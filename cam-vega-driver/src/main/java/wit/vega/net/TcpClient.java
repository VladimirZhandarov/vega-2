package wit.vega.net;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import wit.vega.frame.ResponseFactory;
import wit.vega.frame.VegaCmd;
import wit.vega.VegaException;

/**
 *
 * @author vneverov
 */
public class TcpClient {

    private static final Logger LOG = Logger.getLogger(TcpClient.class.getName());
    private static final int SOCK_TIMEOUT = 3000;
    private Socket socket;
    private BufferedOutputStream ous;
    private BufferedInputStream ins;

    /**
     * Ждать пока не загрузится нужный объем данных (Не более 3 сек).
     *
     * @param ins
     * @param len сколько надо загрузить.
     * @throws IOException
     */
    public static void waitStream(InputStream ins, Integer len) throws IOException, VegaException {
//        int n = 0;
//        for (int i = 0; i < 30 && (n = ins.available()) < len; i++) {
//            try {
//                Thread.sleep(100);
//            } catch (InterruptedException ex) {
//            }
//        }
//
//        if (n < len) {
//            throw new VisiterException("[waitStream] Error download data, need: " + len + ", found: " + n);
//        }
    }

    public void open(String host, int port) throws UnknownHostException, IOException {
        socket = new Socket();
        socket.connect(new InetSocketAddress(host, port), SOCK_TIMEOUT);
        ous = new BufferedOutputStream(socket.getOutputStream());
        ins = new BufferedInputStream(socket.getInputStream());
    }

    public void close() throws IOException {
        if (ous != null) {
            ous.close();
            ous = null;
        }
        if (ins != null) {
            ins.close();
            ins = null;
        }
        if (socket != null) {
            socket.close();
            socket = null;
        }
    }

    public VegaCmd sendReceive(VegaCmd cmd) throws IOException, VegaException {
        if (cmd == null) {
            throw new VegaException("[TcpClient#sendReceive] zero parameter");
        }
        checkIsOpened();
        send(cmd);
        cmd.log("[TcpClient#sendReceive] Send");

        // Ждем пока загрузится ответ, пока не будет новых данных 0,5 сек
        long t = System.currentTimeMillis();
        ByteArrayOutputStream bous = new ByteArrayOutputStream();
        int emptyCnt = 0;
        int n = 0;
        while (emptyCnt < 5) {
            if ((n = ins.available()) > 0) {
                byte[] bb = new byte[n];
                ins.read(bb);
                bous.write(bb);
                emptyCnt = 0;
//                LOG.info("[TcpClient#sendReceive] loaded " + l);
            } else {
                emptyCnt++;
                try {
                    Thread.sleep(100);
                } catch (InterruptedException ex) {
                    LOG.log(Level.SEVERE, null, ex);
                }
            }
        }

        int sz = bous.toByteArray().length;
        if (sz == 0) {
            throw new VegaException("[TcpClient#sendReceive] no answear");
        }

        LOG.log(Level.INFO, "[TcpClient#sendReceive] loaded {0} in {1}, ms", new Object[]{sz, (System.currentTimeMillis() - t - 500)});
        ByteArrayInputStream bis = new ByteArrayInputStream(bous.toByteArray());
        return receive(bis);
    }

    public void send(VegaCmd cmd) throws IOException {
        checkIsOpened();
        cmd.serialize(ous);
        ous.flush();
    }

    private VegaCmd receive(InputStream ins) throws IOException, VegaException {
        if (ins.available() != 0) {
            ResponseFactory rf = new ResponseFactory();
            VegaCmd resp = rf.deserialize(ins);
            if (resp != null) {
//                resp.log("[TcpClient] Receive");
                return resp;
            }
        }
        LOG.info("[TcpClient#receive] received zero");
        return null;
    }

    ////////////////////////////////////////////////////////////////
    private void checkIsOpened() {
        if (socket == null || ins == null || ous == null) {
            throw new RuntimeException("Trying send on not opened socket");
        }
    }
}
