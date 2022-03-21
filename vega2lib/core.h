#ifndef CORE_H
#define CORE_H

#include "vega2libdef.h"

#include "vegacommandexecutor.h"
#include "vegacommand40000_datamessage.h"

#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

#include <time.h>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <deque>

#include <iANPR.h>

#ifndef NO_GUARDANT
#include <grdapi.h>
#include <grddongle.h>

#define C2 0x15DCB
#define CryptPU 0x5B17C98Au
#define CryptRD 0xA9903C82u
#endif

class PlateReadyVisitor;
class TcpCommandServer;
class Settings;
class VegaCommand;
class VegaPacket;
class PlateTimeData;

#ifdef USE_MONGODB
class DbInterface;
#endif

struct DirectionInfoStruct
{
	int dirScore;
	CvRect lastPlateRect;
	std::time_t lastDateTime;
	VegaCommand40000_DataMessage::DirectionType direction;
};

void videoCaptureWiringPi();

class GPIOClass;

/**
 * Класс ядра приложения VEGA-2
 */
class VEGA2LIB_EXPORT Core : public VegaCommandExecutor
{
	static std::string listAFile;	///< Название файла со списком А
	static std::string listBFile;	///< Название файла со списком B
	static std::string listImeiFile;
	static std::string iniFile;
	static std::string bakIniFile;
	static std::string logFile;
	static std::string oldLogFile;
	static std::string base64Chars;

	static std::string portAString;
	static std::string portNAString;
	static std::string portBString;
	static std::string portNBString;
	static char* ver17;

private:
	/**
	 * Конструктор
	 */
	Core();

public:
	static Core* getInstance();

	static void releaseInstance();

	/**
	 * Деструктор
	 */
	virtual ~Core();

#ifndef NO_GUARDANT
	int guardantErrorHandling(CGrdDongle* grdDongle, int errorCode);
	bool guardantDemix(unsigned char* inArray, size_t inArrayLen, unsigned char* outArray1, size_t outArray1Len, unsigned char* outArray2,
		size_t outArray2Len);
	void guardantRotateCharArray(unsigned char* array, size_t arrayLen);
#endif

	std::string getGuardantLicenceInfo();
	std::string getLicenceInfo();
	std::string parseLicenceString(unsigned char* data, size_t dataLen);

	int char2int(unsigned char input) const;
	int hex2byte(const unsigned char* src, unsigned char* dst) const;
	bool isBase64(unsigned char c) const;
	std::vector<char> base64Decode(const std::string& encodedString) const;

	/**
	 * Установить объект, который должен знать о событиях обрабоки изображения:
	 * новый распознанный номер или есть новый кадр для отображения
	 * @param plateReadyVisitor указатель на объект, который должен знать о событиях обрабоки
	 */
	void setPlateReadyVisitor(PlateReadyVisitor* plateReadyVisitor);

	/**
	 * Открыть файл с изображением для поиска номера на нем
	 * @param fileName путь к файлу с изображением
	 */
	void openFile(const std::string& fileName);

	/**
	 * Открыть поток на получение видео с ip-камеры или web-камеры.
	 */
	void startCapture();

	/**
	 * Ожидать окончания обработки
	 */
	void joinVideoCaptureThread();

	/**
	 * Открыть поток на получение видео с ip-камеры.
	 * URL IP-камеры указан в файле vega-2.ini ключ IP_CAMERA_URL
	 * @return =TRUE - поток на получение видео открыт успешно
	 */
	bool startCaptureStream();

	/**
	 * Открыть поток на получение видео с web-камеры.
	 * Номер web-камеры указан в файле vega-2.ini ключ WEB_CAMERA_NUMBER
	 * @return =TRUE - поток на получение видео открыт успешно
	 */
	bool startCaptureWebCam();

	/**
	 * Получить область рапознавания номера на изображении
	 * @return область рапознавания номера на изображении
	 */
	const CvRect& getSettingsRect() const;

	virtual DigitalInputOutputStatus startDigitalOutput(uint32_t number);
	virtual DigitalInputOutputStatus stopDigitalOutput(uint32_t number);
	virtual DigitalInputOutputStatus readDigitalInputOutput();
	virtual SingleImage getSingleImage();
	virtual void resetDevice();

	virtual std::string getHomeDir() const;

	/**
	 * @copydoc VegaCommandExecutor::reloadAList()
	 */
	virtual void reloadAList();

