/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.proto;

public class Constants {
    public static final byte DATA_MESSAGE_CODE = 4;
    public static final int DATA_SIZE_OFFSET = 1;
    public static final int DATA_HEADER_LENGTH = 13;
    public static final int INT_LENGTH = 4;
    public static final int DATA_SOURCE_ID_OFFSET = 5;
    public static final int DATA_MESSAGE_ID_OFFSET = 9;
    static final byte[] EMPTY_INT = new byte[]{0, 0, 0, 0};
    public static final byte DATA_RESPONSE_MESSAGE_CODE = 5;
    public static final int DATA_RESPONSE_LENGTH = 9;
    public static final byte DATA_RESPONSE_SIZE_OFFSET = 1;
    public static final byte DATA_RESPONSE_MESSAGE_ID_OFFSET = 5;
    public static final byte CONNECT_MESSAGE_CODE = 1;
    public static final int CONNECT_LENGTH = 7;
    public static final int CONNECT_SOURCE_ID_OFFSET = 1;
    public static final int CONNECT_VERSION_HI_OFFSET = 5;
    public static final int CONNECT_VERSION_LO_OFFSET = 6;
    public static final byte CONNECT_CONFIRMATION_MESSAGE_CODE = 2;
    public static final byte CONNECT_ERROR_MESSAGE_CODE = 3;
}
