#ifndef VEGAHEADER_H
#define VEGAHEADER_H

#include "vegaswap.h"

#include <stdint.h>
#include <iostream>

/**
 * Заголовок команды
 */
class VegaHeader : VegaSwap
{
public:
	static uint32_t HEADER_SIZE;
	/**
	 * Конструктор
	 */
	VegaHeader();

	/**
	 * Парсить данные полученные от клиента по сети для получения параметров
	 * заголовока команды
	 * @param inData поток данных полученный от клиента по сети
	 */
	VegaHeader(std::istream& inData);

	/**
	 * Деструктор
	 */
	~VegaHeader();

	/**
	 * Получить размерность заголовка
	 * @return размерность заголовка = 24
	 */
	uint32_t getHeaderDimension() const;

	/**
	 * Установить код команды
	 * @param commandCode код команды
	 */
	void setCommandCode(uint32_t commandCode);

	/**
	 * Получить код команды
	 * @return код команды
	 */
	uint32_t getCommandCode() const;

	/**
	 * Получить код отправителя команды
	 * @return код отправителя команды
	 */
	uint32_t getSender() const;

	/**
	 * Получить код получателя команды
	 * @return код получателя команды
	 */
	uint32_t getReceiver() const;

	/**
	 * Получить код ошибки
	 * @return код получателя команды
	 */
	uint32_t getError() const;

	/**
	 * Установить размерность блока данных
	 * @param dataDimension размерность блока данных
	 */
	void setDataDimension(uint32_t dataDimension);

	/**
	 * Получить размерность блока данных
	 * @return размерность блока данных
	 */
	uint32_t getDataDimension() const;

	/**
	 * Является ли заголовок команды путым
	 * @return =TRUE - заголовок пустой
	 */
	bool isNull() const;

	/**
	 * Сериализовать заголовок команды в пакет готовый для передачи клиенту
	 * @param outData поток для передачи клиенту
	 */
	void serialize(std::ostream& outData);

private:
	uint32_t m_headerDimension;	///< Размерность заголовка = 24
	uint32_t m_commandCode;		///< Код команды
	uint32_t m_sender;			///< Код отправителя команды
	uint32_t m_receiver;		///< Код получателя команды
	uint32_t m_error;			///< Код ошибки; 0 - нет ошибки
	uint32_t m_dataDimension;	///< Размерность блока данных
};

#endif // VEGAHEADER_H
