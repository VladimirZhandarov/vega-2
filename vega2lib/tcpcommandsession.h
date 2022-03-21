#ifndef TCPCOMMANDSESSION_H
#define TCPCOMMANDSESSION_H

#include "vegaheader.h"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class TcpCommandServer;
class VegaCommandExecutor;

/**
 * Сессия приема и обработки команды от клиента
 */
class TcpCommandSession : public boost::enable_shared_from_this<TcpCommandSession>
{
public:
	/**
	 * Конструктор
	 * @param executor объект реализующий бизнес-логику выполнения команды на сервере
	 * @param logLevel уровень журналирования (=1 - журналировать входящие сообщения)
	 * @param ioService сетевой сервис системы
	 */
	TcpCommandSession(TcpCommandServer* server, VegaCommandExecutor* executor, uint16_t logLevel, boost::asio::io_service& ioService);

	/**
	 * Деструктор
	 */
	~TcpCommandSession();

	/**
	 * Получить сокет
	 * @return сокет
	 */
	boost::asio::ip::tcp::socket& getSocket();

	/**
	 * Начать сессию
	 */
	void start();

private:
	/**
	 * Обработчик события для чтения полученных данных
	 * @param error код ошибки работы с сокетом
	 * @param bytes количестов байт готовых к чтению из сокета
	 */
	void handleRead(const boost::system::error_code& error, size_t bytes);

	std::string bufferToString(const boost::asio::streambuf& buffer);

	boost::asio::ip::tcp::socket m_socket;	///< Сетевой сервис системы
	enum { maxLength = 4096 };				///< Максимальный размер блока для получения из сокета

	boost::asio::streambuf m_streamBuffer;	///< Буфер потока данных из сокета
	uint32_t m_streamBufferLen;
	VegaHeader m_currentHeader;				///< Заголовок команды, получерие которой уже начато

	TcpCommandServer* m_server;
	VegaCommandExecutor* m_executor;	///< Объект реализующий бизнес-логику выполнения команды на сервере
	uint16_t m_logLevel;				///< Уровень журналирования (=1 - журналировать входящие сообщения)
};

#endif // TCPCOMMANDSESSION_H
