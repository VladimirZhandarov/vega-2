package wit.krm.admin;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Date;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author vneverov
 */
public class KrmProp {

    private static final Logger LOG = Logger.getLogger(KrmProp.class.getName());
    private final Properties props = new Properties();
    private final Translator tr = new Translator(null);

//    private String[] SS = {
//        "533CA3793850787DED8C8ED7F6BEA657421678644B99D6E9EE8E9BE7AD19A858", // 2016-05-31, BFEBFBFF000106E5 мой CPU
//    };
    public void load() throws KrmException {
        // lic
        if (new Date().before(new Date(116, 11, 30))) { // 30.12.16
            checkIfExists();
            String flPath = getOsDependentPath();
            try {
                FileInputStream fis = new FileInputStream(flPath);
                props.load(fis);
            } catch (Exception ex) {
                throw new KrmException(ex);
            }
        } else {
            LOG.info("lic null");
        }
    }

    public void save() throws KrmException {
        String flPath = getOsDependentPath();
        File fl = new File(flPath);
        if (fl.exists() && fl.isFile()) {
            fl.renameTo(new File(flPath + ".bak"));
        }

        FileOutputStream fous = null;
        try {
            ByteArrayOutputStream baous = new ByteArrayOutputStream();
            init();
            props.store(baous, null);

            // replace '\:'
            byte[] bb = baous.toByteArray();
            String s = new String(bb);
            String s2 = s.replaceAll("\\\\:", ":");

            fous = new FileOutputStream(flPath);
            fous.write(s2.getBytes());
        } catch (Exception ex) {
            throw new KrmException(ex);
        } finally {
            if (fous != null) {
                try {
                    fous.close();
                } catch (IOException ex) {
                    LOG.log(Level.SEVERE, "[KrmProp#save]", ex);
                }
            }
        }
    }

        // TODO: не работает получение серийника платы
//    public boolean check() {
//        ProgK k = new ProgK();
//        for (String s : SS) {
//            if (k.do2(s)) {
//                return true;
//            }
//        }
//        return false;
//    }
    /**
     * Создать с деф значениями, если нет.
     *
     * @throws wit.krm.admin.KrmException
     */
    public void checkIfExists() throws KrmException {
        String flPath = getOsDependentPath();
        File fl = new File(flPath);
        if (!fl.exists() || !fl.isFile() || fl.length() == 0) {
            if (fl.length() == 0) {
                fl.delete();
            }

            FileOutputStream fous = null;
            try {
                fous = new FileOutputStream(flPath);
                init();
                props.store(fous, null);
            } catch (Exception ex) {
                LOG.log(Level.SEVERE, null, ex);
            } finally {
                if (fous != null) {
                    try {
                        fous.close();
                    } catch (IOException ex) {
                        LOG.log(Level.SEVERE, null, ex);
                    }
                }
            }
        }
    }

