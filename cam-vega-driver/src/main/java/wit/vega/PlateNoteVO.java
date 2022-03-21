/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package wit.vega;

/**
 *
 * @author Vladimir
 */
public class PlateNoteVO {

    private String note;
    private boolean flag;

    public PlateNoteVO() {

    }

    public PlateNoteVO(String note, boolean flag) {
        this.note = note;
        this.flag = flag;
    }

    static public PlateNoteVO create(String note, boolean flag) {
        return new PlateNoteVO(note, flag);
    }
}
