#ifndef VEGABASETAG_H
#define VEGABASETAG_H

#include "vegaswap.h"

#include <stdint.h>
#include <string>
#include <vector>

/**
 * Тег команды
 */
class VegaBaseTag : public VegaSwap
{
public:
	/**
	 * Конструктор
	 */
	VegaBaseTag();

	/**
	 * Конструктор
	 * @param code код тега
	 */
	VegaBaseTag(uint32_t code);

	/**
	 * Деструктор
	 */
	virtual ~VegaBaseTag();

	/**
	 * Установить код тега
	 * @param code код тега
	 */
	void setCode(uint32_t code);

	/**
	 * Получить код тега
	 * @return код тега
	 */
	uint32_t getCode() const;

	/**
	 * Получить длину данных
	 * @return длина данных
	 */
	uint32_t getLen() const;

	/**
	 * Является ли тег путым
	 * @return =TRUE - тег пустой
	 */
	bool isNull() const;

	/**
	 * Получить длину сериализовааного тега для передачи по сети
	 * @return длина сериализовааного тега для передачи по сети
	 */
	virtual uint32_t getSize() const = 0;

	/**
	 * Парсить данные полученные от клиента по сети для получения параметров
	 * тега
	 * @param inData поток данных полученный от клиента по сети
	 */
	virtual void parse(std::istream& inData);

	/**
	 * Сериализовать тег в пакет готовый для передачи клиенту
	 * @param outData поток для передачи клиенту
	 */
	virtual void serialize(std::ostream& outData);

protected:
	uint32_t m_code;	///< Код тега
	uint32_t m_len;		///< Длина данных
};

#endif // VEGABASETAG_H
