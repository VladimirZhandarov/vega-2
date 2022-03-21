/*
 * GrdECC160.java
 *
 * Created on 27 Апрель 2009 г., 12:11
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

public class GrdECC160 {

    /**
     * ECC160 public key size
     */
    public static final GrdECC160 PUBLIC_KEY_SIZE = new GrdECC160(40);
    /**
     * ECC160 private key size
     */
    public static final GrdECC160 PRIVATE_KEY_SIZE = new GrdECC160(20);
    /**
     * ECC160 digest size
     */
    public static final GrdECC160 DIGEST_SIZE = new GrdECC160(40);
    /**
     * ECC160 message size
     */
    public static final GrdECC160 MESSAGE_SIZE = new GrdECC160(20);

    int mValue;

    GrdECC160(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
