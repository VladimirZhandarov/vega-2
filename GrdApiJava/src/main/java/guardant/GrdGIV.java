/*
 * GrdGIV.java
 *
 * Created on 23 Ноябрь 2007 г., 0:59
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Константы для получения информации с помощью функции GrdGetInfo.
 * <br/>Информация об API.
 */
public class GrdGIV extends GrdInfo {

    /**
     * API version
     */
    public static final GrdGIV VerAPI = new GrdGIV(0x0000);

    GrdGIV(int value) {
        super(value);
    }
}
