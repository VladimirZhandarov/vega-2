package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd35003_GetListB extends VegaCmd {

    public Cmd35003_GetListB() {
        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_GET_LIST_B);
        frame = new VegaCmdTag(header);
    }
}
