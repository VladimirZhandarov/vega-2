/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.net;

import java.io.InputStream;
import java.net.Socket;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import wit.vegaproto.proto.ConnectMessage;
import wit.vegaproto.proto.DataMessage;
import wit.vegaproto.proto.ResponseDataMessage;

public class TcpConnection implements Connection {

    private static final Logger logger = LoggerFactory.getLogger((Class) TcpConnection.class);
    private Socket clientSocket;
    private String host;
    private int port;

    public TcpConnection(String string, int n) {
        this.host = string;
        this.port = n;
    }

    @Override
    public ResponseDataMessage transmit(DataMessage dataMessage) throws Exception {
        if (this.clientSocket == null) {
            this.clientSocket = new Socket(this.host, this.port);
            this.shakeHands();
        }
        byte[] arrby = dataMessage.toBytes();
        logger.debug("sending {} bytes to server", (Object) arrby.length);
        InputStream inputStream = this.clientSocket.getInputStream();
        this.clientSocket.getOutputStream().write(arrby);
        logger.debug("reading response from server");
        byte[] arrby2 = new byte[9];
        int n = inputStream.read(arrby2);
        logger.debug("received response of size={} from server", (Object) n);
        return ResponseDataMessage.fromBytes((byte[]) arrby2);
    }

    @Override
    public void close() throws Exception {
        if (this.clientSocket != null) {
            this.clientSocket.close();
        }
    }

    private void shakeHands() throws Exception {
        logger.debug("transmitting handshake message; sourceId={}, version={}.{}", new Object[]{239, Byte.valueOf("2"), Byte.valueOf("1")});
        ConnectMessage connectMessage = new ConnectMessage(239, Byte.valueOf("2"), Byte.valueOf("1"));
        InputStream inputStream = this.clientSocket.getInputStream();
        this.clientSocket.getOutputStream().write(connectMessage.toBytes());
        byte[] arrby = new byte[1];
        inputStream.read(arrby);
        if (arrby[0] == 2) {
            logger.info("server confirmed handshake; connection established");
        } else if (arrby[0] == 3) {
            logger.warn("server not confirmed handshake; acting at own risk");
        } else {
            logger.error("received unexpected response code={}", (Object) Integer.valueOf(arrby[0]));
        }
    }
}
