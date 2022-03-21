package wit.krm.admin;

import java.util.logging.Logger;

/**
 * Локализация.
 *
 * @author vneverov
 */
public class Translator {

    private static final Logger LOG = Logger.getLogger(Translator.class.getName());
    //
    public static final String LANG_RU = "ru";
    public static final String LANG_UK = "en";
    public static final String LANG_DE = "de";
    //
    /**
     * Работаем из кабинета.
     */
    public static final String SES_ATR_CLIENT_ID = "clientId";
    public static final String SES_ATR_BACK_URL = "backUrl";
    public static final String SES_ATR_LANG = "lang";
    //
//    @EJB(mappedName = CentralRegistryRemote.ejbName)
//    protected CentralRegistryRemote centralReg;
//    //
//    @EJB(mappedName = AccountDaoRemote.ejbName)
//    private AccountDaoRemote accountDao;

//    private GenderVO[] genders;
    private String locale = LANG_RU;
//    private String lastLocale;
//    protected PersonVO loggedUser;
    //
    private final String moduleId;

    public Translator(String moduleId) {
        this.moduleId = moduleId;
    }

    /**
     * Имя под которым известен в центральном регистраторе.
     *
     * @return
     */
    public String getModuleId() {
        return moduleId;
    }

//    public String getCurrLocale() {
//        FacesContext fc = FacesContext.getCurrentInstance();
//        if (fc != null) {
//            ExternalContext ec = fc.getExternalContext();
//            if (ec != null) {
//                Map<String, String> prms = ec.getRequestParameterMap();
//                if (prms.containsKey(SES_ATR_LANG)) {
//                    // вытаскиваем локаль с URL
//                    String userLang = prms.get(SES_ATR_LANG);
//                    if (!locale.equals(userLang)) {
//                        locale = userLang;
//                        onChangeLocale();
//                    }
//                    updateSessionWithLocale();
//                } else {
////                  boolean isSetCook = false;
//                    Map<String, Object> sm = ec.getSessionMap();
//                    String atr = (String) sm.get(SES_ATR_LANG);
//                    if (atr != null) {
//                        if (!locale.equals(atr)) {
//                            locale = atr;
//                            onChangeLocale();
//                        }
//                    } else {
//                        // def user lang
//                        HttpServletRequest req = (HttpServletRequest) ec.getRequest();
//                        if (req != null) {
//                            Locale loc = req.getLocale();
//                            String userLang = loc.getLanguage();
//                            if (!locale.equals(userLang)) {
//                                locale = userLang;
//                                onChangeLocale();
//                            }
//                            updateSessionWithLocale();
//                        }
//                    }
//                }
//
////                if (atr == null) {
////                    Map<String, Object> cookies = ec.getRequestCookieMap();
////                    Object o = cookies.get(SES_ATR_LANG);
////                    if (o != null && o instanceof Cookie) {
////                        currLocale = ((Cookie) o).getValue();
////                    } else {
////                        currLocale = LANG_RU;
////                        isSetCook = true;
////                    }
////                } else {
////                    currLocale = atr;
////                    isSetCook = true;
////                }
////                sm.put(SES_ATR_LANG, currLocale);
////                if (isSetCook) {
////                    putCookie(SES_ATR_LANG, currLocale);
////                }
//                return locale;
//            }
////        }
//        }
//        return LANG_RU;
//    }

//    public void updateSessionWithLocale() {
//        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
//        if (ec != null) {
//            Map<String, Object> sm = ec.getSessionMap();
//            sm.put(SES_ATR_LANG, locale);
//        }
//    }

