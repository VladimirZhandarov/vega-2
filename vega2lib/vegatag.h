#ifndef VEGATAG_H
#define VEGATAG_H

#include "vegabasetag.h"

#include <stdint.h>
#include <string>
#include <vector>

/**
 * Тег команды (передается по сети от сервера к клиенту и обратно)
 */
class VegaTag : public VegaBaseTag
{
public:
	static uint32_t FILE_NAME_TAG;	///< Тег "Имя файла"
	static uint32_t FILE_DATA_TAG;	///< Тег "Данные файла"

	static uint32_t DATE_TAG;			///< Тег "Дата"
	static uint32_t TIME_TAG;			///< Тег "Время"
	static uint32_t PLATE_STRING_TAG;	///< Тег "Номер"
	static uint32_t PLATE_COUNTRY_TAG;	///< Тег "Код страны"

	static uint32_t DIRECTION_TAG;	///< Тег "Направление движения"

	static uint32_t IMAGE_BW_TAG;		///< Полная картинка с камеры
	static uint32_t IMAGE_COL_TAG;		///< Картинка номером машины

	static uint32_t IMEI_TAG;

	static std::string DIRECTION_GOAWAY_VALUE;
	static std::string DIRECTION_APROACH_VALUE;
	static std::string DIRECTION_UNKNOW_VALUE;

	/**
	 * Конструктор
	 */
	VegaTag();

	/**
	 * @copydoc VegaTag::VegaTag(uint32_t)
	 */
	VegaTag(uint32_t code);

	/**
	 * Конструктор
	 * @param code код тега
	 * @param data строковые данные
	 * @param len длина строковых данных
	 */
	VegaTag(uint32_t code, const std::string& data, int32_t len = -1);

	/**
	 * Конструктор
	 * @param code код тега
	 * @param data двоичные данные
	 * @param len актуальная длина двоичных данных
	 */
	VegaTag(uint32_t code, const std::vector<unsigned char>& data, int32_t len = -1);

	/**
	 * Деструктор
	 */
	virtual ~VegaTag();

	/**
	 * Получить двоичные данные
	 * @return двоичные данные
	 */
	const std::vector<unsigned char>& getData() const;

	/**
	 * Получить данные в строковом виде
	 * @return данные в строковом виде
	 */
	std::string getDataString() const;

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
	std::vector<unsigned char> m_data;	///< Данные
};

#endif // VEGATAG_H
