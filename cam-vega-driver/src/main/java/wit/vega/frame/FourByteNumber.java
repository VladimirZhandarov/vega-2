package wit.vega.frame;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author vneverov
 */
public class FourByteNumber {

    public static final int SIZE = 4;
    private byte[] buf = new byte[SIZE];

    /**
     * Заполнить 0xFFFFFFFF
     */
    public FourByteNumber() {
        for (int i = 0; i < SIZE; i++) {
            buf[i] = (byte) 0xFF;
        }
    }

    public FourByteNumber(int n) {
        set(n);
    }

    public FourByteNumber(byte[] bb, int start) {
        int end = start + SIZE;
        for (int i = start, ii = 0; i < end; i++, ii++) {
            buf[ii] = bb[i];
        }
    }

    public FourByteNumber(InputStream ins) {
        try {
            ins.read(buf);
        } catch (IOException ex) {
            Logger.getLogger(FourByteNumber.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public byte[] getBytes() {
        return buf;
    }

    public int toInt() {
        ByteBuffer bb = ByteBuffer.wrap(buf);
        bb.order(ByteOrder.LITTLE_ENDIAN);
        return bb.getInt();
    }

    public void increase(int n) {
        int n2 = n + toInt();
        set(n2);
    }

    public void serialize(OutputStream ous) throws IOException {
        ous.write(buf);
    }

    public void deserialize(InputStream ins) throws IOException {
        ins.read(buf);
    }

    @Override
    public String toString() {
        return "FourByteNumber{" + toInt() + '}';
    }

    private void set(int n) {
        int i = 0;
        buf[i++] = (byte) n;
        buf[i++] = (byte) (n >> 8);
        buf[i++] = (byte) (n >> 16);
        buf[i++] = (byte) (n >> 24);
    }
}
