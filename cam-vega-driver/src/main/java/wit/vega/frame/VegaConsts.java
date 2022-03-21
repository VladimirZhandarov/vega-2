package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public interface VegaConsts {

    // Команды
    int CMD_SET_DIG_OUT_ON = 25051;
    int CMD_SET_DIG_OUT_OFF = 25052;
    int CMD_GET_DIG_STATUS = 25134;
    int CMD_GET_SINGL_IMG = 25166;
    int CMD_RELOAD_LIST_A = 35000;
    int CMD_RELOAD_LIST_B = 35001;
    int CMD_GET_LIST_A = 35002;
    int CMD_GET_LIST_B = 35003;
    int CMD_GET_FILE = 26001;
    int CMD_SET_FILE = 26002;
    /**
     * Сообщение от камеры
     */
    int CMD_CAM_EVENT = 40000;

    // Блоки данных в ответе
    int IMAGE_TIME = 2001;
    int IMAGE = 2000;
    int JPEG_QUALITY = 10015;
    int POINT = 10069;
    int FILE_NAME = 12038;
    int FILE_DATA = 12039;
    /**
     * Number of digital input on the camera
     */
    int DIGITAL_INPUT_NUMBER = 12041;
    /**
     * Number of digital output on the camera
     */
    int DIGITAL_OUTPUT_NUMBER = 12042;
    /**
     * Bit field that identifies the physical state of the digital inputs. The
     * input 0 is the least significant bit (0-31).
     */
    int DIGITAL_INPUT_STATUS = 12043;
    /**
     * Bit field that identifies the physical state of the digital inputs. The
     * output 0 is the least significant bit (0-31).
     */
    int DIGITAL_OUTPUT_STATUS = 12044;
    /**
     * Number of the output to activate
     */
    int DIGITAL_OUTPUT_SEL_NUMBER = 12062;
    int NUM_PLATE_IN_LIST = 13208;
    int PLATE_LIST_STRING = 13209;

    // Блоки данных в сообщении от камеры
//	int CAM_EVENT_DATE = 14000;
//	int CAM_EVENT_TIME = 14001;
//	int CAM_EVENT_NETBIOSNAME = 14002;
//	int CAM_EVENT_PLATE = 14003;
//	int CAM_EVENT_PLATE_STRING = 14004;
//	int CAM_EVENT_PLATE_COUNTRY = 14005;
//	int CAM_EVENT_IMAGE_BW = 14020;
//	int CAM_EVENT_IMAGE_COL = 14021;
    /**
     * %DATE, 12
     */
    int TCP_MSG_TAG_DATE = 14000;
    /**
     * %TIME, 16
     */
    int TCP_MSG_TAG_TIME = 14001;
    /**
     * %NETBIOSNAME, 32
     */
    int TCP_MSG_TAG_NETBIOSNAME = 14002;
    /**
     * %PLATE, 24, X777XX-RUS
     */
    int TCP_MSG_TAG_PLATE = 14003;
    /**
     * %PLATE_STRING, 20, X777XX
     */
    int TCP_MSG_TAG_PLATE_STRING = 14004;
    /**
     * %PLATE_COUNTRY, 4, RUS
     */
    int TCP_MSG_TAG_PLATE_COUNTRY = 14005;
    int TCP_MSG_TAG_NUM_READ = 14006;
    int TCP_MSG_TAG_IMAGENAME = 14007;
    int TCP_MSG_TAG_IMAGEPATH = 14008;
    int TCP_MSG_TAG_TRANSIT_ID = 14009;
    int TCP_MSG_TAG_PLATE_MIN_X = 14010;
    int TCP_MSG_TAG_PLATE_MIN_Y = 14011;
    int TCP_MSG_TAG_PLATE_MAX_X = 14012;
    int TCP_MSG_TAG_PLATE_MAX_Y = 14013;
    int TCP_MSG_TAG_SPEED = 14014;
    int TCP_MSG_TAG_CLASS = 14015;
    int TCP_MSG_TAG_CLASS_STRING = 14016;
    int TCP_MSG_TAG_BOARD_SN = 14017;
    int TCP_MSG_TAG_DIRECTION = 14018;
    int TCP_MSG_TAG_NET_TRIG_ID = 14019;
    /**
     * %IMAGE_BW
     */
    int TCP_MSG_TAG_IMAGE_BW = 14020;
    /**
     * %IMAGE_COL
     */
    int TCP_MSG_TAG_IMAGE_COL = 14021;
    int TCP_MSG_TAG_OCRSCORE = 14022;

}
