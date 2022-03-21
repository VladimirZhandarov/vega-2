package wit.vega;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.imageio.ImageIO;
import wit.vega.frame.*;
import wit.vega.net.TcpClient;
import wit.vega.PlateVO;

/**
 * Управляет процессами обмена с камерой.
 *
 * @author vneverov
 */
public class VegaManager {

    //
    private static final String FL_LST_A = "List_A.txt";
    private static final String FL_LST_B = "List_B.txt";
    private static final String FAKE_PLATE = "X999XX999";
    /**
     * Размер BANCO блока идущий в начале картинки.
     */
    private static final int BANCO_SZ = 40;
    /**
     * IP камеры.
     */
    private final String host;
    /**
     * Порт камеры принимающий команды.
     */
    private final int camPort;

    public VegaManager(String host, int camPort) {
        this.host = host;
        this.camPort = camPort;
    }

    // TODO: test only
//    private void write2file(byte[] bb) {
//        if (bb != null) {
//            String sfl = String.valueOf(new Date().getTime()) + ".jpeg";
//            try {
//                FileOutputStream fous = new FileOutputStream("/home/vneverov/" + sfl);
//                fous.write(bb);
//                fous.close();
//            } catch (FileNotFoundException ex) {
//                Logger.getLogger(VegaManager.class.getName()).log(Level.SEVERE, null, ex);
//            } catch (IOException ex) {
//                Logger.getLogger(VegaManager.class.getName()).log(Level.SEVERE, null, ex);
//            }
//
//        }
//    }
    /**
     * Без блока БАНКО.
     *
     * @param jpegQuality
     * @param picWidth
     * @param picHeight
     * @return
     * @throws UnknownHostException
     * @throws IOException
     * @throws VisiterException
     */
    public byte[] getImageByte(int jpegQuality, int picWidth, int picHeight) throws UnknownHostException, IOException, VegaException {
        Cmd25166_GetSingleImg_Response responce = (Cmd25166_GetSingleImg_Response) sendReceive(new Cmd25166_GetSingleImg(jpegQuality, picWidth, picHeight));
        if (responce != null) {
            byte[] bb = responce.getImageBytes();

            // TODO: test only
//            write2file(bb);
            if (bb != null && bb.length > BANCO_SZ) {
                byte[] bb2 = Arrays.copyOfRange(bb, BANCO_SZ, bb.length);

                // TODO: test only                
//                write2file(bb2);
                return bb2;
            }
        }
        return null;
    }

    public Image getImage(int jpegQuality, int picWidth, int picHeight) throws UnknownHostException, IOException, VegaException {
        BufferedImage buffImg = null;
        byte[] bb = getImageByte(jpegQuality, picWidth, picHeight);
        if (bb != null && bb.length > 0) {
            ByteArrayInputStream bais = new ByteArrayInputStream(bb);
            buffImg = ImageIO.read(bais);
        }
        return buffImg;
    }

    /**
     * Загрузка номеров в камеру, в какой список определяется из PlateVO.listNum
     *
     * @param plates
     * @throws UnknownHostException
     * @throws IOException
     */
    public void setList(Collection<PlateVO> plates) throws UnknownHostException, IOException {
        // разделяем на списки
        List<PlateVO> plates1 = new ArrayList<PlateVO>();
        List<PlateVO> plates2 = new ArrayList<PlateVO>();
        for (PlateVO plate : plates) {
            if (plate.getListNum() == PlateVO.CAM_LIST_A) {
                plates1.add(plate);
            } else if (plate.getListNum() == PlateVO.CAM_LIST_B) {
                plates2.add(plate);
            }
        }

        if (!plates1.isEmpty()) {
            setListA(plates1);
        } else {
            setEmptyListA();
        }

        if (!plates2.isEmpty()) {
            setListB(plates2);
        } else {
            setEmptyListB();
        }
    }

    public List<PlateVO> getList() throws UnknownHostException, IOException, VegaException {
        List<PlateVO> platesCurrCam1 = getListA();
        List<PlateVO> platesCurrCam2 = getListB();

        // Убираем дублирующие
        Set<PlateVO> setPlates = new HashSet<PlateVO>();
        for (PlateVO plate : platesCurrCam1) {
            setPlates.add(plate);
        }

        for (PlateVO plate : platesCurrCam2) {
            setPlates.add(plate);
        }

        List<PlateVO> platesCurrCam = new ArrayList<PlateVO>(setPlates);
        //Collections.sort(platesCurrCam);
        return platesCurrCam;
    }

