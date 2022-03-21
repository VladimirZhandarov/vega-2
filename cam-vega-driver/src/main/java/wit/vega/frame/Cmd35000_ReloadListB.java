package wit.vega.frame;

/**
 *
 * @author vneverov
 */
public class Cmd35000_ReloadListB extends VegaCmd {

    public Cmd35000_ReloadListB() {
        frame = new VegaCmdTag(new VegaHeaderFrame(VegaConsts.CMD_RELOAD_LIST_B));
    }
}
