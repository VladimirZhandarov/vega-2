/*
 * GrdGIM.java
 *
 * Created on 23 Ноябрь 2007 г., 1:00
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * * Константы для получения информации с помощью функции GrdGetInfo.
 * <br/>Информация о режиме.
 */
public class GrdGIM extends GrdInfo {

    /**
     * Work mode
     */
    public static final GrdGIM WorkMode = new GrdGIM(0x1000);
    /**
     * Handle mode
     */
    public static final GrdGIM HandleMode = new GrdGIM(0x1001);

    GrdGIM(int value) {
        super(value);
    }
}