    private String getOsDependentPath() throws KrmException {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            return IConstants.KRM_PATH_LINUX + "/" + IConstants.KRM_INI;
        } else if (Utils.getOs() == Utils.EnOS.OS_WND) {
            return IConstants.KRM_PATH_WIN + "/" + IConstants.KRM_INI;
        }
        throw new KrmException("[KrmProp#load] unknown OS");
    }

    private void init() {
        getPORT_A();
        getPORT_NA();
        getPORT_B();
        getPORT_NB();
        getPORT_CTRL();
        getIP_CAMERA_URL();
        getFPS();
        getLOG_MAX_ROW_NUMBER();
        getMIN_PLATE_SEQUENCE();
        getMIN_PLATE_COUNT_IN_MAP();
        getMAX_PLATE_COUNT_TIME();
        getANPR_DETECT_MODE();
        getANPR_MIN_PLATE_SIZE();
        getANPR_MAX_PLATE_SIZE();
        getANPR_MAX_TEXT_SIZE();
        getANPR_PLATE_RECT_X();
        getANPR_PLATE_RECT_Y();
        getANPR_PLATE_RECT_WIDTH();
        getANPR_PLATE_RECT_HEIGHT();
        //
        getNTP_SERVER();
        getWEB_CAMERA_NUMBER();
    }

    //
    public String getDescrByKey(IConstants.EnKrmProp key) {
        try {
            Method m = this.getClass().getMethod("descr" + key.name());
            return (String) m.invoke(this);
        } catch (Exception ex) {
            LOG.log(Level.SEVERE, null, ex);
        }
        return null;
    }

    public String getStrByKey(IConstants.EnKrmProp key) {
        try {
            Method m = this.getClass().getMethod("get" + key.name());
            return (String) m.invoke(this);
        } catch (Exception ex) {
            LOG.log(Level.SEVERE, null, ex);
        }
        return null;
    }

    public Integer getIntByKey(IConstants.EnKrmProp key) {
        try {
            Method m = this.getClass().getMethod("get" + key.name());
            return (Integer) m.invoke(this);
        } catch (Exception ex) {
            LOG.log(Level.SEVERE, null, ex);
        }
        return null;
    }

    public void setStrByKey(IConstants.EnKrmProp key, String val) {
        try {
            Method m = this.getClass().getMethod("set" + key.name(), String.class);
            Object[] prms = new Object[1];
            prms[0] = val;
            m.invoke(this, prms);
        } catch (Exception ex) {
            LOG.log(Level.SEVERE, null, ex);
        }
    }

    public void setIntByKey(IConstants.EnKrmProp key, Integer val) {
        try {
            Method m = this.getClass().getMethod("set" + key.name(), Integer.class);
            Object[] prms = new Object[1];
            prms[0] = val;
            m.invoke(this, prms);
        } catch (Exception ex) {
            LOG.log(Level.SEVERE, null, ex);
        }
    }

    //
    public String getPORT_A() {
        return getStr("PORT_A", "");
    }

    public void setPORT_A(String s) {
        props.put("PORT_A", s);
    }

    public String descrPORT_A() {
        return tr.tr("IP:PORT куда отправить сообщения если госномер в листе А");
    }

    public String getPORT_NA() {
        return getStr("PORT_NA", "");
    }

    public void setPORT_NA(String s) {
        props.put("PORT_NA", s);
    }

    public String descrPORT_NA() {
        return tr.tr("IP:PORT куда отправить сообщения если госномера нет в листе А");
    }

    public String getPORT_B() {
        return getStr("PORT_B", "");
    }

    public void setPORT_B(String s) {
        props.put("PORT_B", s);
    }

    public String descrPORT_B() {
        return tr.tr("IP:PORT куда отправить сообщения если госномер в листе Б");
    }

    public String getPORT_NB() {
        return getStr("PORT_NB", "");
    }

    public void setPORT_NB(String s) {
        props.put("PORT_NB", s);
    }

    public String descrPORT_NB() {
        return tr.tr("IP:PORT куда отправить сообщения если госномера нет в листе Б");
    }

    public Integer getPORT_CTRL() {
        return getInt("PORT_CTRL", 31000);
    }

    public void setPORT_CTRL(Integer n) {
        props.put("PORT_CTRL", String.valueOf(n));
    }

    public String descrPORT_CTRL() {
        return tr.tr("Локальный порт управления");
    }

    public String getIP_CAMERA_URL() {
        return getStr("IP_CAMERA_URL", "rtsp://127.0.0.1/ONVIF/MediaInput?profile=1_def_profile6");
    }

    public void setIP_CAMERA_URL(String s) {
        props.put("IP_CAMERA_URL", s);
    }

    public String descrIP_CAMERA_URL() {
        return tr.tr("URL для получения видео-потока от камеры");
    }

    public Integer getFPS() {
        return getInt("FPS", 31000);
    }

    public void setFPS(Integer n) {
        props.put("PORT_FPS", String.valueOf(n));
    }

    public String descrFPS() {
        return tr.tr("Частота запуска процедуры распознавания номера (Гц)");
    }

    public Integer getLOG_MAX_ROW_NUMBER() {
        return getInt("LOG_MAX_ROW_NUMBER", 10000);
    }

    public void setLOG_MAX_ROW_NUMBER(Integer n) {
        props.put("LOG_MAX_ROW_NUMBER", String.valueOf(n));
    }

    public String descrLOG_MAX_ROW_NUMBER() {
        return tr.tr("Максимальное количество строк в файле журнала");
    }

    public Integer getMIN_PLATE_SEQUENCE() {
        return getInt("MIN_PLATE_SEQUENCE", 2);
    }

    public void setMIN_PLATE_SEQUENCE(Integer n) {
        props.put("MIN_PLATE_SEQUENCE", String.valueOf(n));
    }

    public String descrMIN_PLATE_SEQUENCE() {
        return tr.tr("Мин кол-во повторных безошибочных распознаваний номеров, чтобы номер был принят как считанный");
    }

    public Integer getMIN_PLATE_COUNT_IN_MAP() {
        return getInt("MIN_PLATE_COUNT_IN_MAP", 2);
    }

    public void setMIN_PLATE_COUNT_IN_MAP(Integer n) {
        props.put("MIN_PLATE_COUNT_IN_MAP", String.valueOf(n));
    }

    public String descrMIN_PLATE_COUNT_IN_MAP() {
        return tr.tr("Мин кол-во повторных распознаваний номеров (с ошибками), чтобы номер был принят как считанный");
    }

    public Integer getMAX_PLATE_COUNT_TIME() {
        return getInt("MAX_PLATE_COUNT_TIME", 3);
    }

    public void setMAX_PLATE_COUNT_TIME(Integer n) {
        props.put("MAX_PLATE_COUNT_TIME", String.valueOf(n));
    }

    public String descrMAX_PLATE_COUNT_TIME() {
        return tr.tr("Макс время накопления данных при распознавании номеров (с ошибками) в последовательности кадров (сек)");
    }

    public Integer getANPR_DETECT_MODE() {
        return getInt("ANPR_DETECT_MODE", 6);
    }

    public void setANPR_DETECT_MODE(Integer n) {
        props.put("ANPR_DETECT_MODE", String.valueOf(n));
    }

    public String descrANPR_DETECT_MODE() {
        return tr.tr("Режимы детектирования номера");
    }

    public Integer getANPR_MIN_PLATE_SIZE() {
        return getInt("ANPR_MIN_PLATE_SIZE", 2000);
    }

    public void setANPR_MIN_PLATE_SIZE(Integer n) {
        props.put("ANPR_MIN_PLATE_SIZE", String.valueOf(n));
    }

    public String descrANPR_MIN_PLATE_SIZE() {
        return tr.tr("?Минимальная площадь номера");
    }

    public Integer getANPR_MAX_PLATE_SIZE() {
        return getInt("ANPR_MAX_PLATE_SIZE", 20000);
    }

    public void setANPR_MAX_PLATE_SIZE(Integer n) {
        props.put("ANPR_MAX_PLATE_SIZE", String.valueOf(n));
    }

    public String descrANPR_MAX_PLATE_SIZE() {
        return tr.tr("?ANPR_MAX_PLATE_SIZE");
    }

    public Integer getANPR_MAX_TEXT_SIZE() {
        return getInt("ANPR_MAX_TEXT_SIZE", 15);
    }

    public void setANPR_MAX_TEXT_SIZE(Integer n) {
        props.put("ANPR_MAX_TEXT_SIZE", String.valueOf(n));
    }

    public String descrANPR_MAX_TEXT_SIZE() {
        return tr.tr("Максимальное количество символов номера + 1");
    }

    public Integer getANPR_PLATE_RECT_X() {
        return getInt("ANPR_PLATE_RECT_X", 0);
    }

    public void setANPR_PLATE_RECT_X(Integer n) {
        props.put("ANPR_PLATE_RECT_X", String.valueOf(n));
    }

    public String descrANPR_PLATE_RECT_X() {
        return tr.tr("X координата верхнего-левого угла области распознавания на кадре");
    }

    public Integer getANPR_PLATE_RECT_Y() {
        return getInt("ANPR_PLATE_RECT_Y", 0);
    }

    public void setANPR_PLATE_RECT_Y(Integer n) {
        props.put("ANPR_PLATE_RECT_Y", String.valueOf(n));
    }

    public String descrANPR_PLATE_RECT_Y() {
        return tr.tr("Y координата верхнего-левого угла области распознавания на кадре");
    }

    public Integer getANPR_PLATE_RECT_WIDTH() {
        return getInt("ANPR_PLATE_RECT_WIDTH", 1200);
    }

    public void setANPR_PLATE_RECT_WIDTH(Integer n) {
        props.put("ANPR_PLATE_RECT_WIDTH", String.valueOf(n));
    }

    public String descrANPR_PLATE_RECT_WIDTH() {
        return tr.tr("Ширина области распознавания на кадре (пикс)");
    }

    public Integer getANPR_PLATE_RECT_HEIGHT() {
        return getInt("ANPR_PLATE_RECT_HEIGHT", 800);
    }

    public void setANPR_PLATE_RECT_HEIGHT(Integer n) {
        props.put("ANPR_PLATE_RECT_HEIGHT", String.valueOf(n));
    }

    public String descrANPR_PLATE_RECT_HEIGHT() {
        return tr.tr("Высота области распознавания на кадре (пикс)");
    }

    public Integer getWEB_CAMERA_NUMBER() {
        return getInt("WEB_CAMERA_NUMBER", -1);
    }

    public void setWEB_CAMERA_NUMBER(Integer n) {
        props.put("WEB_CAMERA_NUMBER", String.valueOf(n));
    }

    public String descrWEB_CAMERA_NUMBER() {
        return tr.tr("Номер usb web-камеры. Если -1, то нет  web-камеры. Если >=0, то IP_CAMERA_URL не учитывается, так как работаем с web-камерой по указанному номеру");
    }

    //
    public String getNTP_SERVER() {
        return getStr("NTP_SERVER", "ntp.ubuntu.com");
    }

    public void setNTP_SERVER(String s) {
        props.put("NTP_SERVER", s);
    }

    private String getStr(String key, String defVal) {
        if (props != null) {
            if (!props.containsKey(key)) {
                props.put(key, defVal);
            }
            return props.getProperty(key);
        }
        return null;
    }

    private Integer getInt(String key, Integer defVal) {
        if (props != null) {
            if (!props.containsKey(key)) {
                props.put(key, String.valueOf(defVal));
            }

            String s = props.getProperty(key);
            if (s != null) {
                try {
                    int n = Integer.parseInt(s);
                    return n;
                } catch (NumberFormatException e) {
                }
            }
        }
        return null;
    }
}
