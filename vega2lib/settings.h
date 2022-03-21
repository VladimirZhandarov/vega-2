#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <string>

#include <boost/property_tree/ptree.hpp>

#include <opencv2/core/types_c.h>

/**
 * Настройки программы (vega-2.ini)
 */
class Settings
{
	static std::string logLevelKey;			///< Имя ключа "Уровень журналирования"
	static std::string portAKey;			///< Имя ключа "Порт A"
	static std::string portNAKey;			///< Имя ключа "Порт не A"
	static std::string portBKey;			///< Имя ключа "Порт B"
	static std::string portNBKey;			///< Имя ключа "Порт не B"
	static std::string portControlKey;		///< Имя ключа "Порт управления"
	static std::string portApkControlKey;	///< Имя ключа "Порт управления apk"
	static std::string ipCameraUrlKey;		///< Имя ключа "Путь к ip-камере"
	static std::string webCameraNumberKey;	///< Имя ключа "Номер web-камеры"
	static std::string fpsKey;				///< Имя ключа "FPS"

	static std::string recognizeThreadNumberKey;	///< Количество потоков распознавания номера

	static std::string logMaxRowNumberKey;	///< Максимальное количество строк в файле журнала

	static std::string minPlateSequenceKey;	///< Имя ключа "Минимальное число считываний номера для определения номера"

	static std::string minPlateCountInMapKey;	///< Имя ключа "Минимальное значение счетчика распознаваний номера для отправки номера сервису"

	static std::string maxPlateCountTimeKey;	///< Имя ключа "Таймаут работы счетчиков распознавания"

	static std::string anprDetectModeKey;	///< Имя ключа "Режим распознавания номеров для библиотеки iANPR"

	static std::string anprMinPlateSizeKey;	///< Имя ключа "Минимальная площадь номера для библиотеки iANPR"

	static std::string anprMaxPlateSizeKey;	///< Имя ключа "Максимальная площадь номера для библиотеки iANPR"

	static std::string anprMaxTextSizeKey;	///< Имя ключа "Максимальное количество символов в номере для библиотеки iANPR"

	static std::string anprPlateRectXKey;	///< Имя ключа "X координата верхнего левого угла области распознавания номера"

	static std::string anprPlateRectYKey;	///< Имя ключа "Y координата верхнего левого угла области распознавания номера"

	static std::string anprPlateRectWidthKey;	///< Имя ключа "Ширина области распознавания номера"

	static std::string anprPlateRectHeightKey;	///< Имя ключа "Высота области распознавания номера"

	static std::string messageTimeToLiveSecKey;	///< Имя ключа "Время жизни сообщения"

	static std::string messageSendTimeoutSecKey;///< Имя ключа "Время до повторной отправки сообщения"

	static std::string storeNoSendedPlateKey;

	static std::string workRelayWaitTimeKey;	///< Имя ключа "Время ожидания в микросекундах для чтения состояния входного реле, после открытия/закрытия
												///< выходного реле"

	static std::string workRelayNumberKey;		///< Имя ключа "Номер реле для отработки при получении команды"

	static std::string portAAutoOpenRelayNumberKey;
	static std::string portBAutoOpenRelayNumberKey;
	static std::string openRelayTimeoutSecKey;

	static std::string sendPlateRectKey;		///< Имя ключа "Посылать в сообщении область номера"

	static std::string sendPlateImageKey;		///< Имя ключа "Посылать в сообщении картинку с номером"

	static std::string checkDirUpKey;			///< Имя ключа "Проверять движение номер вверх по кадру"
	static std::string checkDirDownKey;			///< Имя ключа "Проверять движение номер вниз по кадру"
	static std::string checkDirLeftKey;			///< Имя ключа "Проверять движение номер влево по кадру"
	static std::string checkDirRightKey;		///< Имя ключа "Проверять движение номер вправо по кадру"
	static std::string checkDirRectSizeGoToLargeKey;	///< Имя ключа "Проверять увеличение размера области номерного знака"
	static std::string checkDirRectSizeGoToSmallKey;	///< Имя ключа "Проверять уменьшение размера области номерного знака"
	static std::string checkDirBoundKey;				///< Имя ключа "Пороговое значение, при котором направление движения авто считается распознанным"
	static std::string checkDirDataLiveTimeoutKey;		///< Имя ключа "Таймаут удаление данных об определении направления движения"

