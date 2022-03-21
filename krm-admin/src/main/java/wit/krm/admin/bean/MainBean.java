package wit.krm.admin.bean;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import wit.krm.admin.FacesUtils;
import wit.krm.admin.IConstants;
import wit.krm.admin.KrmCtrl;
import wit.krm.admin.KrmException;
import wit.krm.admin.KrmProp;
import wit.krm.admin.Translator;
import wit.krm.admin.Utils;
import wit.krm.admin.vo.RecognizePropVO;

/**
 *
 * @author vneverov
 */
@ManagedBean
@SessionScoped
public class MainBean {

    private static final Logger LOG = Logger.getLogger(MainBean.class.getName());
    private static final String TXT_ERR = "Error";
    private static final String RTSP = "rtsp://";
    private List<RecognizePropVO> recognProps;
    private Date lastModRecognDate;
    private String ntpServer;
    private String timeYear;
    private String timeMonth;
    private String timeDay;
    private String timeHour;
    private String timeMin;
    private String timeSec;
    private Set<String> tmzs;
    private String tmz;
    private final Translator tr;
    private final KrmCtrl ctrl;

    public MainBean() {
        tr = new Translator(null);
        ctrl = new KrmCtrl();
    }

    public void checkRedirect() {
        Object o = FacesUtils.getSessionVar(UserBean.SESVAR_ROLE);
        if (o instanceof String) {
            try {
                int n = Integer.parseInt((String) o);
                if (n == UserBean.EnRole.ROLE_ADMIN.ordinal()) {
                    return;
                }
            } catch (NumberFormatException e) {
            }
        }

        FacesUtils.redirect(FacesUtils.getUrlModuleRoot2() + "/faces/sec/login.xhtml");
    }

    public void onTimeout() {
        try {
            String path = getLogOsDependentPath();
            File fl = new File(path);
            if (fl.exists() && fl.isFile()) {
                Date d = new Date(fl.lastModified());
                if (lastModRecognDate == null || lastModRecognDate.before(d)) {
                    lastModRecognDate = d;
                    FacesUtils.refreshCtrl("formLog:pnlLog");
                }
            }
        } catch (KrmException ex) {
        }
    }

