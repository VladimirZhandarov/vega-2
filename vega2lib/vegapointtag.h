#ifndef VEGAPOINTTAG_H
#define VEGAPOINTTAG_H

#include "vegabasetag.h"

#include <stdint.h>

class VegaPointTag : public VegaBaseTag
{
public:
	static uint32_t POINT;

	/**
	 * Конструктор
	 */
	VegaPointTag();

	/**
	 * Конструктор
	 * @param xCoordinate координата по оси X
	 * @param yCoordinate координата по оси Y
	 */
	VegaPointTag(uint32_t xCoordinate, uint32_t yCoordinate);

	/**
	 * Деструктор
	 */
	virtual ~VegaPointTag();

	/**
	 * Получить координату по оси X
	 * @return значение
	 */
	uint32_t getXCoordinate() const;

	/**
	 * Получить координату по оси Y
	 * @return значение
	 */
	uint32_t getYCoordinate() const;

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
		Data()
		{
			xCoordinate = 0;
			yCoordinate = 0;
		}

		uint32_t xCoordinate; ///< Координата по оси X
		uint32_t yCoordinate; ///< Координата по оси Y
	};

	Data m_data;
};

#endif // VEGAPOINTTAG_H
