package wit.krm.itrium.bridge;

import java.io.BufferedOutputStream;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.Serializable;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.annotation.Resource;
import javax.ejb.Remove;
import javax.ejb.Stateful;
import javax.resource.spi.work.Work;
import javax.resource.spi.work.WorkException;
import javax.resource.spi.work.WorkManager;
import org.apache.commons.io.IOUtils;
import wit.vega.frame.MsgCamEvent;
import wit.vega.frame.ResponseFactory;
import wit.vega.frame.VegaCmd;

@Stateful
public class TcpListener implements TcpListenerLocal {

    private static final Logger LOG = Logger.getLogger(TcpListener.class.getName());
    private long camId;
    private List<MainSocketHandler> socketHandlers = new ArrayList<MainSocketHandler>();
    private boolean isBlockListen = false;
    
    /**
     * Порт получает событие соответствие списку А
     */
    private Integer callbackPortA;
    
    /**
     * Порт получает событие соответствие списку В
     */
    private Integer callbackPortB;
    
    /**
     * Порт получает событие не соответствие списку А
     */
    private Integer callbackPortNA;
    
    /**
     * Порт получает событие не соответствие списку В
     */
    private Integer callbackPortNB;

    @Resource
    private com.sun.appserv.connectors.internal.api.WorkManagerFactory wmFac;

    @Override
    public TcpListenerLocal.EnCamState getState() {
        TcpListenerLocal.EnCamState state = EnCamState.CAM_STOPED;
        for (MainSocketHandler msh : socketHandlers) {
            if (state.ordinal() < msh.getState().ordinal()) {
                state = msh.getState();
            }
        }
        return state;
    }

    /**
     * Слушатель серверного сокета, в отдельном потоке.
     */
    private class MainSocketHandler implements Work {

        private int port;
        private ServerSocket srvSocket;
        private EnCamState state = EnCamState.CAM_STOPED;
        private WorkManager acceptWorkManager;

        public MainSocketHandler(int port) {
            this.port = port;
        }

        @Override
        public void release() {
        }

        @Override
        public void run() {
            state = EnCamState.CAM_RUNNING;
            try {
                srvSocket = new ServerSocket(port);
                state = EnCamState.CAM_RUNED;
                LOG.log(Level.INFO, "[MainSocketHandler] port opened: {0}", port);

                while (state == EnCamState.CAM_RUNED) {
                    if (!isBlockListen) {
                        Socket socket = srvSocket.accept();
                        WorkManager workManager = getAcceptWorkManager();
                        try {
                            workManager.scheduleWork(new RequestSocketHandler(socket));
                        } catch (WorkException ex) {
                            LOG.log(Level.SEVERE, null, ex);
                        }
                    } else {
                        try {
                            Thread.sleep(500);
                        } catch (InterruptedException ex) {
                            LOG.log(Level.SEVERE, null, ex);
                        }
                    }
                }
            } catch (IOException e) {
                LOG.log(Level.SEVERE, "[MainSocketHandler] cannot open port: {0} ({1})", new Object[]{port, e.getMessage()});
            } finally {
                if (srvSocket != null) {
                    try {
                        srvSocket.close();
                    } catch (IOException ex) {
                        LOG.log(Level.SEVERE, null, ex);
                    }
                }
                LOG.log(Level.INFO, "[MainSocketHandler] finish listen port: {0}", port);
//                state = EnCamState.CAM_STOPED;
                socketHandlers.remove(this);
            }
        }

        public void stop() {
            state = EnCamState.CAM_STOPPING;
            if (srvSocket != null) {
                try {
                    srvSocket.close();
                } catch (IOException ex) {
                    LOG.log(Level.SEVERE, null, ex);
                }
            }
        }

        public EnCamState getState() {
            return state;
        }

        private WorkManager getAcceptWorkManager() {
            if (acceptWorkManager == null) {
                acceptWorkManager = wmFac.createWorkManager(TcpListenerLocal.THREAD_POOL_NAME, null, getClass().getClassLoader());
            }
            return acceptWorkManager;
        }
    };
    
    /**
     * Обработка TCP пакета в отдельном потоке.
     */
    private class RequestSocketHandler implements Work {

        private Socket socket;

        public RequestSocketHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void release() {
        }

