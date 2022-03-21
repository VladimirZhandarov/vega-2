/*
 * GrdGIF.java
 *
 * Created on 23 Ноябрь 2007 г., 0:57
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Константы для получения информации с помощью функции GrdGetInfo.
 * <br/>Режимы поиска и логина.
 */
public class GrdGIF extends GrdInfo {

    /**
     * Локальные и/или удаленные ключи (GrdFMR.Local + GrdFMR.Remote)
     */
    public static final GrdGIF Remote = new GrdGIF(0x2000);
    /**
     * Флаги
     */
    public static final GrdGIF Flags = new GrdGIF(0x2001);
    /**
     * Заданное в GrdSetFindMode() значение для поля"Номера программы"
     */
    public static final GrdGIF Prog = new GrdGIF(0x2002);
    /**
     * Заданное в GrdSetFindMode() значение для поля "Уникального идентификатора ключа"
     */
    public static final GrdGIF ID = new GrdGIF(0x2003);
    /**
     * Заданное в GrdSetFindMode() значение для поля "Серийного номера"
     */
    public static final GrdGIF SN = new GrdGIF(0x2004);
    /**
     * Заданное в GrdSetFindMode() значение для поля "Версия"
     */
    public static final GrdGIF Ver = new GrdGIF(0x2005);
    /**
     * Заданное в GrdSetFindMode() значение для поля "Битовая маска"
     */
    public static final GrdGIF Mask = new GrdGIF(0x2006);
    /**
     * Заданное в GrdSetFindMode() значение для поля "Тип ключа"
     */
    public static final GrdGIF Type = new GrdGIF(0x2007);
    /**
     * Possible model bits. 1 -- GrdDM_XXX (GS1L, GS1U, GF1L, GS2L, GS2U )
     */
    public static final GrdGIF Model = new GrdGIF(0x2008);
    /**
     * Possible interface bits. 1 -- GrdDI_XXX (LPT | USB) )
     */
    public static final GrdGIF Interface = new GrdGIF(0x2009);

    GrdGIF(int value) {
        super(value);
    }
}
