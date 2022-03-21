package wit.krm.admin.vo;

import java.io.Serializable;

/**
 *
 * @author vneverov
 */
public class RecognizePropVO implements Serializable {

    private static final long serialVersionUID = -1L;

    private String key;
    private String descr;
    private String val;
    private boolean isDirty;

    public RecognizePropVO() {
    }

    public RecognizePropVO(String key, String descr, String val) {
        this.key = key;
        this.descr = descr;
        this.val = val;
    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public String getDescr() {
        return descr;
    }

    public void setDescr(String descr) {
        this.descr = descr;
    }

    public String getVal() {
        return val;
    }

    public void setVal(String val) {
        if (this.val == null && val != null
            || this.val != null && !this.val.equals(val)) {
            this.val = val;
            isDirty = true;
        }
    }

    public boolean getIsDirty() {
        return isDirty;
    }

    public void setIsDirty(boolean isDirty) {
        this.isDirty = isDirty;
    }
}