	static std::string wiringPiPinWorkRelay0Key;
	static std::string wiringPiPinCheckRelay0Key;
	static std::string wiringPiPinWorkRelay1Key;
	static std::string wiringPiPinCheckRelay1Key;

	static std::string wiringPiPinNumberVideoCaptureFromLowToHighKey;	///< Имя ключа "Номер пина, который используется для захвата видео при переходе от
																		///< низкого уровня на высокий"
	static std::string wiringPiPinNumberVideoCaptureFromHighToLowKey;	///< Имя ключа "Номер пина, который используется для захвата видео при переходе от
																		///< высокого уровня на низкий"
	static std::string wiringPiEmptyPlateVideoCaptureKey;	///< Имя ключа "Строка - пустой номер: "NO""
	static std::string wiringPiVideoCaptureDelaySecKey;		///< Задержка по времени в секундах при получении сигнала на реле захвата видео для отправки видео
															///< Примечание - распознанный номер может быть отправлен и раньше

public:
	/**
	 * Конструктор
	 * @param fileName имя файла настроек ("vega-2.ini")
	 */
	Settings(const std::string& fileName);

	/**
	 * Деструктор
	 */
	~Settings();

	/**
	 * Получить уровень журналирования
	 * @return уровень журналирования
	 * =1 - журналировать входящие сообщения (команды)
	 */
	int32_t getLogLevel() const;

	/**
	 * Получить IP-адрес клиента-получателя события "Есть в списке A"
	 * @return IP-адрес клиента-получателя события "Есть в списке A"
	 */
	std::string getHostAddressA() const;

	/**
	 * Получить порт клиента-получателя события "Есть в списке A"
	 * @return порт клиента-получателя события "Есть в списке A"
	 */
	uint16_t getPortA() const;

	/**
	 * Получить IP-адрес клиента-получателя события "Нет в списке A"
	 * @return IP-адрес клиента-получателя события "Нет в списке A"
	 */
	std::string getHostAddressNA() const;

	/**
	 * Получить порт клиента-получателя события "Нет в списке A"
	 * @return порт клиента-получателя события "Нет в списке A"
	 */
	uint16_t getPortNA() const;

	/**
	 * Получить IP-адрес клиента-получателя события "Есть в списке B"
	 * @return IP-адрес клиента-получателя события "Есть в списке B"
	 */
	std::string getHostAddressB() const;

	/**
	 * Получить порт клиента-получателя события "Есть в списке B"
	 * @return порт клиента-получателя события "Есть в списке B"
	 */
	uint16_t getPortB() const;

	/**
	 * Получить IP-адрес клиента-получателя события "Нет в списке B"
	 * @return IP-адрес клиента-получателя события "Нет в списке B"
	 */
	std::string getHostAddressNB() const;

	/**
	 * Получить порт клиента-получателя события "Нет в списке B"
	 * @return порт клиента-получателя события "Нет в списке B"
	 */
	uint16_t getPortNB() const;

	/**
	 * Получить порт для получения запросов на подключение для выполнения команд
	 * @return порт для получения запросов на подключение для выполнения команд
	 */
	uint16_t getPortControl() const;
	uint16_t getPortApkControl() const;

	/**
	 * Получить URL IP-камеры
	 * @return URL IP-камеры
	 */
	std::string getIpCameraUrl() const;

	/**
	 * Получить номер web-камеры
	 * @return номер web-камеры
	 */
	int32_t getWebCameraNumber() const;

	/**
	 * Получить FPS работы потока распознванаия
	 * @return FPS работы потока распознванаия
	 */
	int32_t getFps() const;

	/**
	 * Получить количество потоков распознавания номера
	 * @return количество потоков распознавания номера
	 */
	int32_t getRecognizeThreadNumber() const;

	/**
	 * Получить максимальное количество строк в файле журнала
	 * @return максимальное количество строк в файле журнала
	 */
	int32_t getLogMaxRowNumber() const;

	/**
	 * Получить минимальное число считываний номера для определения номера
	 * @return минимальное число считываний номера для определения номера
	 */
	int32_t getMinPlateSequence() const;

	/**
	 * Получить минимальное значение счетчика распознаваний номера для отправки
	 * номера сервису
	 * @return минимальное значение счетчика распознаваний номера для отправки номера сервису
	 */
	int32_t getMinPlateCountInMap() const;

	/**
	 * Получить таймаут работы счетчиков распознавания
	 * @return таймаут работы счетчиков распознавания
	 */
	int32_t getMaxPlateCountTime() const;

