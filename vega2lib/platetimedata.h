#ifndef PLATETIMEDATA_H
#define PLATETIMEDATA_H

#include <stdint.h>
#include <string>
#include <ctime>

class PlateTimeData
{
public:
	/**
	 * Конструктор
	 * @param plateNumber номер машины
	 * @param dateTime время
	*/
	PlateTimeData(const std::string& plateNumber, const std::time_t& dateTime);

	/**
	 * Деструктор
	 */
	~PlateTimeData();

	/**
	 * Получить номер машины
	 * @return номер машины
	 */
	const std::string& getPlateNumber() const;

	/**
	 * Получить время
	 * @return время
	 */
	const std::time_t& getDateTime() const;

protected:
	std::string m_plateNumber;
	std::time_t m_dateTime;
};

#endif // PLATETIMEDATA_H
