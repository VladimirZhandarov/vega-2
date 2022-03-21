package wit.krm.admin;

/**
 *
 * @author vneverov
 */
public interface IConstants {

    enum EnOs {

        OS_UNK,
        OS_WND,
        OS_LNX,
        OS_RASPB
    };

    /**
     * Поля файла настроек программы распознавания автономеров.
     */
    enum EnKrmProp {

        PORT_A,
        PORT_NA,
        PORT_B,
        PORT_NB,
        PORT_CTRL,
        IP_CAMERA_URL,
        FPS,
        LOG_MAX_ROW_NUMBER,
        MIN_PLATE_SEQUENCE,
        MIN_PLATE_COUNT_IN_MAP,
        MAX_PLATE_COUNT_TIME,
        ANPR_DETECT_MODE,
        ANPR_MIN_PLATE_SIZE,
        ANPR_MAX_PLATE_SIZE,
        ANPR_MAX_TEXT_SIZE,
        ANPR_PLATE_RECT_X,
        ANPR_PLATE_RECT_Y,
        ANPR_PLATE_RECT_WIDTH,
        ANPR_PLATE_RECT_HEIGHT,
        PROP_END, // метка до которой идут свойства редактируемые пользователем в таблице настроек
        NTP_SERVER,
//        WEB_CAMERA_NUMBER
    }
    
    /**
     * Тип параметра - String/Integer.
     * 
     * true - str, false - int
     */
    boolean[] krmPropType = new boolean[] {
        true, // PORT_A,
        true, // PORT_NA,
        true, // PORT_B,
        true, // PORT_NB,
        false, // PORT_CTRL,
        true, // IP_CAMERA_URL,
        false, // FPS,
        false, // LOG_MAX_ROW_NUMBER,
        false, // MIN_PLATE_SEQUENCE,
        false, // MIN_PLATE_COUNT_IN_MAP,
        false, // MAX_PLATE_COUNT_TIME,
        false, // ANPR_DETECT_MODE,
        false, // ANPR_MIN_PLATE_SIZE,
        false, // ANPR_MAX_PLATE_SIZE,
        false, // ANPR_MAX_TEXT_SIZE,
        false, // ANPR_PLATE_RECT_X,
        false, // ANPR_PLATE_RECT_Y,
        false, // ANPR_PLATE_RECT_WIDTH,
        false, // ANPR_PLATE_RECT_HEIGHT,
//        false // WEB_CAMERA_NUMBER
    };

    String KRM_PATH_LINUX = "/opt/cerberus";
    String KRM_PATH_WIN = "c:/opt/cerberus";
    String KRM_INI = "settings.ini";
    String KRM_LOG = "log.txt";
}
