package wit.krm.admin.bean;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;
import wit.krm.admin.FacesUtils;
import wit.krm.admin.Translator;

/**
 *
 * @author vneverov
 */
@ManagedBean
@RequestScoped
public class UserBean {

    public static final String SESVAR_ROLE = "role";
    private String pwd;

    public enum EnRole {

        /**
         * Админ.
         */
        ROLE_ADMIN,
        /**
         * Просто охранник.
         */
        ROLE_GUARD
    }

    /**
     * Creates a new instance of UserBean
     */
    public UserBean() {
    }

    public void checkRedirect() {
        Object o = FacesUtils.getSessionVar(UserBean.SESVAR_ROLE);
        if (o instanceof String) {
            try {
                int n = Integer.parseInt((String) o);
                if (n == UserBean.EnRole.ROLE_ADMIN.ordinal()) {
                    FacesUtils.redirect(FacesUtils.getUrlModuleRoot2() + "/faces/sec/main.xhtml");
                    return;
                }
            } catch (NumberFormatException e) {
            }
        }

        FacesUtils.redirect(FacesUtils.getUrlModuleRoot2() + "/faces/sec/login.xhtml");
    }

    public void onLogin() {
        if (pwd != null && !pwd.isEmpty()) {
            if ("admin".equalsIgnoreCase(pwd)) {
                FacesUtils.setSessionVar(SESVAR_ROLE, String.valueOf(EnRole.ROLE_ADMIN.ordinal()));
                FacesUtils.redirect(FacesUtils.getUrlModuleRoot2() + "/faces/sec/main.xhtml");
                return;
            }
        }

        Translator tr = new Translator(null);
        FacesUtils.showFailMsg(tr, "Неизвестный пароль");
    }

    public void onLogout() {
        FacesUtils.invalidateSession();
        FacesUtils.redirect(FacesUtils.getUrlModuleRoot2() + "/");
    }

    public String getPwd() {
        return pwd;
    }

    public void setPwd(String pwd) {
        this.pwd = pwd;
    }
}
