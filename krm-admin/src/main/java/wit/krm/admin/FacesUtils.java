package wit.krm.admin;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.faces.application.FacesMessage;
import javax.faces.context.ExternalContext;
import javax.faces.context.FacesContext;
import org.primefaces.context.RequestContext;

/**
 *
 * @author vneverov
 */
public class FacesUtils {

    private static final Logger LOG = Logger.getLogger(FacesUtils.class.getName());

    public static void showOkMsg(Translator tr, String msg) {
        FacesContext.getCurrentInstance().addMessage(null, new FacesMessage(FacesMessage.SEVERITY_INFO, tr.tr("Операция выполнена"), msg == null ? "" : msg));
    }

    public static void showFailMsg(Translator tr, String msg) {
        FacesContext.getCurrentInstance().addMessage(null, new FacesMessage(FacesMessage.SEVERITY_ERROR, tr.tr("Ошибка"), msg));
    }

    public static void showFailMsg(Translator tr, Exception ex) {
        if (ex != null) {
            showFailMsg(tr, ex.toString());
        }
    }

    /**
     * Данный вызов сбрасывает язык.
     * Использовать VersionBeanBase#invalidateSession().
     */
    public static void invalidateSession() {
        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        if (ec != null) {
            ec.invalidateSession();
        }
    }

    public static void redirect(String url) {
        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        if (ec != null) {
            try {
                ec.redirect(url);
            } catch (IOException ex) {
                LOG.log(Level.SEVERE, "[FacesUtils.redirect] ", ex);
            }
        }
    }

    public static void setSessionVar(String name, Object o) {
        if (name != null && !name.isEmpty()) {
            FacesContext fc = FacesContext.getCurrentInstance();
            if (fc != null) {
                ExternalContext ec = fc.getExternalContext();
                if (ec != null) {
                    if (o != null) {
                        ec.getSessionMap().put(name, o);
                    } else {
                        ec.getSessionMap().remove(o);
                    }
                }
            }
        }
    }

    public static Object getSessionVar(String name) {
        if (name != null && !name.isEmpty()) {
            FacesContext fc = FacesContext.getCurrentInstance();
            if (fc != null) {
                ExternalContext ec = fc.getExternalContext();
                if (ec != null) {
                    return ec.getSessionMap().get(name);
                }
            }
        }
        return null;
    }

    public static void refreshCtrl(String ctrlId) {
//        PartialViewContext pvc = FacesContext.getCurrentInstance().getPartialViewContext();
//        if (pvc != null) {
//            pvc.getRenderIds().add(ctrlId);
//        }

        RequestContext context = RequestContext.getCurrentInstance();
        context.update(ctrlId);
    }
    
    public static String getReferer() {
        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        if (ec != null) {
            String referer = ec.getRequestHeaderMap().get("referer");
            return referer;
        }
        return null;
    }
    
    /**
     * Начало URL, типа https://localhost:8181
     *
     * @return
     */
    public static String getUrlServer2() {
        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
        if (ec != null) {
            return ec.getRequestScheme() + "://" + ec.getRequestServerName() + ":" + ec.getRequestServerPort();
        }
        LOG.severe("[getUrlServer] ExternalContext == null");
        return null;
    }

    /**
     * URL модуля, типа https://localhost:8181/ticket-client
     *
     * @return
     */
    public static String getUrlModuleRoot2() {
        return getUrlServer2() + getUrlDocRoot2();
    }

    /**
     * типа ticket-client
     *
     * @return
     */
    public static String getUrlDocRoot2() {
        return FacesContext.getCurrentInstance().getExternalContext().getRequestContextPath();
    }
    
}
