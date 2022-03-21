package wit.vegaproxy;

import com.google.protobuf.ByteString;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.MalformedURLException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.util.Calendar;
import java.util.Properties;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.TimeZone;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;
import org.apache.commons.io.IOUtils;
import wit.vega.frame.MsgCamEvent;
import wit.vega.frame.ResponseFactory;
import wit.vega.frame.VegaCmd;
import wit.vega.frame.VegaConsts;
import wit.vega.frame.VegaDataTag;
import wit.vegaproto.net.Connection;
import wit.vegaproto.net.HttpConnection;
import wit.vegaproto.net.TcpConnection;
import wit.vegaproto.proto.DataMessage;
import wit.vegaproto.proto.Protocol;

public class App {

    private static Logger LOG = Logger.getLogger(App.class.getName());
    private static FileHandler logFileHandler;

    private static final String propertiesFileName = "/opt/cerberus/vega-proxy.properties";

    private static final String HOST_URL_KEY = "HostURL";
    private static final String HOST_CONNECTION_TIMEOUT_KEY = "HostTimeout";
    private static final String SOURCE_ID_KEY = "SourceId";
    private static final String CAMERA_ID_KEY = "CameraId";
    private static final String CAMERA_CHANNEL_KEY = "CameraChannel";
    private static final String CAMERA_LONGITUDE_KEY = "CameraLongitude";
    private static final String CAMERA_LATITUDE_KEY = "CameraLatitude";
    private static final String CAMERA_DIRECTION_KEY = "CameraDirection";
    private static final String CAR_DIRECTION_IN_FRAME = "CarDirectionInFrame";
    private static final String VEGA_PORT_KEY = "VegaPort";

    private static Properties props = new Properties();
    private static int messageCount = 0;

    private static Connection createConnection(String hostString, int timeout) {
        URL uRL;
        boolean bl = false;
        if (hostString.startsWith("tcp")) {
            bl = true;
            hostString = hostString.replace("tcp", "http");
        }
        try {
            uRL = new URL(hostString);
        } catch (MalformedURLException var4_4) {
            //LOG.info("invalid url provided as argument; error:" + var4_4.toString());
            return null;
        }
        if (bl) {
            //LOG.info("using TCP on " + uRL.getHost() + ":" + uRL.getPort());
            return new TcpConnection(uRL.getHost(), uRL.getPort());
        }
        if ("http".equals(uRL.getProtocol())) {
            //.info("using HTTP on " + hostString);
            return new HttpConnection(hostString, timeout);
        }
        LOG.info("protocol " + uRL.getProtocol() + " not supported; use tcp://... or http://... URL format");
        return null;
    }

