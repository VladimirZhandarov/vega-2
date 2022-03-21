#ifndef VEGAPACKET_H
#define VEGAPACKET_H

#include <stdint.h>
#include <string>

class VegaCommand;

/**
 * Объект сообщением для оправки сервису
 */
class VegaPacket
{
public:
	/**
	 * Конструктор
	 * @param command сообщение
	 * @param host хост получателя сообщения
	 * @param port порт получателя сообщения
	*/
	VegaPacket(VegaCommand* command, const std::string& host, uint16_t port);

	/**
	 * Деструктор
	 */
	~VegaPacket();

	/**
	 * Получить сообщение
	 * @return сообщение
	 */
	VegaCommand* getVegaCommand() const;

	/**
	 * Получить хост получателя сообщения
	 * @return хост получателя сообщения
	 */
	const std::string& getHost() const;

	/**
	 * Получить порт получателя сообщения
	 * @return порт получателя сообщения
	 */
	uint16_t getPort() const;

protected:
	VegaCommand* m_command;	///< Сообщение
	std::string m_host;		///< Xост получателя сообщения
	uint16_t m_port;		///< Порт получателя сообщения
};

#endif // VEGAPACKET_H
