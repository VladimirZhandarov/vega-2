package wit.krm.admin;

import java.util.Properties;
import java.util.logging.Logger;

/**
 * Синглтон настроек вебки.
 * 
 * @author vneverov
 */
public class PrefDaoSingl {
    
    private static final Logger LOG = Logger.getLogger(PrefDaoSingl.class.getName());   
    
    private static final PrefDaoSingl _this;
    
    static {
        _this = new PrefDaoSingl();
    }
    
    public static PrefDaoSingl getInstance() {
        return _this;
    }

    public PrefDaoSingl() {
        init();
    }

    private void init() {
        Properties props = System.getProperties();
        for (Object o : props.keySet()) {
            if (o instanceof String) {
                String k = (String)o;
                String v = props.getProperty(k);
                LOG.info(k + "=" + v);
            }
        }
    }
    
    
}
