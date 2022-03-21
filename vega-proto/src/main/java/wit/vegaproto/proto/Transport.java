/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.proto;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class Transport {

    public static byte[] intToBytes(int n) {
        return ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(n).array();
    }

    public static int bytesToInt(byte[] arrby) {
        return ByteBuffer.wrap(arrby).order(ByteOrder.LITTLE_ENDIAN).getInt();
    }

}
