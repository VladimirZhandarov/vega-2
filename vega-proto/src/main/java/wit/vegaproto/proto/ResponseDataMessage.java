/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.proto;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class ResponseDataMessage {
    private int messageID;
    private int size;
    
    public ResponseDataMessage(int n, int n2) {
        this.messageID = n;
        this.size = n2;
    }

    public byte[] toBytes() throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byteArrayOutputStream.write(5);
        byteArrayOutputStream.write(Transport.intToBytes(this.size));
        byteArrayOutputStream.write(Transport.intToBytes(this.messageID));
        byte[] arrby = byteArrayOutputStream.toByteArray();
        byte[] arrby2 = Transport.intToBytes(arrby.length - 13);
        System.arraycopy(arrby2, 0, arrby, 1, 4);
        return arrby;
    }

    public static ResponseDataMessage fromBytes(byte[] arrby) throws IllegalArgumentException, IOException {
        if (arrby.length < 9) {
            throw new IllegalArgumentException("byte buffer size too small; shall be at least 9");
        }
        if (arrby[0] != 5) {
            throw new IllegalArgumentException("invalid message code; expected 0x5");
        }
        byte[] arrby2 = new byte[4];
        System.arraycopy(arrby, 1, arrby2, 0, 4);
        int n = Transport.bytesToInt(arrby2);
        byte[] arrby3 = new byte[4];
        System.arraycopy(arrby, 5, arrby3, 0, 4);
        int n2 = Transport.bytesToInt(arrby3);
        return new ResponseDataMessage(n2, n);
    }

    public int getMessageID() {
        return this.messageID;
    }

    public void setMessageID(int n) {
        this.messageID = n;
    }
}
