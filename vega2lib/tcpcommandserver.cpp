#include "tcpcommandserver.h"

#include "tcpcommandsession.h"
#include "vegacommandexecutor.h"

#include <boost/bind.hpp>

TcpCommandServer::TcpCommandServer(int32_t protocolVersion, VegaCommandExecutor* executor, uint16_t port, uint16_t logLevel) : m_acceptor(m_ioService,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), m_executor(executor), m_logLevel(logLevel)
{
	m_protocolVersion = protocolVersion;
	startAccept();
}

TcpCommandServer::~TcpCommandServer()
{
	stop();
}

void TcpCommandServer::closeSession(boost::shared_ptr<TcpCommandSession> session)
{
	boost::mutex::scoped_lock lock(m_sessionMutex);
	for(std::list<boost::shared_ptr<TcpCommandSession> >::iterator iter = m_sessionList.begin(); iter != m_sessionList.end(); iter++)
	{
		if(*iter == session)
		{
			m_sessionList.remove(session);
			break;
		}
	}
}

void TcpCommandServer::start()
{
	if(m_thread)
	{
		return;
	}
	m_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioService)));
}

void TcpCommandServer::stop()
{
	{
		boost::mutex::scoped_lock lock(m_sessionMutex);
		m_sessionList.clear();
		if(!m_thread)
		{
			return;
		}
	}
	m_ioService.stop();
	m_thread->join();
	m_ioService.reset();
}

bool TcpCommandServer::isEmeiProtocol() const
{
	return (m_protocolVersion == 2);
}

void TcpCommandServer::startAccept()
{
	boost::shared_ptr<TcpCommandSession> session(new TcpCommandSession(this, m_executor, m_logLevel, m_ioService));
	m_acceptor.async_accept(session->getSocket(), boost::bind(&TcpCommandServer::handleAccept, this, session, boost::asio::placeholders::error));
}

void TcpCommandServer::handleAccept(boost::shared_ptr<TcpCommandSession> session, const boost::system::error_code& error)
{
	boost::mutex::scoped_lock lock(m_sessionMutex);
	if(!error)
	{
		if(m_executor)
		{
			m_executor->addLogMessage(std::string("new connection accept"));
		}
		m_sessionList.push_back(session);
		session->start();
		startAccept();
	}
}