    /**
     * Строка из ресурсов для текущего модуля и локали.
     *
     * @param resName
     * @return
     */
    public String tr(String resName) {
        return resName; //tr(moduleId, resName, getCurrLocale());
    }

//    public String tr(String moduleId, String resName, String locale) {
//        return centralReg.getStringRes4module(moduleId, resName, locale);
//    }

//    public GenderVO[] getGenders(boolean isRequest) {
//        String loc = getCurrLocale();
//        if (genders == null || lastLocale == null || !lastLocale.equals(loc)) {
//            lastLocale = loc;
//            genders = centralReg.getGenders(isRequest, loc);
//        }
//        return genders;
//    }
//
//    public PersonVO getLoggedUser() {
//        if (loggedUser == null) {
//            Principal pr = FacesContext.getCurrentInstance().getExternalContext().getUserPrincipal();
//            if (pr != null) {
//                String usr = pr.getName();
//                loggedUser = accountDao.getAccount(usr, getCurrLocale());
//            }
//        }
//        return loggedUser;
//    }
//
//    /**
//     * Начало URL, типа https://localhost:8181
//     *
//     * @return
//     */
//    public static String getUrlServer2() {
//        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
//        if (ec != null) {
//            return ec.getRequestScheme() + "://" + ec.getRequestServerName() + ":" + ec.getRequestServerPort();
//        }
//        LOG.severe("[getUrlServer] ExternalContext == null");
//        return null;
//    }
//
//    public String getUrlServer() {
//        return getUrlServer2();
//    }
//
//    /**
//     * URL модуля, типа https://localhost:8181/ticket-client
//     *
//     * @return
//     */
//    public static String getUrlModuleRoot2() {
//        return getUrlServer2() + getUrlDocRoot2();
//    }
//
//    public String getUrlModuleRoot() {
//        return getUrlModuleRoot2();
//    }
//
//    /**
//     * типа ticket-client
//     *
//     * @return
//     */
//    public static String getUrlDocRoot2() {
//        return FacesContext.getCurrentInstance().getExternalContext().getRequestContextPath();
//    }
//
//    public String getUrlDocRoot() {
//        return getUrlDocRoot2();
//    }
//
//    /**
//     * пример https://localhost:8181/ticket-res
//     *
//     * @return
//     */
//    public static String getUrlResourceRoot2() {
//        return getUrlServer2() + RES_ROOT;
//    }
//
//    public String getUrlResourceRoot() {
//        return getUrlResourceRoot2();
//    }
//
//    /**
//     * пример https://localhost:8181/ticket-res/ticket-client/rus
//     *
//     * @param locale
//     * @return
//     */
//    public String getUrlModuleResourcesRoot(String locale) {
//        if (locale == null) {
//            locale = getCurrLocale();
//        }
//
//        return getUrlResourceRoot() + centralReg.getModule(getModuleId()).getModuleLastUrl() + "/" + locale;
//    }
//
//    public String getModuleResourcesRoot() {
//        return getUrlModuleResourcesRoot(getCurrLocale());
//    }
//
//    protected void onChangeLocale() {
//        genders = null;
//    }
//
//    protected String getLoggedFIO2(String groupName) {
//        if (groupName == null || groupName.isEmpty()) {
//            return null;
//        }
//
//        if (loggedUser == null) {
//            Principal pr = FacesContext.getCurrentInstance().getExternalContext().getUserPrincipal();
//            if (pr != null) {
//                String usr = pr.getName();
//                PersonVO persVo = accountDao.getAccount(usr, getCurrLocale());
//                if (persVo == null) {
//                    return null;
//                }
//                for (String groupid : persVo.getGroup()) {
//                    if (groupid.equals(groupName)) {
//                        loggedUser = persVo;
//                        break;
//                    }
//                }
//            }
//        }
//
//        if (loggedUser != null) {
//            StringBuilder sb = new StringBuilder(loggedUser.getSurnameName());
//            sb.append(" ");
//            sb.append(loggedUser.getFirstName());
//            sb.append(" ");
//            sb.append(loggedUser.getMiddleName());
//            return sb.toString();
//        }
//        return null;
//    }
//
//    protected void putCookie(String key, String val) {
//        ExternalContext ec = FacesContext.getCurrentInstance().getExternalContext();
//        if (ec != null) {
//            Cookie cook = null;
//            Map<String, Object> cookies = ec.getRequestCookieMap();
//            if (cookies != null) {
//                Object o = cookies.get(key);
//                if (o != null && o instanceof Cookie) {
//                    cook = (Cookie) o;
//                    cook.setValue(val);
//                    cook.setMaxAge(604800); // 7
//                }
//            }
//
//            if (cook == null) {
//                cook = new Cookie(key, val);
//                cook.setMaxAge(604800); // 7 days. Expire time. -1 = by end of current session, 0 = immediately expire it, otherwise just the lifetime in seconds.
//            }
//
//            HttpServletResponse resp = (HttpServletResponse) ec.getResponse();
//            resp.addCookie(cook);
//        }
//    }
}
