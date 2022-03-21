/*
 * GrdLM.java
 *
 * Created on 23 Ноябрь 2007 г., 0:41
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */
package guardant;

/**
 * Флаги для функции <b>GrdLogin</b>
 */
public class GrdLM {

    /**
     * Сетевые лицензии распределяются по рабочим станциям, вне зависимости от количества запущенных копий приложений на одной станции.
     */
    public static final GrdLM PerStation = new GrdLM(0x00000000);
    /**
     * Сетевые лицензии распределяются хэндлам. Каждый новый регистрируемый через GrdLogin хэндл получит отдельную лицензию.
     */
    public static final GrdLM PerHandle = new GrdLM(0x00000001);
    /**
     * Сетевые лицензии распределяются рабочим процессам, вне зависимости от количества запущенных потоков в одной копии приложения.
     */
    public static final GrdLM PerProcess = new GrdLM(0x00000002);

    int mValue;

    GrdLM(int value) {
        mValue = value;
    }

    public int getValue() {
        return mValue;
    }

}
