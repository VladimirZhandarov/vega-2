package wit.guardant.ticket;

/**
 *
 * @author vneverov
 */
public class GrdException extends Exception {

    public GrdException(String message) {
        super(message);
    }

    public GrdException(Throwable cause) {
        super(cause);
    }
}