	/**
	 * Получить режим распознавания номеров для библиотеки iANPR
	 * @return режим распознавания номеров для библиотеки iANPR
	 */
	int32_t getAnprDetectMode() const;

	/**
	 * Получить минимальную площадь номера для библиотеки iANPR
	 * @return минимальная площадь номера для библиотеки iANPR
	 */
	int32_t getAnprMinPlateSize() const;

	/**
	 * Получить максимальную площадь номера для библиотеки iANPR
	 * @return максимальная площадь номера для библиотеки iANPR
	 */
	int32_t getAnprMaxPlateSize() const;

	/**
	 * Получить максимальное количество символов в номере для библиотеки iANPR
	 * @return максимальное количество символов в номере для библиотеки iANPR
	 */
	int32_t getAnprMaxTextSize() const;

	/**
	* Получить область распознавания номера
	* @return область распознавания номера
	*/
	CvRect getRect() const;

	/**
	 * Получить время жизни сообщения в секундах
	 * @return время жизни сообщения в секундах
	 */
	int32_t getMessageTimeToLiveSec() const;

	/**
	 * Получить время до повторной отправки сообщения
	 * @return время до повторной отправки сообщения
	 */
	int32_t getMessageSendTimeoutSec() const;

	int32_t getStoreNoSendedPlate() const;
	int32_t getMinFreeDiskSize() const;

	/**
	 * Получить время ожидания в микросекундах для чтения состояния входного реле
	 * после открытия/закрытия выходного реле
	 * Если = -1, то чтение входного реле при получении команды на
	 * открытие/закрытие реле не производится
	 * @return время ожидания в микросекундах для чтения состояния входного реле после открытия/закрытия выходного реле
	 */
	int32_t getWorkRelayWaitTimeMilliSec() const;

	/**
	 * Получить номер реле для отработки при получении команды
	 * =0 – включать реле по номеру, полученному в команде (Открыть/закрыть реле)
	 * =1 – всегда открывать/закрывать реле 0
	 * =2 – всегда открывать/закрывать реле 1
	 * =3 - всегда открывать/закрывать оба реле
	 * @return номер реле для отработки при получении команды
	 */
	int32_t getWorkRelayNumber() const;

	int32_t getPortAAutoOpenRelayNumber() const;
	int32_t getPortBAutoOpenRelayNumber() const;
	int32_t getOpenRelayTimeoutSec() const;

	/**
	 * Посылать в сообщении область номера
	 * @return =1 - посылать в сообщении область номера
	 */
	int32_t getSendPlateRect() const;

	/**
	 * Посылать в сообщении картинку с номером
	 * @return =1 - посылать в сообщении картинку с номером
	 */
	int32_t getSendPlateImage() const;

	int32_t getCheckDirUp() const;
	int32_t getCheckDirDown() const;
	int32_t getCheckDirLeft() const;
	int32_t getCheckDirRight() const;
	int32_t getCheckDirRectSizeGoToLarge() const;
	int32_t getCheckDirRectSizeGoToSmall() const;
	int32_t getCheckDirBound() const;
	int32_t getCheckDirDataLiveTimeout() const;

	int32_t getWiringPiPinWorkRelay0() const;
	int32_t getWiringPiPinCheckRelay0() const;
	int32_t getWiringPiPinWorkRelay1() const;
	int32_t getWiringPiPinCheckRelay1() const;

	int32_t getWiringPiPinNumberVideoCaptureFromLowToHigh() const;
	int32_t getWiringPiPinNumberVideoCaptureFromHighToLow() const;
	std::string getWiringPiEmptyPlateVideoCapture() const;
	int32_t getWiringPiVideoCaptureDelaySec() const;

private:
	/**
	 * Получить IP-адрес клиента по ключу
	 * @param key ключ
	 * @return IP-адрес клиента
	 */
	std::string getHostAddress(const std::string& key) const;

	/**
	 * Получить порт
	 * @param key ключ
	 * @return порт
	 */
	uint16_t getPort(const std::string& key) const;

	/**
	 * Получить число uint32_t
	 * @param key ключ
	 * @param defaultValue значение по умолчанию
	 * @return число uint32_t
	 */
	int32_t getInt32(const std::string& key, int32_t defaultValue) const;

	boost::property_tree::ptree m_propertyTree;	///< Списое ключ-значение
};

#endif // SETTINGS_H
