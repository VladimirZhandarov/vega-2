package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd25051_SetDigOutStateOn extends VegaCmd {

    /**
     *
     * @param outNum номер выхода с 0
     */
    public Cmd25051_SetDigOutStateOn(int outNum) {
        VegaDataTag df1 = new VegaDataTag(VegaConsts.DIGITAL_OUTPUT_SEL_NUMBER, outNum);
        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_SET_DIG_OUT_ON);
        frame = new VegaCmdTag(header);
        frame.addFrame(df1);
    }
}
