package wit.krm.admin.bean;

import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.annotation.WebListener;
import wit.krm.admin.KrmCtrl;
import wit.krm.admin.KrmException;
import wit.krm.admin.KrmProp;

/**
 * Синглетон состояния приложения.
 * 
 * @author vneverov
 */
//@ManagedBean(eager = true)
//@ApplicationScoped
@WebListener
public class AppSingleton implements ServletContextListener {
    
    private static final Logger LOG = Logger.getLogger(AppSingleton.class.getName());
    private static AppSingleton _this;
    private KrmProp prop;
    private String hostIp;
    private String hostName;

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        _this = this;
        prop = new KrmProp();
        try {
            prop.checkIfExists();
            prop.load();
        } catch (KrmException ex) {
            LOG.log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
    }
    
    public static AppSingleton getInstance() {
        return _this;
    }
    
    public KrmProp getKrmProp() {
        return prop;
    }

    public String getHostIp() {
        if (hostIp == null) {
            KrmCtrl ctrl = new KrmCtrl();
            hostIp = ctrl.getHostIp();
        }
        return hostIp;
    }

    public String getHostName() {
        if (hostName == null) {
            KrmCtrl ctrl = new KrmCtrl();
            hostName = ctrl.getHostName();
        }
        return hostName;
    }

//    public KrmCtrl getKrmCtrl() {
//        if (ctrl == null) {
//            ctrl = new KrmCtrl();
//        }
//        return ctrl;
//    }
}
