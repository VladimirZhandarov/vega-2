#ifndef VEGASWAP_H
#define VEGASWAP_H

#include <stdint.h>
#include <iostream>

/**
 * Класс для преобразования сетевого порядка и хостового порядка байт для
 * uint32_t
 */
class VegaSwap
{
public:
	/**
	 * Конструктор
	 */
	VegaSwap();

	/**
	 * Деструктор
	 */
	~VegaSwap();

	/**
	 * Получить uint32_t из потока данных с преобразованием в хостовый порядок
	 * байт
	 * @param inData поток данных полученный из сети
	 * @return полученное число
	 */
	uint32_t readSwap(std::istream& inData);

	/**
	 * Записать uint32_t В поток данных с преобразованием в сетевой порядок байт
	 * @param outData поток данных для передачи по сети
	 * @param data число для передачи
	 */
	void writeSwap(std::ostream& outData, uint32_t data);
};

#endif // VEGASWAP_H
