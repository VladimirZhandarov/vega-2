package wit.vega.frame;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * TOP header.
 *
 * @author vneverov
 */
public class VegaHeaderFrame {

    private final FourByteNumber headerDimension;
    private final FourByteNumber commandCode;
    private final FourByteNumber sender;
    private final FourByteNumber receiver;
    private FourByteNumber error;
    /**
     * Суммарный размер всех последующих за заголовком данных.
     */
    private FourByteNumber dataDimension;

    public VegaHeaderFrame() {
        headerDimension = new FourByteNumber();
        commandCode = new FourByteNumber();
        sender = new FourByteNumber();
        receiver = new FourByteNumber();
        error = new FourByteNumber();
        dataDimension = new FourByteNumber();
    }

    public VegaHeaderFrame(int commandCode) {
        this.headerDimension = new FourByteNumber(24);
        this.sender = new FourByteNumber(); // Всегда компьютер
        this.receiver = new FourByteNumber(0); // Работаем с любой камерой (ЧБ, ЦВ)
        this.error = new FourByteNumber(0);

        this.commandCode = new FourByteNumber(commandCode);
        this.dataDimension = new FourByteNumber(0);
    }

    public void serialize(OutputStream ous) throws IOException {
        headerDimension.serialize(ous);
        commandCode.serialize(ous);
        sender.serialize(ous);
        receiver.serialize(ous);
        error.serialize(ous);
        dataDimension.serialize(ous);
    }

    public void deserialize(InputStream ins) throws IOException {
        headerDimension.deserialize(ins);
        commandCode.deserialize(ins);
        sender.deserialize(ins);
        receiver.deserialize(ins);
        error.deserialize(ins);
        dataDimension.deserialize(ins);
    }

    public FourByteNumber getError() {
        return error;
    }

    public void setError(FourByteNumber error) {
        this.error = error;
    }

    public int getDataDimensionInt() {
        return this.dataDimension.toInt();
    }

    public void setDataDimensionInt(int n) {
        this.dataDimension = new FourByteNumber(n);
    }

    public void increaseDataDimensionInt(int n) {
        this.dataDimension.increase(n);
    }

    public int getCommandCodeInt() {
        return commandCode.toInt();
    }
}
