package wit.vega.frame;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author vneverov
 */
public class Cmd25166_GetSingleImg extends VegaCmd {

    public Cmd25166_GetSingleImg(int jpegQuality, int picWidth, int picHeight) {
        FourByteNumber q = new FourByteNumber(jpegQuality);
        VegaDataTag df1 = new VegaDataTag(VegaConsts.JPEG_QUALITY, q.getBytes());

        FourByteNumber w = new FourByteNumber(picWidth);
        FourByteNumber h = new FourByteNumber(picHeight);
        //
        VegaDataTag df2 = null;
        ByteArrayOutputStream baos = new ByteArrayOutputStream(8);
        try {
            baos.write(w.getBytes());
            baos.write(h.getBytes());
            df2 = new VegaDataTag(VegaConsts.POINT, baos.toByteArray());
            baos.close();
        } catch (IOException ex) {
            Logger.getLogger(Cmd25166_GetSingleImg.class.getName()).log(Level.SEVERE, null, ex);
            return;
        }
        //
//        Integer lenDataFrame = df1.getLength();
//        lenDataFrame += df2.getLength();

        VegaHeaderFrame header = new VegaHeaderFrame(VegaConsts.CMD_GET_SINGL_IMG);
        frame = new VegaCmdTag(header);
        frame.addFrame(df1);
        frame.addFrame(df2);
    }
}
