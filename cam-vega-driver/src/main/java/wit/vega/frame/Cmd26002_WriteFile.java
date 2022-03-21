package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd26002_WriteFile extends VegaCmd {

    public Cmd26002_WriteFile(String flName, String flData) {
        VegaDataTag df1 = new VegaDataTag(VegaConsts.FILE_NAME, flName);
        VegaDataTag df2 = new VegaDataTag(VegaConsts.FILE_DATA, flData);

        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_SET_FILE);
        frame = new VegaCmdTag(header);
        frame.addFrame(df1);
        frame.addFrame(df2);
    }
}
