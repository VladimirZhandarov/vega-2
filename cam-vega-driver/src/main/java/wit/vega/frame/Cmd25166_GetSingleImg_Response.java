package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd25166_GetSingleImg_Response extends VegaCmd {

    public Cmd25166_GetSingleImg_Response(VegaCmdTag frame) {
        this.frame = frame;
        if (frame.getFrameCount() != 2) {
            throw new RuntimeException("Error creating Cmd25166_GetSingleImg_Response");
        }
    }

    public byte[] getImageBytes() {
        return frame.getFrame(VegaConsts.IMAGE).getData();
    }
}
