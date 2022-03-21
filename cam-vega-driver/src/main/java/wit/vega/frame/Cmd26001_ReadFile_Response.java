package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd26001_ReadFile_Response extends VegaCmd {

    public Cmd26001_ReadFile_Response(VegaCmdTag frame) {
        this.frame = frame;
        if (frame.getFrameCount() != 1 && frame.getFrameCount() != 0) {
            throw new RuntimeException("Error creating Cmd26001_ReadFile_Response");
        }
    }

    public String getString() {
        VegaDataTag dframe = frame.getFrame(VegaConsts.FILE_DATA);
        if (dframe != null) {
            return dframe.toString();
        }
        return "";
    }
}
