package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd25134_GetDigOutState extends VegaCmd {

    public Cmd25134_GetDigOutState() {
        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_GET_DIG_STATUS);
        frame = new VegaCmdTag(header);
    }
}
