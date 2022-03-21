#ifndef VEGACOMMANDEXECUTOR_H
#define VEGACOMMANDEXECUTOR_H

#include <stdint.h>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

struct DigitalInputOutputStatus
{
	uint32_t digitalInputNumber;
	uint32_t digitalInputStatus;
	uint32_t digitalOutputNumber;
	uint32_t digitalOutputStatus;
};

struct SingleImage
{
	std::time_t imageDateTime;
	cv::Mat image;
};

/**
* Объект реализующий бизнес-логику выполнения команды на сервере
*/
class VegaCommandExecutor
{
public:
	/**
	 * Конструктор
	 */
	VegaCommandExecutor();

	/**
	 * Деструктор
	 */
	~VegaCommandExecutor();

	//TODO comment
	virtual DigitalInputOutputStatus startDigitalOutput(uint32_t number) = 0;
	virtual DigitalInputOutputStatus stopDigitalOutput(uint32_t number) = 0;
	virtual DigitalInputOutputStatus readDigitalInputOutput() = 0;
	virtual SingleImage getSingleImage() = 0;
	virtual void resetDevice() = 0;

	virtual std::string getHomeDir() const = 0;

	/**
	 * Перечитать лист A
	 */
	virtual void reloadAList() = 0;

	/**
	 * Перечитать лист B
	 */
	virtual void reloadBList() = 0;

	virtual void addLogMessage(const std::string& message, bool withTime = true) = 0;

	virtual bool checkImei(const std::string& imei) = 0;
};

#endif // VEGACOMMANDEXECUTOR_H
