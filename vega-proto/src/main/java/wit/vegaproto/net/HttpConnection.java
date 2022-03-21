/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vegaproto.net;

import java.io.InputStream;
import org.apache.commons.io.IOUtils;
import org.apache.http.HttpEntity;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.impl.client.HttpClients;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import wit.vegaproto.proto.DataMessage;
import wit.vegaproto.proto.ResponseDataMessage;

public class HttpConnection implements Connection {
    private static final Logger logger = LoggerFactory.getLogger((Class) HttpConnection.class);
    private String url;
    private CloseableHttpClient closeableHttpClient;
    
    public HttpConnection(String string, int timeout) {
        this.url = string;
        RequestConfig config = RequestConfig.custom().setConnectTimeout(timeout).setConnectionRequestTimeout(timeout).setSocketTimeout(timeout).build();
        closeableHttpClient = HttpClientBuilder.create().setDefaultRequestConfig(config).build();
    }

    @Override
    public ResponseDataMessage transmit(DataMessage dataMessage) throws Exception {
        byte[] arrby = dataMessage.toBytes();
        logger.debug("sending {} bytes to {}", (Object)arrby.length, (Object)this.url);
        
        HttpPost httpPost = new HttpPost(this.url);
        httpPost.setEntity((HttpEntity)new ByteArrayEntity(arrby));
        httpPost.setHeader("Content-Type", "application/x-cvss-vddp");

        CloseableHttpResponse closeableHttpResponse = closeableHttpClient.execute((HttpUriRequest)httpPost);
        byte[] arrby2 = IOUtils.toByteArray((InputStream)closeableHttpResponse.getEntity().getContent());
        logger.debug("received response of size={} from server", (Object)arrby2.length);
        ResponseDataMessage responseDataMessage = ResponseDataMessage.fromBytes((byte[])arrby2);
        closeableHttpResponse.close();
        return responseDataMessage;
    }

    @Override
    public void close() throws Exception {
        closeableHttpClient.close();
    }
}