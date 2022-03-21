#ifndef VEGADATATAG_H
#define VEGADATATAG_H

#include "vegabasetag.h"

#include <stdint.h>

/**
 * Тег команды (передается по сети от сервера к клиенту и обратно)
 */
class VegaDataTag : public VegaBaseTag
{
public:
	static uint32_t JPEG_QUALITY;
	static uint32_t DIGITAL_INPUT_NUMBER;
	static uint32_t DIGITAL_OUTPUT_NUMBER;
	static uint32_t DIGITAL_INPUT_STATUS;
	static uint32_t DIGITAL_OUTPUT_STATUS;
	static uint32_t DIGITAL_OUTPUT_SEL_NUMBER; ///< Тег "Номер реле" 

	static uint32_t PLATE_MIN_X;		///< Область номера машины на картинке с камеры
	static uint32_t PLATE_MIN_Y;		///< Область номера машины на картинке с камеры
	static uint32_t PLATE_MAX_X;		///< Область номера машины на картинке с камеры
	static uint32_t PLATE_MAX_Y;		///< Область номера машины на картинке с камеры

	/**
	 * Конструктор
	 */
	VegaDataTag();

	/**
	 * @copydoc VegaTag::VegaTag(uint32_t)
	 */
	VegaDataTag(uint32_t code);

	/**
	 * Конструктор
	 * @param code код тега
	 * @param value значение
	 */
	VegaDataTag(uint32_t code, uint32_t value);

	/**
	 * Деструктор
	 */
	virtual ~VegaDataTag();

	/**
	 * Получить значение
	 * @return значение
	 */
	uint32_t getValue() const;

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
	uint32_t m_value;	///< Значение
};

#endif // VEGADATATAG_H