	/**
	 * @copydoc VegaCommandExecutor::reloadBList()
	 */
	virtual void reloadBList();

	virtual void addLogMessage(const std::string& message, bool withTime = true);

	virtual bool checkImei(const std::string& imei);

	/**
	 * Процесс/поток получения видео с камеры
	 */
	void processVideoCapture();

	/**
	 * Распознавание номера
	 * @param image изображение для распознавания номера
	 * @return =TRUE- номер был найден на картинке
	 */
	bool plateRecognize(const cv::Mat& image);

#ifndef NO_GUARDANT
	void verifySign();
#endif

	/**
	 * Процесс/поток получения распознавания номера
	 */
	void processPlateRecognize();
	
	/**
	 * Процесс/поток отправки номера и картинки на сервис
	 * @param index индес потока отправки сообщений
	 */
	void processSendVegaPacket(int index);

	void setTestPlateString(const std::string& testPlateString);
	void setTestPlateRect(const CvRect& testPlateRect);

	void setWiringPiNeedVideoCapture();

private:
	static Core* m_instance;

	/**
	 * Перечитать список A/B
	 * @param filename имя файла
	 * @return список номеров
	 */
	std::vector<std::string> reloadList(const std::string& filename);

	void updateDirection(std::time_t currentDateTime, const std::string& plateString, const CvRect& plateRect);

	/**
	 * Проверить наличия номера в списоке A и B и создание соответвующего
	 * сообщения для клиента
	 * @param plateString номер
	 * @param direction направление движения
	 * @param plateRect область номера
	 * @param srcImage черно-белое изображение
	*/
	void testForListsAndCreateMessage(const std::string& plateString, VegaCommand40000_DataMessage::DirectionType direction, const CvRect& plateRect,
		const cv::Mat& srcImage);

	void clearMessageQueueForTimeout(int index);
	int findPlateTimeDataQueueForTimeout(int index, const std::string& plate);

	void testForAddSendVegaPacketThread(const std::string& host, uint16_t port, int portIndex);

	void checkOpenRelayTimeoutAndClose();

	PlateReadyVisitor* m_plateReadyVisitor;	///< Указатель на объект, который должен знать о событиях обрабоки

	Settings* m_settings;					///< Настройки программы (vega-2.ini)
	TcpCommandServer* m_tcpCommandServer;	///< Сервер для получения команд от клиента
	TcpCommandServer* m_tcpApkCommandServer;

	uint16_t m_logLevel;				///< Уровень журналирования (=1 - журналировать изображения в файлы)
	std::vector<std::string> m_aList;	///< Список A
	boost::mutex m_aListMutex;			///< Мютекс Список A
	std::vector<std::string> m_aListTemp;
	bool m_aListReinited;
	std::vector<std::string> m_bList;	///< Список B
	boost::mutex m_bListMutex;			///< Мютекс Список B
	std::vector<std::string> m_bListTemp;
	bool m_bListReinited;

	std::vector<std::string> m_imeiList;

	cv::Mat m_srcImage;						///< Текущее изображение
	std::time_t m_srcImageDateTime;			///< Время захвата текущего изображнния
	boost::mutex m_srcImageMutex;			///< Мютекс работы с изображением
	bool m_srcImageReadyForPlateRecognize;	///< Флаг готовности избражения для начала распознавания номера
											///< Поток получения устанавливает =true, а процесс распознавания ждет когда =true, начинает распознавать номер,
											///< а после окончания =false

	cv::VideoCapture m_videoCapture;		///< Получение видео
	boost::asio::io_service m_ioService;	///< Сетевой сервис системы

	boost::thread m_videoCaptureThread;		///< Поток получения видео

	std::vector<boost::thread> m_plateRecognizeThreadVector;	///< Поток распознавания номера

	std::vector<int> m_sendVegaPacketIndexVector;	///< Индесы потоков отправки сообщений
	std::vector<std::string> m_portNameVector;

	std::vector<boost::thread> m_sendVegaPacketThreadVector;	///< Потоки для отправки сообщений с номерами на сервис

	std::vector<boost::mutex*> m_messageQueueMutexVector;	///< Мютекс очереди сообщений для отправки номерами на сервис

	std::vector<std::queue<VegaPacket*> > m_messageQueueVector;
											///< Очередь сообщений с номерами
											///< Заполняется потоком
											///< m_plateRecognizeThread
											///< Опустошается потоками
											///< m_sendVegaPacketThreadVector