    public static void main(String[] arg) throws Exception {
        logFileHandler = new FileHandler("/opt/cerberus/vega-proxy.log");
        LOG.addHandler(logFileHandler);
        SimpleFormatter formatter = new SimpleFormatter();
        logFileHandler.setFormatter(formatter);

        // the following statement is used to log any messages  
        LOG.info("Vega itrium proxy log");

        File propertyFile = new File(propertiesFileName);
        if (!propertyFile.exists()) {
            propertyFile.createNewFile();
        }
        InputStream inputStreamProperties = new FileInputStream(propertyFile);
        try {
            if (inputStreamProperties != null) {
                props.load(inputStreamProperties);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        if (props.isEmpty()) {
            try {
                props.setProperty(HOST_URL_KEY, "http://0.0.0.0:8080");
                props.setProperty(HOST_CONNECTION_TIMEOUT_KEY, Integer.toString(1000));
                props.setProperty(SOURCE_ID_KEY, Integer.toString(0));
                props.setProperty(CAMERA_ID_KEY, Integer.toString(1));
                props.setProperty(CAMERA_CHANNEL_KEY, Integer.toString(1));
                props.setProperty(CAMERA_LONGITUDE_KEY, Integer.toString(-181));
                props.setProperty(CAMERA_LATITUDE_KEY, Integer.toString(-91));
                props.setProperty(CAMERA_DIRECTION_KEY, Integer.toString(-1));
                props.setProperty(CAR_DIRECTION_IN_FRAME, Integer.toString(270));
                props.setProperty(VEGA_PORT_KEY, Integer.toString(32002));
                OutputStream out = new FileOutputStream(propertyFile);
                props.store(out, "Vega proxy properties");
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        int serverPort = Integer.valueOf(props.getProperty(VEGA_PORT_KEY, Integer.toString(-1)));
        if (serverPort <= 0) {
            LOG.info("serverPort not setted");
        }
        ServerSocket serverSocket = new ServerSocket(serverPort);
        LOG.info("start server at port " + Integer.toString(serverPort));
        while (true) {
            Socket socket = serverSocket.accept();
            InputStream bis = null;
            try {
                byte[] buf = IOUtils.toByteArray(socket.getInputStream());
                if (buf.length > 0) {
                    bis = new ByteArrayInputStream(buf);
                    VegaCmd respCmd = ResponseFactory.deserialize(bis);
                    if (respCmd instanceof MsgCamEvent) {
                        MsgCamEvent evn = (MsgCamEvent) respCmd;
                        String plateNum = evn.getPlateNum();
                        String direction = evn.getDirection();
                        byte[] photo = evn.getBWPhoto();
                        byte[] platePhoto = evn.getColPhoto();
                        boolean plateRectExist = false;
                        int plateMinX = evn.getPlateMinX();
                        int plateMinY = evn.getPlateMinY();
                        int plateMaxX = evn.getPlateMaxX();
                        int plateMaxY = evn.getPlateMaxY();
                        if (plateMinX < plateMaxX && plateMinY < plateMaxY) {
                            plateRectExist = true;
                        }
                        Date date = evn.getDate();
                        Date time = evn.getTime();
                        Protocol.DateAndTime.Builder dateAndTimeBuilder = Protocol.DateAndTime.newBuilder();
                        Calendar calendar = Calendar.getInstance();
                        if (date != null && time != null) {
                            calendar.set(date.getYear(), date.getMonth(), date.getDate(), time.getHours(), time.getMinutes(), time.getSeconds());
                        }
                        Calendar utcTime = new GregorianCalendar(TimeZone.getTimeZone("GMT"));
                        utcTime.setTimeInMillis(calendar.getTimeInMillis());
                        dateAndTimeBuilder.setYearUTC(utcTime.get(Calendar.YEAR)).
                                setMonthUTC(utcTime.get(Calendar.MONTH) + 1).
                                setDayUTC(utcTime.get(Calendar.DAY_OF_MONTH)).
                                setHourUTC(utcTime.get(Calendar.HOUR_OF_DAY)).
                                setMinuteUTC(utcTime.get(Calendar.MINUTE)).
                                setSecondUTC(utcTime.get(Calendar.SECOND)).
                                setMillisecondUTC(utcTime.get(Calendar.MILLISECOND)).
                                setOffsetFromUTCMinutes(TimeZone.getDefault().getRawOffset() / 1000 / 60);
                        String rectString = "rect is null";
                        if (plateRectExist) {
                            rectString = "rect(" + Integer.toString(plateMinX) +
                                    ", " + Integer.toString(plateMinY) + ", " +
                                    Integer.toString(plateMaxX - plateMinX) + ", " +
                                    Integer.toString(plateMaxY - plateMinY) + ")";
                        }
                        String platePhotoString = "plate photo NOT EXIST";
                        if(platePhoto != null) {
                            platePhotoString = "plate photo EXIST";
                        }
                        LOG.info("receive " + plateNum + " " + rectString + " " + platePhotoString);
                        
                        Protocol.Body.Builder builder = Protocol.Body.newBuilder();
                        builder.setCamera(createCamera(props));
                        builder.setTime(dateAndTimeBuilder);

                        Protocol.Plate.Builder plateBuilder = Protocol.Plate.newBuilder();
                        plateBuilder.setPlateText(plateNum);
                        plateBuilder.setPlateType(plateNum.length() - 6); //TODO fix
                        builder.setPlate(plateBuilder);

                        Protocol.FixationFrame.Builder fixationFrameBuilder = Protocol.FixationFrame.newBuilder();
                        fixationFrameBuilder.setNum(1);
                        fixationFrameBuilder.setIsBest(true);
                        if (photo != null) {
                            Protocol.Photo.Builder photoBuilder = Protocol.Photo.newBuilder();
                            photoBuilder.setType(Protocol.PhotoTypes.PhotoTypeVehicle);

                            Protocol.Image.Builder imageBuilder = Protocol.Image.newBuilder();
                            imageBuilder.setData(ByteString.copyFrom(photo));
                            imageBuilder.setType(Protocol.ImageContainerTypes.ImageContainerTypeJPG);

                            photoBuilder.setImage(imageBuilder);
                            fixationFrameBuilder.setVehicle(photoBuilder);
                        }
 
                        if (platePhoto != null || plateRectExist) {
                            Protocol.Photo.Builder photoBuilder = Protocol.Photo.newBuilder();
                            photoBuilder.setType(Protocol.PhotoTypes.PhotoTypePlate);
                            if(platePhoto != null) {
                                Protocol.Image.Builder imageBuilder = Protocol.Image.newBuilder();
                                imageBuilder.setData(ByteString.copyFrom(platePhoto));
                                imageBuilder.setType(Protocol.ImageContainerTypes.ImageContainerTypeJPG);
                                photoBuilder.setImage(imageBuilder);
                            }
                            if(plateRectExist) {
                                Protocol.Rectangle.Builder rectangleBuilder = Protocol.Rectangle.newBuilder();
                                rectangleBuilder.setTop(plateMinY);
                                rectangleBuilder.setLeft(plateMinX);
                                rectangleBuilder.setWidth(plateMaxX - plateMinX);
                                rectangleBuilder.setHeight(plateMaxY - plateMinY);
                                photoBuilder.setArea(rectangleBuilder);
                            }
                            fixationFrameBuilder.setPlate(photoBuilder);
                        }
                        builder.addFixationFrames(fixationFrameBuilder);

                        Protocol.AdditionalParams.Builder additionalParamsBuilder = Protocol.AdditionalParams.newBuilder();
                        
                        int carDirectionInFrame = Integer.valueOf(props.getProperty(CAR_DIRECTION_IN_FRAME, Integer.toString(270)));
                        
                        if(direction.equals("DIRECTION_GOAWAY_VALUE")) {
                            carDirectionInFrame = 0;
                        } else if(direction.equals("DIRECTION_APROACH_VALUE")) {
                            carDirectionInFrame = 180;
                        }
                        if (carDirectionInFrame >= 0 && carDirectionInFrame < 360) {
                            additionalParamsBuilder.setDirInFrame(carDirectionInFrame);
                        }
                        builder.setAdditionalVehicleParams(additionalParamsBuilder);
                        
                        DataMessage dataMessage = new DataMessage(Integer.valueOf(props.getProperty(SOURCE_ID_KEY,
                                Integer.toString(0))), messageCount, builder.build());

                        Connection connection = App.createConnection(props.getProperty(HOST_URL_KEY),
                                Integer.valueOf(props.getProperty(HOST_CONNECTION_TIMEOUT_KEY, Integer.toString(1000))));
                        connection.transmit(dataMessage);
                        connection.close();

                        messageCount++;
                    }
                }
            } catch (Exception e) {
                LOG.info("[TcpListener.run]: " + e.toString());
            } finally {
                try {
                    if (bis != null) {
                        bis.close();
                    }
                    socket.close();
                } catch (IOException ex) {
                    LOG.info("[run] close: " + ex.toString());
                }
            }
        }
    }

    private static Protocol.Camera createCamera(Properties props) {
        Protocol.Camera.Builder cameraBuilder = Protocol.Camera.newBuilder();
        cameraBuilder.setId(Integer.valueOf(props.getProperty(CAMERA_ID_KEY, Integer.toString(1))));
        cameraBuilder.setChannel(Integer.valueOf(props.getProperty(CAMERA_CHANNEL_KEY, Integer.toString(1))));
        int longitude = Integer.valueOf(props.getProperty(CAMERA_LONGITUDE_KEY, Integer.toString(-181)));
        int latitude = Integer.valueOf(props.getProperty(CAMERA_LATITUDE_KEY, Integer.toString(-91)));
        if (longitude >= -180 && longitude <= 180 && latitude >= -90 && latitude <= 90) {
            Protocol.Coordinate.Builder coordinateBuilder = Protocol.Coordinate.newBuilder();
            coordinateBuilder.setLongitude(longitude);
            coordinateBuilder.setLatitude(latitude);
            int direction = Integer.valueOf(props.getProperty(CAMERA_DIRECTION_KEY, Integer.toString(-1)));
            if (direction >= 0 && direction < 360) {
                coordinateBuilder.setDirection(direction);
            }
            cameraBuilder.setLocation(coordinateBuilder);
        }
        return cameraBuilder.build();
    }
}
