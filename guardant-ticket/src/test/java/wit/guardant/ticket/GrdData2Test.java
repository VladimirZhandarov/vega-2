package wit.guardant.ticket;

import org.junit.AfterClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author vneverov
 */
public class GrdData2Test {
    
    private static final String PUB_ECC_KEY = "579BCBFC51C14A1D9AF483C59EE496D24BCB3439D0B9EA1962CB9B1C4C4FD6BD77CCDEFA4FD40C6B";
    private static final int KEY_ACT_ECC = 1558217874;
    private static final String D0 = "LLC Virasoft";
    private static final String D1 = "Customer";
    private static final String D11 = "FIO";
    private static final String D12 = "+78121234567";
    private static final String D13 = "test@mail.ru";
    private static final String D2 = "Ticket";
    private static final String D3 = "1.69.1";
    private static final String D5 = "SN-123";
    
    @AfterClass
    public static void afterClass() {
        GrdDongle.deinitApi();
    }

    public GrdData2Test() {
    }

    @Test
    public void test1() throws GrdException {
        long t = System.currentTimeMillis();
        GrdData2 d = new GrdData2(D0, D1, D11, D12, D13, D2, D3, t, D5, PUB_ECC_KEY, KEY_ACT_ECC);
        byte[] bb = d.serialize();
        
        String hex = GrdDongle.byte2hex(bb, 32);
        System.out.println("[test1] " + hex);
        System.out.println("[test1] " + d.toString());
        
        GrdData2 d2 = new GrdData2(hex);
        System.out.println("[test1] time: " + (System.currentTimeMillis() - t));
        
        assertEquals(D0, d2.getD0());
        assertEquals(D1, d2.getD1());
        assertEquals(D2, d2.getD2());
        assertEquals(D3, d2.getD3());
        assertEquals(t, d2.getD4());
        assertEquals(D5, d2.getD5());
        assertEquals(PUB_ECC_KEY, d2.getD6());
        assertEquals(KEY_ACT_ECC, d2.getD7());
    }

//    @Test
//    public void test2() throws GrdException {
//        GrdDongle dngl = new GrdDongle();
//        Handle h = dngl.getHandle(GrdData2.SEARCH_DONGLE_VER);
//        GrdE err = Api.GrdPI_Activate(h, 0, KEY_ACT_ECC);
//        GrdDongle.errorHandling(err);
//        
//        byte[] bb = new byte[64];
//        err = Api.GrdPI_Read(h, 0, 0, bb);
//        GrdDongle.errorHandling(err);
//        
//        System.out.println("[test2] " + GrdData2.byte2hex(bb, 0));
//    }
}
