package wit.vega.frame;

/**
 * Загрузка из файла в памяти камеры во внутренние структуры камеры для
 * распознавания.
 *
 * @author vneverov
 */
public class Cmd35000_ReloadListA extends VegaCmd {

    public Cmd35000_ReloadListA() {
        frame = new VegaCmdTag(new VegaHeaderFrame(VegaConsts.CMD_RELOAD_LIST_A));
    }
}
