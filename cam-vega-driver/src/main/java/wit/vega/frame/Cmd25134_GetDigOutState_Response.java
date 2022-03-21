package wit.vega.frame;

import wit.vega.VegaException;

/**
 *
 * @author vneverov
 */
public class Cmd25134_GetDigOutState_Response extends VegaCmd {

    public Cmd25134_GetDigOutState_Response(VegaCmdTag frame) {
        this.frame = frame;
        if (frame.getFrameCount() != 4) {
            throw new RuntimeException("Error creating Cmd25134_GetDigOutState_Response");
        }
    }

    public int getNumberDigInput() throws VegaException {
        VegaDataTag dt = frame.getFrame(VegaConsts.DIGITAL_INPUT_NUMBER);
        return dt.toInt();
    }

    public int getNumberDigOutput() throws VegaException {
        VegaDataTag dt = frame.getFrame(VegaConsts.DIGITAL_OUTPUT_NUMBER);
        int n = dt.toInt();
        return n;
    }

    /**
     *
     * @param inputNum номер входа
     * @return
     * @throws VisiterException
     */
    public boolean getDigIntputState(int inputNum) throws VegaException {
        if (inputNum > getNumberDigInput() - 1) {
            throw new VegaException("[getDigIntputState] wrong parameter");
        }
        VegaDataTag dt = frame.getFrame(VegaConsts.DIGITAL_INPUT_STATUS);
        int n = dt.toInt();
        int mask = 1 << inputNum;
        return (n & mask) != 0;
    }

    /**
     *
     * @param outputNum номер выхода
     * @return
     * @throws VisiterException
     */
    public boolean getDigOutputState(int outputNum) throws VegaException {
        if (outputNum > getNumberDigOutput() - 1) {
            throw new VegaException("[getDigOutputState] wrong parameter");
        }
        VegaDataTag dt = frame.getFrame(VegaConsts.DIGITAL_OUTPUT_STATUS);
        int n = dt.toInt();
        int mask = 1 << outputNum;
        return (n & mask) != 0;
    }
}
