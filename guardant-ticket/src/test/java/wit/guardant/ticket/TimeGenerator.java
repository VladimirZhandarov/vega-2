package wit.guardant.ticket;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import org.junit.Assert;
import org.junit.Test;

/**
 *
 * @author vneverov
 */
public class TimeGenerator {
    
    private static final SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
    private static final SimpleDateFormat yy = new SimpleDateFormat("yyyy");
    private static final SimpleDateFormat mm = new SimpleDateFormat("MM");
    private static final SimpleDateFormat dd = new SimpleDateFormat("dd");
    private static final SimpleDateFormat hh = new SimpleDateFormat("HH");
    private static final SimpleDateFormat mnt = new SimpleDateFormat("mm");
    private static final SimpleDateFormat ss = new SimpleDateFormat("ss");
    private static final SimpleDateFormat ms = new SimpleDateFormat("SSS");
        
    public TimeGenerator() {
    }

    @Test
    public void generateLongTime() throws GrdException {
        System.out.println("[generateLongTime]");
        Calendar c = Calendar.getInstance();
        c.set(2016, 6, 1);
        long t = c.getTimeInMillis();
        System.out.println(t);
    }

    @Test
    public void testCalendarTime() throws GrdException {
        System.out.println("[testCalendarTime]");
        
        Date now = new Date();
        Calendar c = Calendar.getInstance();
        
        System.out.println(sdf.format(now));
        
        Assert.assertEquals(Integer.parseInt(yy.format(now)), c.get(Calendar.YEAR));
        Assert.assertEquals(Integer.parseInt(mm.format(now)), c.get(Calendar.MONTH) + 1);
        Assert.assertEquals(Integer.parseInt(dd.format(now)), c.get(Calendar.DAY_OF_MONTH));
        Assert.assertEquals(Integer.parseInt(hh.format(now)), c.get(Calendar.HOUR_OF_DAY));
        Assert.assertEquals(Integer.parseInt(mnt.format(now)), c.get(Calendar.MINUTE));
        Assert.assertEquals(Integer.parseInt(ss.format(now)), c.get(Calendar.SECOND));

//        System.out.println(Integer.parseInt(ms.format(now)));
//        System.out.println(c.get(Calendar.MILLISECOND));
        Assert.assertEquals(Integer.parseInt(ms.format(now)), c.get(Calendar.MILLISECOND));
    }
}
