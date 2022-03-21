package wit.vega.frame;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import wit.vega.VegaException;

/**
 *
 * @author vneverov
 */
public class MsgCamEvent extends VegaCmd {

    public String DIRECTION_GOAWAY_VALUE = "GOAWAY";
    public String DIRECTION_APROACH_VALUE = "APROACH";
    public String DIRECTION_UNKNOW_VALUE = "UNKNOW";
    
    public MsgCamEvent(VegaCmdTag frame) {
        this.frame = frame;
    }

    /**
     * X777XX77
     *
     * @return
     */
    public String getPlateNum() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_PLATE_STRING);
        if (df != null) {
            return df.toString().trim();
        }
        return "";
    }

    public String getDirection() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_DIRECTION);
        if (df != null) {
            return df.toString().trim();
        }
        return "";
    }
    
    /**
     * RUS
     *
     * @return
     */
    public String getPlateCountry() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_PLATE_COUNTRY);
        if (df != null) {
            return df.toString().trim();
        }
        return "";
    }

    /**
     *
     * @return
     */
    public String getSDate() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_DATE);
        if (df != null) {
            return df.toString();
        }
        return "";
    }

    public Date getDate() {
        Date res = null;
        String sdate = getSDate();
        if (sdate == null || sdate.isEmpty()) {
            return res;
        }

        SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");
        try {
            res = formatter.parse(sdate);
        } catch (ParseException ex) {
            Logger.getLogger(MsgCamEvent.class.getName()).log(Level.SEVERE, null, ex);
        }
        return res;
    }

    /**
     *
     * @return
     */
    public String getSTime() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_TIME);
        if (df != null) {
            return df.toString();
        }
        return "";
    }

    public Date getTime() {
        Date res = null;
        String stime = getSTime();
        if (stime == null || stime.isEmpty()) {
            return res;
        }

        SimpleDateFormat formatter = new SimpleDateFormat("HH-mm-ss-SSS");
        try {
            res = formatter.parse(stime);
        } catch (ParseException ex) {
            Logger.getLogger(MsgCamEvent.class.getName()).log(Level.SEVERE, null, ex);
        }
        return res;
    }

    public int getPlateMinX() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_PLATE_MIN_X);
        if (df != null) {
            try {
                return df.toInt();
            } catch (VegaException ex) {
                Logger.getLogger(MsgCamEvent.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return 0;
    }

    public int getPlateMinY() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_PLATE_MIN_Y);
        if (df != null) {
            try {
                return df.toInt();
            } catch (VegaException ex) {
                Logger.getLogger(MsgCamEvent.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return 0;
    }

    public int getPlateMaxX() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_PLATE_MAX_X);
        if (df != null) {
            try {
                return df.toInt();
            } catch (VegaException ex) {
                Logger.getLogger(MsgCamEvent.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return 0;
    }

    public int getPlateMaxY() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_PLATE_MAX_Y);
        if (df != null) {
            try {
                return df.toInt();
            } catch (VegaException ex) {
                Logger.getLogger(MsgCamEvent.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
        return 0;
    }

    public byte[] getBWPhoto() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_IMAGE_BW);
        if (df != null) {
            return df.getData();
        }
        return null;
    }

    public byte[] getColPhoto() {
        VegaDataTag df = frame.getFrame(VegaConsts.TCP_MSG_TAG_IMAGE_COL);
        if (df != null) {
            return df.getData();
        }
        return null;
    }
}
