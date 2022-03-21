/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.krm.itrium.bridge;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import javax.ejb.Singleton;
import javax.ejb.Startup;

@Singleton
@Startup
public class StartupBean {

    public enum States {
        BEFORESTARTED, STARTED, PAUSED, SHUTTINGDOWN
    };
    private States state;
    private final String propertiesFileName = "krm-itrium-bridge.properties";
    
    TcpListenerLocal tcplistener = null;

    @PostConstruct
    public void initialize() {
        state = States.BEFORESTARTED;
        Properties props = new Properties();
        try {
            File catalinaBase = new File(System.getProperty("catalina.base")).getAbsoluteFile();
            File propertyFile = new File(catalinaBase, propertiesFileName);

            InputStream inputStreamProperties = new FileInputStream(propertyFile);
            if(inputStreamProperties != null) {
                props.load(inputStreamProperties);
            } else {
                props = createDefaultProperties();
            }
        } catch (IOException ex) {
            props = createDefaultProperties();
        }
        /*tcplistener = new TcpListener();
        try {
            tcplistener.setRunState(Long.valueOf(props.getProperty("CameraId", Integer.toString(1))),
                    Integer.valueOf(props.getProperty("KrmPortA", Integer.toString(32000))),
                    Integer.valueOf(props.getProperty("KrmPortB", Integer.toString(32001))),
                    Integer.valueOf(props.getProperty("KrmPortNA", Integer.toString(32002))),
                    Integer.valueOf(props.getProperty("KrmPortNB", Integer.toString(32003))));
        } catch (IOException ex) {
            Logger.getLogger(StartupBean.class.getName()).log(Level.SEVERE, null, ex);
        }*/
        
        state = States.STARTED;
        System.out.println("Service KRM Itrium Bridge Started");
    }
    
    public Properties createDefaultProperties() {
    try {
        Properties props = new Properties();
        props.setProperty("HostURL", "http://0.0.0.0:8080");
        props.setProperty("CameraId", Integer.toString(1));
        props.setProperty("CameraChannel", Integer.toString(1));
        props.setProperty("CameraLongitude", Integer.toString(-181));
        props.setProperty("CameraLatitude", Integer.toString(-91));
        props.setProperty("CameraDirection", Integer.toString(-1));
        props.setProperty("KrmPortA", Integer.toString(32000));
        props.setProperty("KrmPortB", Integer.toString(32001));
        props.setProperty("KrmPortNA", Integer.toString(32002));
        props.setProperty("KrmPortNB", Integer.toString(32003));
        
        File catalinaBase = new File(System.getProperty("catalina.base")).getAbsoluteFile();
        File propertyFile = new File(catalinaBase, propertiesFileName);
        OutputStream out = new FileOutputStream(propertyFile);
        props.store(out, "KRM Itrium properties");
        return props;
    }
    catch (Exception e ) {
        e.printStackTrace();
    }
    return new Properties();
}

    @PreDestroy
    public void terminate() {
        state = States.SHUTTINGDOWN;
        if(tcplistener != null)
        {
            try {
                tcplistener.setStopState();
            } catch (IOException ex) {
                Logger.getLogger(StartupBean.class.getName()).log(Level.SEVERE, null, ex);
            }
            tcplistener = null;
        }
                
        System.out.println("Service KRM Itrium Bridge shut down in progress");
    }

    public States getState() {
        return state;
    }

    public void setState(States state) {
        this.state = state;
    }
}
