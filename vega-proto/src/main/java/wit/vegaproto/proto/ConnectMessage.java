/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.proto;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class ConnectMessage {
    private int sourceId;
    private byte versionHi;
    private byte versionLo;

    public ConnectMessage(int n, byte by, byte by2) {
        this.sourceId = n;
        this.versionHi = by;
        this.versionLo = by2;
    }

    public static ConnectMessage fromBytes(byte[] arrby) throws IllegalArgumentException, IOException {
        if (arrby.length < 7) {
            throw new IllegalArgumentException("byte buffer size too small; shall be at least 7");
        }
        if (arrby[0] != 1) {
            throw new IllegalArgumentException("invalid message code; expected 0x1");
        }
        byte[] arrby2 = new byte[4];
        System.arraycopy(arrby, 1, arrby2, 0, 4);
        int n = Transport.bytesToInt(arrby2);
        byte by = arrby[5];
        byte by2 = arrby[6];
        return new ConnectMessage(n, by, by2);
    }

    public byte[] toBytes() throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        byteArrayOutputStream.write(1);
        byteArrayOutputStream.write(Transport.intToBytes(this.sourceId));
        byteArrayOutputStream.write(this.versionHi);
        byteArrayOutputStream.write(this.versionLo);
        return byteArrayOutputStream.toByteArray();
    }

    public int getSourceId() {
        return this.sourceId;
    }

    public byte getVersionHi() {
        return this.versionHi;
    }

    public byte getVersionLo() {
        return this.versionLo;
    }
}