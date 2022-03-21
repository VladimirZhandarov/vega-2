package wit.krm.admin;

import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.Set;
import java.util.TimeZone;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Управление, получение инфо о системе и программе распознавания.
 *
 * @author vneverov
 */
public class KrmCtrl {

    private static final Logger LOG = Logger.getLogger(KrmCtrl.class.getName());
    private static final String DEF_HOST_IP = "127.0.0.1";
    private static final String DEF_HOST_NAME = "Error";
    private static final String CMD_GET_IP = "ifconfig eth0  | grep 'inet addr:'| grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'";
    private static final String CMD_GET_HOST = "hostname";
    private static final String CMD_SYNC_TIME = "ntpdate %s";
    private static final String CMD_TIMEZONES = "timedatectl list-timezones";
    private static final String CMD_CURR_TIMEZONE = "timedatectl status|grep Timezone:";
    private static final String CMD_SET_TIMEZONE = "timedatectl set-timezone %s";
    //
    private static final String MSG_UNSUPP_OS = "Неподдерживаемая ОС";
    //
    private final Translator tr = new Translator(null);
    private Set<String> timeZones;

    public String getHostIp() {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            try {
                return exec(CMD_GET_IP);
            } catch (KrmException ex) {
                LOG.log(Level.SEVERE, null, ex);
                return DEF_HOST_IP;
            }
        } else {
            return DEF_HOST_IP;
        }
    }

    public String getHostName() {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            try {
                return exec(CMD_GET_HOST);
            } catch (KrmException ex) {
                LOG.log(Level.SEVERE, null, ex);
                return DEF_HOST_NAME;
            }
        } else {
            return DEF_HOST_NAME;
        }
    }

    /**
     *
     * @param year yyyy
     * @param month
     * @param day
     * @param hour
     * @param min
     * @param sec
     */
    public void setDate(int year, int month, int day, int hour, int min, int sec) throws KrmException {
        Utils.EnOS os = Utils.getOs();
        if (os == Utils.EnOS.OS_LIN) {
            String s = String.format("date %02d%02d%02d%02d%04d.%02d", month, day, hour, min, year, sec);
            exec(s);

            LOG.info(String.format("[KrmCtrl#setDate] date changed: %02d.%02d.%04d %02d:%02d:%02d", day, month, year, hour, min, sec));
        } else {
            throw new KrmException(tr.tr(MSG_UNSUPP_OS));
        }
    }

    public String syncNtpServer(String ntpHost) throws KrmException {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            String s = exec(String.format(CMD_SYNC_TIME, ntpHost));
            return s;
        } else {
            throw new KrmException(tr.tr(MSG_UNSUPP_OS));
        }
    }

    public Set<String> getTimezones() throws KrmException {
        if (timeZones == null || timeZones.isEmpty()) {
            if (Utils.getOs() == Utils.EnOS.OS_LIN) {
                String s = exec(CMD_TIMEZONES);
                if (s != null && !s.isEmpty()) {
                    String[] ss = s.split("\n");
                    if (ss.length > 0) {
                        timeZones = new TreeSet<String>(Arrays.asList(ss));
                    } else {
                        LOG.severe("[KrmCtrl#getTimezones] cmd return wrong str: " + s);
                    }
                } else {
                    LOG.severe("[KrmCtrl#getTimezones] cmd return null");
                }
            } else {
                timeZones = new TreeSet<String>();
            }
        }
        return timeZones;
    }

    public String getCurrTimezone() throws KrmException {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            String s = exec(CMD_CURR_TIMEZONE); //         Timezone: Europe/Moskow (MSK, +0300)
            final String S1 = "Timezone:";
            final String S2 = "(";
            if (s != null && !s.isEmpty()) {
                int i1 = s.indexOf(S1);
                if (i1 >= 0) {
                    int i2 = s.indexOf(S2, i1 + S1.length());
                    if (i2 > 0) {
                        String s3 = s.substring(i1 + S1.length(), i2).trim(); // Europe/Moskow
                        LOG.log(Level.INFO, "[getCurrTimezone] {0}", s3);
                        return s3;
                    }
                }
            }
        }
        return "";
    }

    public void setTimezone(String tmz) throws KrmException {
        if (Utils.getOs() == Utils.EnOS.OS_LIN) {
            Set<String> tmzs = getTimezones();
            if (tmzs.contains(tmz)) {
                String s = exec(String.format(CMD_SET_TIMEZONE, tmz));
                if (s != null && !s.isEmpty()) {
                    throw new KrmException(tr.tr(s));
                }
                
                TimeZone.setDefault(TimeZone.getTimeZone(tmz));
                LOG.info("[setTimezone] update timezone to " + tmz);
            } else {
                throw new KrmException(tr.tr("Неизвестный часовой пояс"));
            }
        }
    }

    public String exec(String cmd) throws KrmException {
        Utils.EnOS os = Utils.getOs();
        if (os == Utils.EnOS.OS_LIN || os == Utils.EnOS.OS_WND) {
            try {
                String s1 = (os == Utils.EnOS.OS_LIN ? "/bin/bash" : "cmd");
                String s2 = (os == Utils.EnOS.OS_LIN ? "-c" : "/c");
                Process pr = Runtime.getRuntime().exec(new String[]{s1, s2, cmd});
                if (pr != null) {
                    pr.waitFor();
                    InputStream is = pr.getInputStream();
                    int n = is.available();
                    if (n > 0) {
                        byte[] bb = new byte[n];
                        int n2 = is.read(bb);
                        if (n2 > 0) {
                            String s = new String(bb, 0, n2);
                            return s;
                        }
                    }
                }
            } catch (IOException ex) {
                throw new KrmException(ex);
            } catch (InterruptedException ex) {
                throw new KrmException(ex);
            }
        } else {
            throw new KrmException("[KrmCtrl#exec] unknown OS");
        }
        return null;
    }
}