    private String getLogOsDependentPath() throws KrmException {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            return IConstants.KRM_PATH_LINUX + "/" + IConstants.KRM_LOG;
        } else if (Utils.getOs() == Utils.EnOS.OS_WND) {
            return IConstants.KRM_PATH_WIN + "/" + IConstants.KRM_LOG;
        }
        throw new KrmException("[MainBean#getLogOsDependentPath] unknown OS");
    }

    public String getLog() {
        String path;
        try {
            path = getLogOsDependentPath();
        } catch (KrmException ex) {
            LOG.log(Level.SEVERE, null, ex);
            return "Error";
        }

        File fl = new File(path);
        if (fl.exists() && fl.isFile()) {
            Date d = new Date(fl.lastModified());
            try {
                FileInputStream fis = new FileInputStream(fl);
                byte[] bb = new byte[1024];
                int l = fis.read(bb);
                if (l > 0) {
                    String s = new String(bb, 0, l);
                    int i = s.lastIndexOf("\n");
                    return s.substring(0, i);
                }
            } catch (IOException ex) {
                LOG.log(Level.SEVERE, null, ex);
                return TXT_ERR;
            }
        }

        return "";
    }

    public List<RecognizePropVO> getRecognProps() {
        if (recognProps == null) {
            final int propLength = IConstants.EnKrmProp.PROP_END.ordinal();
            recognProps = new ArrayList<RecognizePropVO>(propLength);
            KrmProp kp = AppSingleton.getInstance().getKrmProp();
            try {
                kp.load();
            } catch (KrmException ex) {
                LOG.log(Level.SEVERE, null, ex);
            }

            for (int i = 0; i < propLength; i++) {
                IConstants.EnKrmProp prop = IConstants.EnKrmProp.values()[i];
                String descr = kp.getDescrByKey(prop);
                String val;
                if (IConstants.krmPropType[prop.ordinal()]) {
                    // String
                    val = kp.getStrByKey(prop);
                } else {
                    // Integer
                    val = String.valueOf(kp.getIntByKey(prop));
                }

                RecognizePropVO vo = new RecognizePropVO(prop.name(), descr, val);
                recognProps.add(vo);
            }
        }
        return recognProps;
    }

    public void onOkRecognProp() {
        if (recognProps != null) {
            KrmProp prop = AppSingleton.getInstance().getKrmProp();
            boolean isDirty = false;
            for (RecognizePropVO vo : recognProps) {
                if (!isDirty && vo.getIsDirty()) {
                    isDirty = true;
                }

                if (vo.getIsDirty()) {
                    vo.setIsDirty(false);
                    IConstants.EnKrmProp en = IConstants.EnKrmProp.valueOf(vo.getKey());
                    boolean isStr = IConstants.krmPropType[en.ordinal()];
                    if (isStr) {
                        prop.setStrByKey(en, vo.getVal());
                    } else {
                        try {
                            int n = Integer.parseInt(vo.getVal());
                            prop.setIntByKey(en, n);
                        } catch (NumberFormatException e) {
                            FacesUtils.showFailMsg(tr, e);
                            return;
                        }
                    }
                }
            }

            if (isDirty) {
                try {
                    prop.save();
                } catch (KrmException ex) {
                    FacesUtils.showFailMsg(tr, ex);
                }
            }

            FacesUtils.showOkMsg(tr, null);
        }
    }

    public void onCancelRecognProp() {
        recognProps = null;
        FacesUtils.refreshCtrl("formCenter:tabView1:tableRecogn");
    }

    public String getRtsp() {
        return AppSingleton.getInstance().getKrmProp().getIP_CAMERA_URL();
    }

    /**
     * Совет пользователю об использовании RTSP.
     *
     * @return
     */
    public String getRtsp2() {
        String s = AppSingleton.getInstance().getKrmProp().getIP_CAMERA_URL();
        if (s != null) {
            int n = s.indexOf(RTSP);
            if (n >= 0) {
                int n2 = s.indexOf("/", n + RTSP.length());
                String s2 = s.substring(RTSP.length(), n2);

                int n3 = s2.indexOf(":");
                String s3 = null;
                if (n3 >= 0) {
                    s3 = s2.substring(n3);
                }

                StringBuilder sb = new StringBuilder();
                sb.append(s.substring(n, n + RTSP.length()));

                sb.append(AppSingleton.getInstance().getHostIp());
                if (s3 != null) {
                    sb.append(s3);
                }

                sb.append(s.substring(n2));
                return sb.toString();
            }
        }
        return TXT_ERR;
    }

    public String getHostInfo() {
        AppSingleton app = AppSingleton.getInstance();
        String s = app.getHostName() + " (" + app.getHostIp() + ")";
        return s;
    }

    public String getNtpServer() {
        if (ntpServer == null) {
            ntpServer = AppSingleton.getInstance().getKrmProp().getNTP_SERVER();
        }
        return ntpServer;
    }

    public void setNtpServer(String ntpServer) {
        this.ntpServer = ntpServer;
    }

    public Set<String> getTmzs() {
        if (tmzs == null) {
            try {
                tmzs = ctrl.getTimezones();
                tmz = ctrl.getCurrTimezone();
            } catch (KrmException ex) {
                LOG.log(Level.SEVERE, "[getTmzs]", ex);
            }
        }
        return tmzs;
    }

    public String getTmz() {
        return tmz;
    }

    public void setTmz(String tmz) {
        this.tmz = tmz;
    }

    public void onNtpOk() {
        if (ntpServer != null && !ntpServer.isEmpty()) {
            AppSingleton.getInstance().getKrmProp().setNTP_SERVER(ntpServer);
            try {
                AppSingleton.getInstance().getKrmProp().save();
                FacesUtils.showOkMsg(tr, null);
            } catch (KrmException ex) {
                LOG.log(Level.SEVERE, null, ex);
                FacesUtils.showFailMsg(tr, ex);
            }
        } else {
            FacesUtils.showFailMsg(tr, "Ошибка в определении сервера");
        }
    }

    public void onTmzOk() {
        if (tmz != null && !tmz.isEmpty()) {
            try {
                String currTmz = ctrl.getCurrTimezone();
                if (!tmz.equals(currTmz)) {
                    ctrl.setTimezone(tmz);
                    FacesUtils.showOkMsg(tr, null);
                }
            } catch (KrmException ex) {
                LOG.log(Level.SEVERE, null, ex);
                FacesUtils.showFailMsg(tr, ex);
            }
        }
    }

    public void onNtpSync() {
        try {
            String s = ctrl.syncNtpServer(ntpServer);
            FacesUtils.showOkMsg(tr, s);
        } catch (KrmException ex) {
            LOG.log(Level.SEVERE, null, ex);
            FacesUtils.showFailMsg(tr, ex);
        }
    }

    public String getTimeYear() {
        return timeYear;
    }

    public void setTimeYear(String timeYear) {
        this.timeYear = timeYear;
    }

    public String getTimeMonth() {
        return timeMonth;
    }

    public void setTimeMonth(String timeMonth) {
        this.timeMonth = timeMonth;
    }

    public String getTimeDay() {
        return timeDay;
    }

    public void setTimeDay(String timeDay) {
        this.timeDay = timeDay;
    }

    public String getTimeHour() {
        return timeHour;
    }

    public void setTimeHour(String timeHour) {
        this.timeHour = timeHour;
    }

    public String getTimeMin() {
        return timeMin;
    }

    public void setTimeMin(String timeMin) {
        this.timeMin = timeMin;
    }

    public String getTimeSec() {
        return timeSec;
    }

    public void setTimeSec(String timeSec) {
        this.timeSec = timeSec;
    }

    public void onTimeOk() {
        int ntimeYear = str2int(timeYear);
        if (ntimeYear < 2016 || ntimeYear > 3000) {
            FacesUtils.showFailMsg(tr, "Ошибка задания года");
            return;
        }

        int ntimeMonth = str2int(timeMonth);
        if (ntimeMonth < 1 || ntimeMonth > 12) {
            FacesUtils.showFailMsg(tr, "Ошибка задания месяца");
            return;
        }

        int ntimeDay = str2int(timeDay);
        if (ntimeDay < 1 || ntimeDay > 31) {
            FacesUtils.showFailMsg(tr, "Ошибка задания дня");
            return;
        }

        int ntimeHour = str2int(timeHour);
        if (ntimeHour < 0 || ntimeHour > 59) {
            FacesUtils.showFailMsg(tr, "Ошибка задания часа");
            return;
        }

        int ntimeMin = str2int(timeMin);
        if (ntimeMin < 0 || ntimeMin > 59) {
            FacesUtils.showFailMsg(tr, "Ошибка задания минут");
            return;
        }

        int ntimeSec = str2int(timeSec);
        if (ntimeSec < 0 || ntimeSec > 59) {
            FacesUtils.showFailMsg(tr, "Ошибка задания секунд");
            return;
        }

        try {
            ctrl.setDate(ntimeYear, ntimeMonth, ntimeDay, ntimeHour, ntimeMin, ntimeSec);
        } catch (KrmException ex) {
            LOG.log(Level.SEVERE, null, ex);
            FacesUtils.showFailMsg(tr, "Ошибка задания даты/времени: " + ex.getMessage());
            return;
        }
        FacesUtils.showOkMsg(tr, "Время успешно изменено");
    }

    private int str2int(String s) {
        try {
            return Integer.parseInt(s);
        } catch (NumberFormatException e) {
        }
        return -1;
    }
}
