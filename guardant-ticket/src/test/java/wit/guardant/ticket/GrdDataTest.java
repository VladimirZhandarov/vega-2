package wit.guardant.ticket;

import java.util.HashSet;
import java.util.Random;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.AfterClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author vneverov
 */
public class GrdDataTest {

    private static final String PUB_ECC_KEY = "579BCBFC51C14A1D9AF483C59EE496D24BCB3439D0B9EA1962CB9B1C4C4FD6BD77CCDEFA4FD40C6B";
    private static final int KEY_ACT_ECC = 1558217874;
    private static final String D0 = "LLC Virasoft";
    private static final String D1 = "Customer";
    private static final String D2 = "Ticket";
    private static final int D3 = 69;
    private static final String D5 = "SN-123";
    private static final byte DNGL_VERS = (byte) 0;

    public GrdDataTest() {
    }

    @AfterClass
    public static void afterClass() {
        GrdDongle.deinitApi();
    }

/*    @Test
    public void testEncodeDecode() throws GrdException {
        System.out.println("[testEncodeDecode]");
        long t = System.currentTimeMillis();
        String sn = GrdDongle.getSN(GrdData.SEARCH_DONGLE_VER);
        System.out.println("[testEncodeDecode] SN: " + sn);
        
        GrdData d = new GrdData(D0, D1, D2, (byte) D3, t, D5, PUB_ECC_KEY, KEY_ACT_ECC);
        byte[] bb = d.serialize();

        String hex = GrdDongle.byte2hex(bb, 32);
        System.out.println("lic:\r\n" + hex);

        GrdData d2 = new GrdData(hex);
        System.out.println("time: " + (System.currentTimeMillis() - t));

        assertEquals(D0, d2.getD0());
        assertEquals(D1, d2.getD1());
        assertEquals(D2, d2.getD2());
        assertEquals(D3, d2.getD3());
        assertEquals(t, d2.getD4());
        assertEquals(D5, d2.getD5());
        assertEquals(PUB_ECC_KEY, d2.getD6());
        assertEquals(KEY_ACT_ECC, d2.getD7());
    }*/

    private static boolean isErr;
    private static final Set<Integer> ths = new HashSet<Integer>();

    private class Runnable1 implements Runnable {

        private int th;

        public Runnable1(int th) {
            this.th = th;
            ths.add(th);
        }

        @Override
        public void run() {
            try {
                do1(th);
            } catch (GrdException ex) {
                isErr = true;
            }
            ths.remove(th);
        }
    }

//    @Test
//    public void testSignInMultythread() throws GrdException {
//        System.out.println("[testSignInMultythread]");
//        long t = System.currentTimeMillis();
//
//        for (int i = 0; i < 20 && !isErr; i++) {
//            new Thread(new Runnable1(i)).start();
//        }
//
//        while (!ths.isEmpty()) {
//            try {
//                Thread.sleep(50);
//            } catch (InterruptedException ex) {
//                Logger.getLogger(GrdDataTest.class.getName()).log(Level.SEVERE, null, ex);
//                return;
//            }
//        }
//
//        System.out.println("time: " + (System.currentTimeMillis() - t));
//    }

    protected void do1(int idx) throws GrdException {
        System.out.println("[do1] #" + idx);
        Random rnd = new Random(System.currentTimeMillis());
        byte[] bb0 = new byte[20];
        rnd.nextBytes(bb0);

        byte[] bb1 = GrdDongle.sign(DNGL_VERS, KEY_ACT_ECC, bb0);
//        assertTrue(bb1 != null && bb1.length == 40);

        int e = GrdDongle.verifySign(DNGL_VERS, KEY_ACT_ECC, bb0, bb1, GrdDongle.hex2byte(PUB_ECC_KEY));
//        assertTrue(e == 0);

        // need fail
        rnd.nextBytes(bb0);
        int e2 = GrdDongle.verifySign(DNGL_VERS, KEY_ACT_ECC, bb0, bb1, GrdDongle.hex2byte(PUB_ECC_KEY));
//        assertTrue(e != 0);
        System.out.println("[do1] #" + idx + " finished: " + (e == 0 && e2 != 0 ? "OK" : "FAIL"));
    }
}
