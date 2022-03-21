package wit.krm.itrium.bridge;

import java.io.IOException;
import javax.ejb.Local;

/**
 *
 * @author vneverov
 */
@Local
public interface TcpListenerLocal {

    enum EnCamState {
        CAM_STOPED,
        CAM_STOPPING,
        CAM_RUNNING,
        CAM_RUNED
    };

    String JNDI_NAME = "java:module/TcpListener!wit.vega.ejb.TcpListenerLocal";
    String THREAD_POOL_NAME = "thread-pool-1";

    void init(final long camId);

    void setIsBlockListen(boolean isBlockListen);

    void remove();

    EnCamState getState();

//    Integer getListenPort();

    /**
     * Запуск слушанья камеру.
     *
     * @param camId
     * @param host
     * @param port
     * @param callbackPortA соответствие списку А
     * @param callbackPortB соответствие списку В
     * @param callbackPortNA не соответствие списку А
     * @param callbackPortNB не соответствие списку В
     * @throws IOException
     */
    void setRunState(final long camId,
            final Integer callbackPortA, final Integer callbackPortB,
            final Integer callbackPortNA, final Integer callbackPortNB) throws IOException;

    void setStopState() throws IOException;
}
