#ifndef TCPCOMMANDSERVER_H
#define TCPCOMMANDSERVER_H

#include <stdint.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>

class TcpCommandSession;
class VegaCommandExecutor;

/**
 * Сервер для получения команд от клиента
 */
class TcpCommandServer
{
public:
	/**
	 * Конструктор
	 * @param protocolVersion
	 * @param executor объект реализующий бизнес-логику выполнения команды на сервере
	 * @param port порт для получения запросов на подключение
	 * @param logLevel уровень журналирования
	 *                 =1 - журналировать входящие сообщения (команды)
	 */
	TcpCommandServer(int32_t protocolVersion, VegaCommandExecutor* executor, uint16_t port, uint16_t logLevel);

	/**
	 * Деструктор
	 */
	~TcpCommandServer();

	void closeSession(boost::shared_ptr<TcpCommandSession> session);

	/**
	 * Начать поток
	 */
	void start();

	/**
	 * Окончить поток
	 */
	void stop();

	bool isEmeiProtocol() const;

private:
	/**
	 * Начать прослушивание запросов на подключение
	 */
	void startAccept();

	/**
	 * Обработчик события для обработки запроса на подключение
	 * @param session сессия приема и обработки команды от клиента
	 * @param error код ошибки работы с сокетом
	 */
	void handleAccept(boost::shared_ptr<TcpCommandSession> session, const boost::system::error_code& error);

	int32_t m_protocolVersion;

	boost::asio::io_service m_ioService;		///< Сетевой сервис системы
	boost::scoped_ptr<boost::thread> m_thread;	///< Поток сервера
	boost::asio::ip::tcp::acceptor m_acceptor;	///< Получение запросов на подключение

	VegaCommandExecutor* m_executor;	///< Объект реализующий бизнес-логику выполнения команды на сервере
	uint16_t m_logLevel;				///< Уровень журналирования (=1 - журналировать входящие сообщения)
	boost::mutex m_sessionMutex;
	std::list<boost::shared_ptr<TcpCommandSession> > m_sessionList;
};

#endif // TCPCOMMANDSERVER_H
