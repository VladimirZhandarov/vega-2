package wit.vega;

import java.io.IOException;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import org.apache.log4j.BasicConfigurator;
import org.apache.log4j.RollingFileAppender;
import org.apache.log4j.TTCCLayout;

public class App {

    private static final Logger LOG = Logger.getLogger(App.class.getName());
    private static final String LOG_FL = "vega.log";
    private static final String IP = "192.168.111.37";
    private static final int PORT = 31000;

//	public static BufferedImage toBufferedImage(final Image image) {
//		if (image instanceof BufferedImage) {
//			return (BufferedImage) image;
//		}
//		if (image instanceof VolatileImage) {
//			return ((VolatileImage) image).getSnapshot();
//		}
//		final BufferedImage buffImg = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_RGB);
//		final Graphics2D g2 = buffImg.createGraphics();
//		g2.drawImage(image, null, null);
//		g2.dispose();
//		return buffImg;
//	}
//
//	private void getSingleImgTest() {
//		VegaManager mng = new VegaManager(IP, PORT);
//		try {
//			Image img = mng.getImage(80, 640, 480);
//			if (img != null) {
//				BufferedImage bimg = toBufferedImage(img);
//				ImageIO.write(bimg, "jpg", new File("test.jpg"));
//			}
//		} catch (UnknownHostException ex) {
//			log.error(ex);
//		} catch (IOException ex) {
//			log.error(ex);
//		}
//	}
//
//    private void testGetDigState() {
//		VegaManager mng = new VegaManager(IP, PORT);
//		try {
//			Image img = mng.getImage(80, 640, 480);
//			if (img != null) {
//				BufferedImage bimg = toBufferedImage(img);
//				ImageIO.write(bimg, "jpg", new File("test.jpg"));
//			}
//		} catch (UnknownHostException ex) {
//			log.error(ex);
//		} catch (IOException ex) {
//			log.error(ex);
//		}
//    }
//	private void getListA() {
//		VegaManager mng = new VegaManager(IP, PORT);
//		try {
//			Set<String[]> lst = mng.getListA();
//			log.info("ListA:");
//			int i = 0;
//			for(String[] ss : lst) {
//				log.info(String.format("%d, %s, %s", i, ss[0], ss[1]));
//				i++;
//			}
//		} catch (UnknownHostException ex) {
//			log.error(ex);
//		} catch (IOException ex) {
//			log.error(ex);
//		}
//	}
//	private void startListenEvents() {
//		TcpServer srv = new TcpServer(32000);
//		srv.start();
//	}
//
    public static void main(String[] args) {
        try {
            TTCCLayout lay = new TTCCLayout("HH:mm:ss.SSS");
            lay.setThreadPrinting(false);
            BasicConfigurator.configure(new RollingFileAppender(lay, LOG_FL));
        } catch (IOException ex) {
            JOptionPane.showMessageDialog(null, String.format("Не могу запустить логгер, %s", ex.toString()));
            return;
        }

        LOG.info("Start test Vega");
        App app = new App();
//		app.getSingleImgTest();
//		app.startListenEvents();
//		app.getListA();
    }
}
