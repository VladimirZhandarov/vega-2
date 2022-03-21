/*
 * GrdSystemTime.java
 *
 * Created on 5 Май 2009 г., 14:23
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

public class GrdSystemTime {

    /**
     * The year (1601 - 2099)
     */
    public short mYear;
    /**
     * The month (January = 1, February = 2, ...)
     */
    public short mMonth;
    /**
     * The day of the week (Sunday = 0, Monday = 1, ...)
     */
    public short mDayOfWeek;
    /**
     * The day of the month (1-31)
     */
    public short mDay;
    /**
     * The hour (0-23)
     */
    public short mHour;
    /**
     * The minute (0-59)
     */
    public short mMinute;
    /**
     * The second (0-59)
     */
    public short mSecond;
    /**
     * The millisecond (0-999)
     */
    public short mMilliseconds;
}
