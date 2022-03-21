package wit.vega.frame;

import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import wit.vega.PlateNoteVO;
import wit.vega.PlateVO;

/**
 *
 * @author vneverov
 */
public class Cmd_GetList_Response extends VegaCmd {

    private static final Logger LOG = Logger.getLogger(Cmd_GetList_Response.class.getName());

    public Cmd_GetList_Response(VegaCmdTag frame) {
        this.frame = frame;
        if (frame.getFrameCount() != 2 && frame.getFrameCount() != 0) {
            throw new RuntimeException("Error creating Cmd25166_GetSingleImg_Response");
        }
    }

    /**
     * Вернуть список разрешенных номеров А|B.
     *
     * @param listNum
     * @return
     */
    public List<PlateVO> getList(PlateVO.EnCamLists listNum) {
        if (frame.getFrame(VegaConsts.NUM_PLATE_IN_LIST) != null) {
            byte[] bb = frame.getFrame(VegaConsts.NUM_PLATE_IN_LIST).getData();
            FourByteNumber fnumPlates = new FourByteNumber(bb, 0);
            int numPlates = fnumPlates.toInt();

            if (numPlates > 0 && frame.getFrame(VegaConsts.PLATE_LIST_STRING) != null) {
                bb = frame.getFrame(VegaConsts.PLATE_LIST_STRING).getData();
                if (bb != null && bb.length > 0) {
                    String s = new String(bb);
                    if (!s.isEmpty()) {
                        List<PlateVO> plates = new ArrayList<PlateVO>(numPlates);
                        s = s.replaceAll("\r", "");
                        String[] lines = s.split("\n");
                        for (String line : lines) {
                            String[] parts = line.split(";");
                            if (parts.length == 2) {
                                plates.add(new PlateVO(parts[0].trim(), parts[1].trim(), new PlateNoteVO(), listNum));
                            } else if (parts.length == 3) {
                                try {
                                    plates.add(new PlateVO(parts[0].trim(), parts[1].trim(), PlateNoteVO.create(parts[2].trim(), true), listNum));
                                } catch (Exception ex) {
                                    LOG.log(Level.SEVERE, "[getList] {0}", ex.getMessage());
                                }
                            }
                        }
                        LOG.log(Level.INFO, "[getList] Загружено {0} из {1}", new Object[]{plates.size(), listNum.name()});
                        return plates;
                    }
                }
            }
        }
        LOG.log(Level.INFO, "[getList] Загружено 0 из {0}", listNum.name());
        return new ArrayList<PlateVO>(0);
    }
}
