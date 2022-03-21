/*
 * GrdAM.java
 *
 * Created on 23 Ноябрь 2007 г., 1:06
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Режимы использования GSII64 в Guardant Stealth
 * <br/>- bit 0-5 - режим использования
 * <br/>- bit 7 - шифрование/расшифрование
 * <br/>- bit 8-9 - позиция шифруемого блока
 */
public class GrdAM {

    /**
     * Electronic Code Book
     */
    public static final GrdAM ECB = new GrdAM(0);
    /**
     * Cipher Block Chaining
     */
    public static final GrdAM CBC = new GrdAM(1);
    /**
     * Cipher Feed Back
     */
    public static final GrdAM CFB = new GrdAM(2);
    /**
     * output Feed Back
     */
    public static final GrdAM OFB = new GrdAM(3);
    /**
     * Encode mode
     */
    public static final GrdAM Encode = new GrdAM(0);
    /**
     * Decode mode
     */
    public static final GrdAM Decode = new GrdAM(0x80);
    /**
     * Synonym definition
     */
    public static final GrdAM Encrypt = new GrdAM(Encode.mValue);
    /**
     * Synonym definition
     */
    public static final GrdAM Decrypt = new GrdAM(Decode.mValue);

    int mValue;

    GrdAM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }
}
