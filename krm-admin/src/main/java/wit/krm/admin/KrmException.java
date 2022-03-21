package wit.krm.admin;

/**
 *
 * @author vneverov
 */
public class KrmException extends Exception {

    public KrmException(String message) {
        super(message);
    }

    public KrmException(String message, Throwable cause) {
        super(message, cause);
    }

    public KrmException(Throwable cause) {
        super(cause);
    }
}