        @Override
        public void run() {
            InputStream bis = null;
            BufferedOutputStream bos = null;
            try {
                //
                // Read a message sent by client application
                //
                byte[] buf = IOUtils.toByteArray(socket.getInputStream());
                if (buf.length > 0) {
                    bis = new ByteArrayInputStream(buf);
                    LOG.log(Level.FINE, "[RequestSocketHandler#run] Received {0} bytes", 
                            bis.available());

                    VegaCmd respCmd = ResponseFactory.deserialize(bis);
                    if (!isBlockListen && respCmd instanceof MsgCamEvent) {
                        MsgCamEvent evn = (MsgCamEvent) respCmd;
                        String plateNum = evn.getPlateNum();
                        String plateCntry = evn.getPlateCountry();
                        byte[] photo = evn.getBWPhoto();
                        Date d = evn.getDate();
                        Date t = evn.getTime();
                        Date date;
                        if (d != null && t != null) {
                            date = new Date(d.getYear(), d.getMonth(), d.getDate(), t.getHours(), t.getMinutes(), t.getSeconds());
                        } else {
                            date = new Date();
                        }
                    }
                }
            } catch (Exception e) {
                LOG.log(Level.SEVERE, "[TcpListener.run] {0}", e);
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
                    LOG.log(Level.SEVERE, "[run] close: {0}", ex.getMessage());
                }
            }
        }
    }

    @Override
    public void setIsBlockListen(boolean isBlockListen) {
        this.isBlockListen = isBlockListen;
        LOG.log(Level.FINE, "[setIsBlockListen] {0} {1}", new Object[]{camId, isBlockListen});
    }

    public void init(final long camId) {
        this.camId = camId;
    }

    @Remove
    public void remove() {
        System.out.println(String.format("[TcpListener] removed"));
        stopListen();
    }

    @Override
    public void setRunState(final long camId,
            final Integer callbackPortA, final Integer callbackPortB, final Integer callbackPortNA, final Integer callbackPortNB) throws IOException {
        if (getState() == EnCamState.CAM_STOPED) {
            this.callbackPortA = callbackPortA;
            this.callbackPortB = callbackPortB;
            this.callbackPortNA = callbackPortNA;
            this.callbackPortNB = callbackPortNB;
            runListen();
        }
    }

    @Override
    public void setStopState() throws IOException {
        EnCamState state = getState();
        if (state != EnCamState.CAM_STOPED && state != EnCamState.CAM_STOPPING) {
            stopListen();
        }
    }

    //-------------------------------------------------------------------------
    private void runListen() {
        try {
            /**
             * create a new work manager for the resource-adapter
             *
             * @param threadPoolId thread-pool-id
             * @param raName resource-adapter-name
             * @return WorkManager
             */
            WorkManager workManager = wmFac.createWorkManager(TcpListenerLocal.THREAD_POOL_NAME, null, getClass().getClassLoader());
            LOG.log(Level.INFO, "[runListen] '{0}' starting listen ports:", camId);
            if (callbackPortA != null) {
                MainSocketHandler sh = new MainSocketHandler(callbackPortA);
                workManager.scheduleWork(sh);
                socketHandlers.add(sh);
                LOG.log(Level.INFO, "\tport: {0}", callbackPortA);
            }

            if (callbackPortB != null) {
                MainSocketHandler sh = new MainSocketHandler(callbackPortB);
                workManager.scheduleWork(sh);
                socketHandlers.add(sh);
                LOG.log(Level.INFO, "\tport: {0}", callbackPortB);
            }

            if (callbackPortNA != null) {
                MainSocketHandler sh = new MainSocketHandler(callbackPortNA);
                workManager.scheduleWork(sh);
                socketHandlers.add(sh);
                LOG.log(Level.INFO, "\tport: {0}", callbackPortNA);
            }

            if (callbackPortNB != null) {
                MainSocketHandler sh = new MainSocketHandler(callbackPortNB);
                workManager.scheduleWork(sh);
                socketHandlers.add(sh);
                LOG.log(Level.INFO, "\tport: {0}", callbackPortNB);
            }
        } catch (WorkException e) {
            LOG.log(Level.SEVERE, null, e);
        }
    }

    private synchronized void stopListen() {
        LOG.log(Level.INFO, "[stopListen] Stopping listen ports...");
        long t = System.currentTimeMillis();

        List<MainSocketHandler> socketHandlers2 = new ArrayList<MainSocketHandler>(socketHandlers);
        for (MainSocketHandler sh : socketHandlers2) {
            sh.stop();
        }

        for (int i = 0; i < 100; i++) {
            if (getState() == EnCamState.CAM_STOPED) {
                break;
            }
            try {
                Thread.sleep(50);
            } catch (InterruptedException ex) {
                LOG.log(Level.SEVERE, null, ex);
            }
        }
        LOG.log(Level.INFO, "[stopListen] Listen ports stopped in {0}, ms", System.currentTimeMillis() - t);
    }
}
