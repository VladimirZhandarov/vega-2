/*
 * GrdGIR.java
 *
 * Created on 23 Ноябрь 2007 г., 1:03
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Константы для получения информации с помощью функции GrdGetInfo.
 * <br/>Информация о текущем сетевом ключе.
 */
public class GrdGIR extends GrdInfo {

    /**
     * Guardant Net server version
     */
    public static final GrdGIR VerSrv = new GrdGIR(0x5000);
    /**
     * Guardant Net local IP
     */
    public static final GrdGIR LocalIP = new GrdGIR(0x5001);
    /**
     * Guardant Net local IP port
     */
    public static final GrdGIR LocalPort = new GrdGIR(0x5002);
    /**
     * Guardant Net local NetBIOS name
     */
    public static final GrdGIR LocalNB = new GrdGIR(0x5003);
    /**
     * Guardant Net remote IP
     */
    public static final GrdGIR RemoteIP = new GrdGIR(0x5004);
    /**
     * Guardant Net remote IP port
     */
    public static final GrdGIR RemotePort = new GrdGIR(0x5005);
    /**
     * Guardant Net remote NetBIOS name
     */
    public static final GrdGIR RemoteNB = new GrdGIR(0x5006);
    /**
     * Handle of internal heartbeat thread
     */
    public static final GrdGIR HeartBeatThread = new GrdGIR(0x5007);
    /**
     * Send operation timeout in seconds. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniTimeOutSend = new GrdGIR(0x5008);
    /**
     * Receive operation timeout in seconds. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniTimeOutReceive = new GrdGIR(0x5009);
    /**
     * Broadcasting search timeout in seconds. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniTimeOutSearch = new GrdGIR(0x500A);
    /**
     * Client's UDP port for sending of datagrams to a server. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniClientUDPPort = new GrdGIR(0x500B);
    /**
     * Server's UDP port for sending of replies to a client. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniServerUDPPort = new GrdGIR(0x500C);
    /**
     * Broadcasting address . Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniBroadcastAddress = new GrdGIR(0x500D);
    /**
     * Initialization file name. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniFileName = new GrdGIR(0x500E);
    /**
     * MAC address of the local network adapter. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR LocalMACAddress = new GrdGIR(0x500F);
    /**
     * Full name of the local host. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR FullHostName = new GrdGIR(0x5010);
    /**
     * Server IP address or host name. Requires the API to be started up with GrdFMR_Remote flag.
     */
    public static final GrdGIR IniServerIPName = new GrdGIR(0x5011);

    GrdGIR(int value) {
        super(value);
    }
}
