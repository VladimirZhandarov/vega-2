package wit.guardant.ticket;

import java.util.Random;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author vneverov
 */
public class GrdDongleTest {
    
    private static final String PUB_KEY_ECC = "579BCBFC51C14A1D9AF483C59EE496D24BCB3439D0B9EA1962CB9B1C4C4FD6BD77CCDEFA4FD40C6B";
    private static final byte V = (byte)69;
    private static final int KEY_ACT_ECC = 1558217874;
    
    public GrdDongleTest() {
    }

/*    @Test
    public void testSign() throws GrdException {
        Random rnd = new Random(System.currentTimeMillis());
        byte[] bb0 = new byte[20];
        rnd.nextBytes(bb0);
        
        
        byte[] bb1 = GrdDongle.sign(V, KEY_ACT_ECC, bb0);
        
        assertTrue(bb1 != null && bb1.length == 40);
        
        int e = GrdDongle.verifySign(V, KEY_ACT_ECC, bb0, bb1, GrdDongle.hex2byte(PUB_KEY_ECC));
        assertTrue(e == 0);

        // need fail
        rnd.nextBytes(bb0);
        e = GrdDongle.verifySign(V, KEY_ACT_ECC, bb0, bb1, GrdDongle.hex2byte(PUB_KEY_ECC));
        assertTrue(e != 0);
        
//        dn.close();
    }*/
}
