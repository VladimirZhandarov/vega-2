package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd35002_GetListA extends VegaCmd {

    public Cmd35002_GetListA() {
        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_GET_LIST_A);
        frame = new VegaCmdTag(header);
    }
}