    ///////////////////////////////////////////////////////////////////////////
    // List A
    public List<PlateVO> getListA() throws UnknownHostException, IOException, VegaException {
        Cmd_GetList_Response responce = (Cmd_GetList_Response) sendReceive(new Cmd35002_GetListA());
        return responce.getList(PlateVO.CAM_LIST_A);
    }

    public void setListA(List<PlateVO> plates) throws UnknownHostException, IOException {
        setList(FL_LST_A, plates);
        reloadListA();
    }

    public void setEmptyListA() throws UnknownHostException, IOException {
        writeFile(FL_LST_A, "\r\n");
        reloadListA();
    }

    public List<PlateVO> getListB() throws UnknownHostException, IOException, VegaException {
        Cmd_GetList_Response responce = (Cmd_GetList_Response) sendReceive(new Cmd35003_GetListB());
        List<PlateVO> res = responce.getList(PlateVO.CAM_LIST_B);

        // удаляем костыль
        for (PlateVO pvo : res) {
            if (pvo.getPlateNumber().equals(FAKE_PLATE)) {
                res.remove(pvo);
                break;
            }
        }
        return res;
    }

    ///////////////////////////////////////////////////////////////////////////
    // List B
    public void setListB(List<PlateVO> plates) throws UnknownHostException, IOException {
        setList(FL_LST_B, plates);
        reloadListB();
    }

    public void setEmptyListB() throws UnknownHostException, IOException {
        // костыль, еси в списке В нет номеров то перестают приходит сообщения от этого списка
        // принудительно добавляем в список номер, если он пуст
        List<PlateVO> plates = new ArrayList<PlateVO>(1);
        plates.add(new PlateVO(FAKE_PLATE, "RUS", null, PlateVO.CAM_LIST_B));
        setList(FL_LST_B, plates);

//        writeFile(FL_LST_B, "\r\n");
        reloadListB();
    }

    public String readFile(String flName) throws UnknownHostException, IOException, VegaException {
        Cmd26001_ReadFile_Response responce = (Cmd26001_ReadFile_Response) sendReceive(new Cmd26001_ReadFile(flName));
        return responce.getString();
    }

    public void writeFile(String flName, String flData) throws UnknownHostException, IOException {
        send(new Cmd26002_WriteFile(flName, flData));
    }

    public void reloadListA() throws UnknownHostException, IOException {
        send(new Cmd35000_ReloadListA());
    }

    public void reloadListB() throws UnknownHostException, IOException {
        send(new Cmd35000_ReloadListB());
    }

    public boolean isDigitalOutputOn(int digNum) throws IOException, UnknownHostException, VegaException {
        Cmd25134_GetDigOutState_Response resp = (Cmd25134_GetDigOutState_Response) sendReceive(new Cmd25134_GetDigOutState());
        return resp.getDigOutputState(digNum);
    }

    public void setDigitalOutputState(boolean isOn, int digNum) throws UnknownHostException, IOException {
        if (isOn) {
            send(new Cmd25051_SetDigOutStateOn(digNum));
        } else {
            send(new Cmd25051_SetDigOutStateOff(digNum));
        }
    }

    public String getHost() {
        return host;
    }

    public int getCamPort() {
        return camPort;
    }

    public VegaCmd receive(InputStream ins) throws IOException, VegaException {
        ResponseFactory fabr = new ResponseFactory();
        return fabr.deserialize(ins);
    }

    public Boolean isCameraAvailable() {
        TcpClient cl = new TcpClient();
        try {
            cl.open(host, camPort);
        } catch (Exception ex) {
            return false;
        } finally {
            try {
                cl.close();
            } catch (IOException ex) {
            }
        }
        return true;
    }

    //---------------------------------------------------------------------------------------------------------------
    private void send(VegaCmd cmd) throws UnknownHostException, IOException {
        TcpClient cl = new TcpClient();
        try {
            cl.open(host, camPort);
            cl.send(cmd);
        } finally {
            cl.close();
        }
    }

    private VegaCmd sendReceive(VegaCmd cmd) throws UnknownHostException, IOException, VegaException {
        TcpClient cl = new TcpClient();
        try {
            cl.open(host, camPort);
            return cl.sendReceive(cmd);
        } finally {
            cl.close();
        }
    }

    private void setList(String flName, List<PlateVO> plates) throws UnknownHostException, IOException {
        StringBuilder sb = new StringBuilder(1024);
        for (PlateVO plate : plates) {
            sb.append(plate.getPlateNumber());
            sb.append(";");
            sb.append(plate.getPlateCountry());
            sb.append(";");
            sb.append(plate.getPlateNote() == null ? "" : plate.getPlateNote().toString());
            sb.append("\r\n");
        }
        writeFile(flName, sb.toString());
    }
}
