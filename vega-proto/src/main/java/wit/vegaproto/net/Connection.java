/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.net;

import wit.vegaproto.proto.DataMessage;
import wit.vegaproto.proto.ResponseDataMessage;

public interface Connection {
    public ResponseDataMessage transmit(DataMessage var1) throws Exception;

    public void close() throws Exception;
}