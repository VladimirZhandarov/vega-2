package wit.vega.net;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import org.apache.log4j.Logger;

/**
 * Слушатель событий камеры.
 *
 * @author vneverov
 */
public class TcpServer implements Runnable {

    private static Logger log = Logger.getLogger(TcpServer.class);
    private ServerSocket server;
    private boolean isStopThread = false;

    public TcpServer(int port) {
        try {
            server = new ServerSocket(port);
            log.info("Start listen port: " + port);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void start() {
        Thread t = new Thread(this);
        t.start();
    }

    public void run() {
        log.info("Waiting for client message...");
        while (!isStopThread) {
            try {
                Socket socket = server.accept();
                new TcpServerHandler(socket);
            } catch (IOException e) {
            }
        }
        isStopThread = false;
    }

    public void stop() {
        isStopThread = true;
        try {
            server.close();
        } catch (IOException ex) {
        }

        for (int i = 0; i < 100 && isStopThread; i++) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException ex) {
            }
        }
    }
}
