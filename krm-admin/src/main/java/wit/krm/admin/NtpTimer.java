package wit.krm.admin;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.annotation.WebListener;
import wit.krm.admin.bean.AppSingleton;

/**
 *
 * @author vneverov
 */
@WebListener
public class NtpTimer implements ServletContextListener {

    private static final Logger LOG = Logger.getLogger(NtpTimer.class.getName());
    private ScheduledExecutorService scheduler;
    private boolean inProcess;
    private final KrmCtrl ctrl = new KrmCtrl();

    private class NtpJob implements Runnable {

        @Override
        public void run() {
//            LOG.info("[NtpJob] in ...");
            if (!inProcess) {
//                long startProcessTime = System.currentTimeMillis();
                inProcess = true;
                try {
                    String srv = AppSingleton.getInstance().getKrmProp().getNTP_SERVER();
//                    LOG.log(Level.INFO, "[NtpJob] srv {0}", srv);
                    String s = ctrl.syncNtpServer(srv);
//                    LOG.log(Level.INFO, "[NtpJob] {0} ({1})", new Object[]{s, srv});
                } catch (Throwable ex) {
                    LOG.log(Level.SEVERE, "[NtpJob] ", ex);
                } finally {
//                    LOG.log(Level.INFO, "[NtpJob] finished in {0}", (System.currentTimeMillis() - startProcessTime));
                    inProcess = false;
                }
            } else {
                LOG.info("[NtpJob] cannot sync, task already busy");
            }
        }
    }

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            scheduler = Executors.newSingleThreadScheduledExecutor();
            scheduler.scheduleAtFixedRate(new NtpJob(), 10, 3600, TimeUnit.SECONDS); // 1 час
            LOG.info("[NtpTimer#contextInitialized] ...");
        } else {
            LOG.warning("[NtpTimer#contextInitialized] cannot start NTP sync task, unsupported OS");
        }
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            scheduler.shutdownNow();
            LOG.info("[NtpTimer#contextDestroyed]");
        }
    }
}
