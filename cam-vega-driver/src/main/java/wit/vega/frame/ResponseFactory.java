package wit.vega.frame;

import java.io.IOException;
import java.io.InputStream;
import wit.vega.VegaException;

/**
 *
 * @author vneverov
 */
public class ResponseFactory {

    public static VegaCmd deserialize(InputStream ins) throws IOException, VegaException {
        VegaCmd resp = null;
        VegaCmdTag frame = deserializeCmdTag(ins);
        Integer commandCode = frame.getCommandCodeInt();
        switch (commandCode) {
            case VegaConsts.CMD_GET_SINGL_IMG:
                resp = new Cmd25166_GetSingleImg_Response(frame);
                break;
            case VegaConsts.CMD_GET_LIST_A:
                resp = new Cmd_GetList_Response(frame);
                break;
            case VegaConsts.CMD_GET_LIST_B:
                resp = new Cmd_GetList_Response(frame);
                break;
            case VegaConsts.CMD_GET_FILE:
                resp = new Cmd26001_ReadFile_Response(frame);
                break;
            case VegaConsts.CMD_CAM_EVENT:
                resp = new MsgCamEvent(frame);
                break;
            case VegaConsts.CMD_GET_DIG_STATUS:
                resp = new Cmd25134_GetDigOutState_Response(frame);
                break;
        }
        return resp;
    }

    public static VegaCmdTag deserializeCmdTag(InputStream ins) throws IOException, VegaException {
        VegaHeaderFrame header = new VegaHeaderFrame();
        header.deserialize(ins);

        VegaCmdTag frame;
        if (header.getCommandCodeInt() == VegaConsts.CMD_CAM_EVENT) {
            frame = new VegaMsgTag(header);
        } else {
            frame = new VegaCmdTag(header);
        }

        frame.deserialize(ins);
        return frame;
    }
}
