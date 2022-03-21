/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.proto;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class DataMessage {
    private int sourceID;
    private int messageID;
    private int bodySize;
    private Protocol.Body body;
    
    public DataMessage(int n, int n2, Protocol.Body body) {
        this(n, n2, body, 0);
    }

    public DataMessage(int n, int n2, Protocol.Body body, int n3) {
        this.sourceID = n;
        this.messageID = n2;
        this.body = body;
        this.bodySize = n3;
    }

    public byte[] toBytes() throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byteArrayOutputStream.write(4);
        byteArrayOutputStream.write(Constants.EMPTY_INT);
        byteArrayOutputStream.write(Transport.intToBytes(this.sourceID));
        byteArrayOutputStream.write(Transport.intToBytes(this.messageID));
        this.body.writeTo((OutputStream)byteArrayOutputStream);
        byte[] arrby = byteArrayOutputStream.toByteArray();
        byte[] arrby2 = Transport.intToBytes(arrby.length - 13);
        System.arraycopy(arrby2, 0, arrby, 1, 4);
        return arrby;
    }

    public static DataMessage fromBytes(byte[] arrby) throws IllegalArgumentException, IOException {
        return DataMessage.fromBytes(arrby, false);
    }

    public static DataMessage fromBytes(byte[] arrby, boolean bl) throws IllegalArgumentException, IOException {
        if (arrby.length < 13) {
            throw new IllegalArgumentException("byte buffer size too small; shall be at least 13");
        }
        if (arrby[0] != 4) {
            throw new IllegalArgumentException("invalid message code; expected 0x4");
        }
        byte[] arrby2 = new byte[4];
        System.arraycopy(arrby, 1, arrby2, 0, 4);
        int n = Transport.bytesToInt(arrby2);
        byte[] arrby3 = new byte[4];
        System.arraycopy(arrby, 5, arrby3, 0, 4);
        int n2 = Transport.bytesToInt(arrby3);
        byte[] arrby4 = new byte[4];
        System.arraycopy(arrby, 9, arrby4, 0, 4);
        int n3 = Transport.bytesToInt(arrby4);
        Protocol.Body body = null;
        if (!bl) {
            if (n > arrby.length - 13) {
                throw new IllegalArgumentException("invalid data buffer size; expected " + n + " bytes");
            }
            body = Protocol.Body.parseFrom((InputStream)new ByteArrayInputStream(arrby, 13, n));
        }
        return new DataMessage(n2, n3, body, n);
    }

    public int getSourceID() {
        return this.sourceID;
    }

    public void setSourceID(int n) {
        this.sourceID = n;
    }

    public int getMessageID() {
        return this.messageID;
    }

    public void setMessageID(int n) {
        this.messageID = n;
    }

    public Protocol.Body getBody() {
        return this.body;
    }

    public void setBody(Protocol.Body body) {
        this.body = body;
    }

    public int getBodySize() {
        return this.bodySize;
    }
}
