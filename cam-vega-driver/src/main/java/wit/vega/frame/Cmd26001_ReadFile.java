package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd26001_ReadFile extends VegaCmd {

    public Cmd26001_ReadFile(String flName) {
        VegaDataTag df1 = new VegaDataTag(VegaConsts.FILE_NAME, flName);

        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_GET_FILE);
        frame = new VegaCmdTag(header);
        frame.addFrame(df1);
    }
}