	std::vector<boost::mutex*> m_plateTimeDataQueueMutexVector;
											///< Мютекс очереди распознанных номеров

	std::vector<std::deque<PlateTimeData*> > m_plateTimeDataQueueVector;
											///< Очередь распознанных номеров
											///< Заполняется потоками
											///< m_plateRecognizeThread
											///< Проверяется потоком
											///< m_plateRecognizeThread
											///< для добавления номера в очередь
											///< на отправку

	std::ofstream m_logFile;		///< Лог-файл
	boost::mutex m_logFileMutex;	///< Мютекс лог-файла

	std::string m_lastPlateString;	///< Последний распознанный номер
	int m_minPlateSequence;			///< Минимальное число считываний номера для определения номера

	int m_plateCount;		///< Число считываний номера

	std::map<std::string, int> m_plateCountMap;	///< Счетчики распознанных номеров
	std::map<std::string, DirectionInfoStruct> m_directionMap;	///< Счетчики распознавания направления движения

	std::time_t m_createPlateMapDateTime;		///< Время начала подсчета номеров

	std::string m_plateCountMapMaxValueKey;		///< Номер имеющий максимальный счетчик

	boost::mutex m_plateMutex;					///< Мютекс распознанных номеров

	int m_fps;		///< FPS для определения usleep в потоках распознавания и отправки сообщений

	int m_logMaxRowNumber;
	int m_logCurrentRowNumber;

	int m_minPlateCountInMap;	///< Сколоко нужно получить номер в счетчике m_plateCountMap, чтобы считать номер считанным

	int m_maxPlateCountTime;	///< Таймаут работы счетчиков распознавания

	ANPR_OPTIONS m_anprOption;	///< Опции библиотеки распознавания номеров
	CvRect m_settingsRect;		///< Область рапознавания номера на изображении

	int m_messageTimeLiveSec;
	int m_messageSendTimeoutSec;
	int m_workRelayWaitTimeMilliSec;
	int m_storeNoSendedPlate;

	int m_workRelayNumber;
	int m_portAAutoOpenRelayNumber;
	int m_portBAutoOpenRelayNumber;
	int m_openRelayTimeoutSec;
	std::time_t m_openRelay0Time;
	std::time_t m_openRelay1Time;

	int m_sendPlateRect;
	int m_sendPlateImage;

	bool m_checkDirFlag;
	int m_checkDirUp;
	int m_checkDirDown;
	int m_checkDirLeft;
	int m_checkDirRight;
	int m_checkDirRectSizeGoToLarge;
	int m_checkDirRectSizeGoToSmall;
	int m_checkDirBound;
	int m_checkDirDataLiveTimeout;

	int m_wiringPiPinWorkRelay0;
	int m_wiringPiPinCheckRelay0;
	int m_wiringPiPinWorkRelay1;
	int m_wiringPiPinCheckRelay1;

	GPIOClass* m_wiringPiWorkRelay0;
	GPIOClass* m_wiringPiCheckRelay0;
	GPIOClass* m_wiringPiWorkRelay1;
	GPIOClass* m_wiringPiCheckRelay1;

	int m_wiringPiPinNumberVideoCaptureFromLowToHigh;
	int m_wiringPiPinNumberVideoCaptureFromHighToLow;
	std::string m_wiringPiEmptyPlateVideoCapture;
	int m_wiringPiVideoCaptureDelaySec;
	std::time_t m_wiringPiCommandTime;
	bool m_wiringPiNeedVideoCapture;

	bool m_finishAllThreads;

	char* m_iANPRKey;

#ifdef USE_PIFACE_DIGITAL
	int m_piFaceHwAddr;
#endif

#ifndef NO_GUARDANT
	DWORD m_dongleId;
	boost::mutex m_dongleVerifyMutex;
	char m_dongleMessage[GrdECC160_MESSAGE_SIZE];
	char m_dongleDigSignMessage[GrdECC160_DIGEST_SIZE];
	int m_dongleVerifyCount;
	int m_verifyState;
	unsigned char* m_pubEccKeyByteArray;
	int m_pubEccKeyByteArrayLen;
	int m_grdRdKey;
	CGrdDongle* m_grdDongle;
	bool m_alg0Activated;
	boost::thread m_verifySignThread;
#endif

	std::string m_testPlateString;
	CvRect m_testPlateRect;

#ifdef USE_MONGODB
	DbInterface* m_dbInterface;
	bool m_dbInited;

	void initDb();
#endif
};

#endif // CORE_H
