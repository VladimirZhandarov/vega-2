package wit.vegaproto;

import java.io.FileInputStream;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Calendar;
import java.util.Random;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import wit.vegaproto.net.Connection;
import wit.vegaproto.net.HttpConnection;
import wit.vegaproto.net.TcpConnection;
import wit.vegaproto.proto.DataMessage;
import wit.vegaproto.proto.Protocol;

public class App {

    private static final Logger logger = LoggerFactory.getLogger((Class) App.class);

    private static Connection createConnection(String hostString) {
        URL uRL;
        boolean bl = false;
        if (hostString.startsWith("tcp")) {
            bl = true;
            hostString = hostString.replace("tcp", "http");
        }
        try {
            uRL = new URL(hostString);
        } catch (MalformedURLException var4_4) {
            logger.error("invalid url provided as argument; error:", (Throwable) var4_4);
            return null;
        }
        if (bl) {
            logger.debug("using TCP on {}:{}", (Object) uRL.getHost(), (Object) uRL.getPort());
            return new TcpConnection(uRL.getHost(), uRL.getPort());
        }
        if ("http".equals(uRL.getProtocol())) {
            logger.debug("using HTTP on {}", (Object) hostString);
            return new HttpConnection(hostString, 1000);
        }
        logger.error("protocol {} not supported; use tcp://... or http://... URL format", (Object) uRL.getProtocol());
        return null;
    }

    public static void main(String[] arg) throws Exception {
        Connection connection = App.createConnection("http://0.0.0.0:8080");

        Protocol.Body body = Protocol.Body.getDefaultInstance();
        Protocol.Body.Builder builder = body.toBuilder();
        Protocol.Camera.Builder cameraBuilder = Protocol.Camera.newBuilder();
        cameraBuilder.setId(1);
        cameraBuilder.setChannel(0);
        builder.setTime(App.buildDateAndTime());
        builder.setCamera(cameraBuilder);
        
        body = builder.build();
        Random random = new Random();
        DataMessage dataMessage = new DataMessage(239, random.nextInt(), body);
        connection.transmit(dataMessage);
        connection.close();
    }

    private static Protocol.DateAndTime.Builder buildDateAndTime() {
        Calendar calendar = Calendar.getInstance();
        return Protocol.DateAndTime.newBuilder().setYearUTC(calendar.get(1)).setMonthUTC(calendar.get(2) + 1).setDayUTC(calendar.get(5)).setHourUTC(calendar.get(11)).setMinuteUTC(calendar.get(12)).setSecondUTC(calendar.get(13)).setMillisecondUTC(calendar.get(14)).setOffsetFromUTCMinutes(180);
    }
}
