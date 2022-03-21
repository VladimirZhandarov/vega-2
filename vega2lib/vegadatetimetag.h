#ifndef VEGADATETIMETAG_H
#define VEGADATETIMETAG_H

#include "vegabasetag.h"

#include <stdint.h>
#include <ctime>

class VegaDateTimeTag : public VegaBaseTag
{
public:
	static uint32_t IMAGE_TIME;

	/**
	 * Конструктор
	 */
	VegaDateTimeTag();

	/**
	 * @copydoc VegaTag::VegaTag(uint32_t)
	 */
	VegaDateTimeTag(uint32_t code);

	/**
	 * Конструктор
	 * @param code TODO comment
	 * @param dateTime TODO comment
	 */
	VegaDateTimeTag(uint32_t code, std::time_t dateTime);

	/**
	 * Деструктор
	 */
	virtual ~VegaDateTimeTag();

	/**
	 * @copydoc VegaBaseTag::getSize()
	 */
	virtual uint32_t getSize() const;

	/**
	 * @copydoc VegaBaseTag::parse()
	 */
	virtual void parse(std::istream& inData);

	/**
	 * @copydoc VegaBaseTag::serialize()
	 */
	virtual void serialize(std::ostream& outData);

private:
	struct Data
	{
		Data(std::time_t dateTime = std::time_t(0))
		{
			std::tm* dateTimeTM = std::localtime(&dateTime);
			day = dateTimeTM->tm_mday;
			month = dateTimeTM->tm_mon + 1;
			year = 1900 + dateTimeTM->tm_year;
			hour = dateTimeTM->tm_hour;
			minute = dateTimeTM->tm_min;
			second = dateTimeTM->tm_sec;
			millisecond = 0;
		}

		uint8_t day;		///< 1..31
		uint8_t month;		///< 1..12
		uint16_t year;
		uint16_t hour;		///< 0..23
		uint16_t minute;	///< 0..59
		uint16_t second;	///< 0..59
		uint16_t millisecond;
	};

	Data m_data;
};

#endif // VEGADATETIMETAG_H
