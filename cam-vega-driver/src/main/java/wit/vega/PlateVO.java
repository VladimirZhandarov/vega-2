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
public class PlateVO {

    public static class EnCamLists {

        private String _name;

        public EnCamLists() {
        }

        public String name() {
            return _name;
        }
    }
    public static EnCamLists CAM_LIST_A;
    public static EnCamLists CAM_LIST_B;

    private String plateNumber;
    private String plateCountry;
    private PlateNoteVO plateNote;
    private EnCamLists listNum;

    public PlateVO(String plateNumber, String plateCountry, PlateNoteVO plateNote, EnCamLists listNum) {
        this.plateNumber = plateNumber;
        this.plateCountry = plateCountry;
        this.plateNote = plateNote;
        this.listNum = listNum;
    }

    public String getPlateNumber() {
        return plateNumber;
    }

    public String getPlateCountry() {
        return plateCountry;
    }

    public PlateNoteVO getPlateNote() {
        return plateNote;
    }

    public EnCamLists getListNum() {
        return listNum;
    }
}
