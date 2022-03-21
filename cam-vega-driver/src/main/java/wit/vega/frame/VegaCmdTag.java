package wit.vega.frame;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.apache.log4j.Logger;
import wit.vega.VegaUtils;
import wit.vega.VegaException;

/**
 * Команда PC -> VEGA.
 *
 * @author vneverov
 */
public class VegaCmdTag {

    private static final Logger log = Logger.getLogger(VegaCmdTag.class);
    public final VegaHeaderFrame header;
    // dataCode -> frame
    private final Map<Integer, VegaDataTag> mapFrames = new HashMap<Integer, VegaDataTag>();
    private final List<VegaDataTag> frames = new ArrayList<VegaDataTag>();

    public VegaCmdTag(VegaHeaderFrame header) {
        this.header = header;
    }

    public VegaCmdTag(int commandCode) {
        header = new VegaHeaderFrame(commandCode);
    }

    public Integer getCommandCodeInt() {
        return header.getCommandCodeInt();
    }

    public VegaDataTag getFrame(Integer dataCode) {
        return mapFrames.get(dataCode);
    }

    public Integer getFrameCount() {
        return frames.size();
    }

    public Set<Integer> getDataCodes() {
        return mapFrames.keySet();
    }

    public void addFrame(VegaDataTag frame) {
        frames.add(frame);
        mapFrames.put(frame.getDataCodeInt(), frame);
        header.increaseDataDimensionInt(frame.getLength());
    }

    public void serialize(OutputStream ous) throws IOException {
        header.serialize(ous);
        for (VegaDataTag frame : frames) {
            frame.serialize(ous);
        }
    }

    public void deserialize(InputStream ins) throws IOException, VegaException {
        Integer dataLen = header.getDataDimensionInt();
        header.setDataDimensionInt(0);
        while (dataLen >= VegaDataTag.HEADER_SZ) {
            VegaDataTag frame = new VegaDataTag();
            dataLen = frame.deserialize(ins, dataLen);
            addFrame(frame);
        }
    }

    public void log() throws IOException {
        ByteArrayOutputStream baous = new ByteArrayOutputStream();
        serialize(baous);
        byte[] bout = baous.toByteArray();
        log.info(String.format("Send %d bytes: %s", bout.length, VegaUtils.getHex(bout)));
        baous.close();
    }
}
