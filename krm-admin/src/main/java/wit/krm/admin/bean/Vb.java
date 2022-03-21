package wit.krm.admin.bean;

import java.text.SimpleDateFormat;
import java.util.Date;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;

/**
 *
 * @author vneverov
 */
@ManagedBean
@RequestScoped
public class Vb {

    private static final String firmName = "© ViraSoft 2012-2016";
    private static final String programName = "KRM";
    private static final String programVers = "1.0.1";
    
    private final SimpleDateFormat sdf = new SimpleDateFormat("dd.MM.yyyy HH:mm:ss");
    private String now;
    /**
     * Creates a new instance of Vb
     */
    public Vb() {
    }

    public String getFirmName() {
        return firmName;
    }

    public String getProgramName() {
        return programName;
    }

    public String getProgramVers() {
        return programVers;
    }
    
    public String tr(String s) {
        return s;
    }
    
    public String getTime() {
        return sdf.format(new Date());
    }
}
