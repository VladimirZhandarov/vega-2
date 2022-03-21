#include "tcpcommandsession.h"

#include "vegacommandfactory.h"
#include "tcpcommandserver.h"
#include "vegacommandexecutor.h"

#include <boost/bind.hpp>

TcpCommandSession::TcpCommandSession(TcpCommandServer* server, VegaCommandExecutor* executor, uint16_t logLevel, boost::asio::io_service& ioService) :
	m_server(server), m_executor(executor), m_logLevel(logLevel), m_socket(ioService), m_streamBufferLen(0)
{

}

TcpCommandSession::~TcpCommandSession()
{
	//m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	m_socket.close();
}

boost::asio::ip::tcp::socket& TcpCommandSession::getSocket()
{
	return m_socket;
}

void TcpCommandSession::start()
{
	try
	{
		boost::asio::streambuf::mutable_buffers_type mutableBuffer = m_streamBuffer.prepare(maxLength);
		m_socket.async_read_some(boost::asio::buffer(mutableBuffer), boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	catch(std::exception& e)
	{
		m_executor->addLogMessage(std::string("Exception caught from TCP command session on start: ") + std::string(e.what()));
	}
}

void TcpCommandSession::handleRead(const boost::system::error_code& error, size_t bytes)
{
	try
	{
		if(!m_executor || !m_server)
		{
			return;
		}
		if(error)
		{
			if(error == boost::asio::error::eof)
			{
				m_executor->addLogMessage(std::string("Client closed connection, closing session: ") + error.message());
			}
			else
			{
				m_executor->addLogMessage(std::string("Error reading session data, closing session: ") + error.message());
			}
			m_server->closeSession(shared_from_this());
			return;
		}

		if(m_logLevel == 1)
		{
			char msgBuffer[1000];
			sprintf(msgBuffer, "streamBuffer size %d bytes", m_streamBuffer.size());
			m_executor->addLogMessage(std::string(msgBuffer));
		}

		if(bytes > 0)
		{
			m_streamBuffer.commit(bytes);
			m_streamBufferLen += bytes;
			if(m_logLevel == 1)
			{
				char msgBuffer[1000];
				sprintf(msgBuffer, "port %d read %d(%d) bytes", m_socket.remote_endpoint().port(), bytes, m_streamBufferLen);
				m_executor->addLogMessage(std::string(msgBuffer));
			}
		}
		if(m_currentHeader.isNull())
		{
			if(m_streamBufferLen < VegaHeader::HEADER_SIZE)
			{
				boost::asio::streambuf::mutable_buffers_type mutableBuffer = m_streamBuffer.prepare(maxLength);
				m_socket.async_read_some(boost::asio::buffer(mutableBuffer), boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				//boost::asio::async_read(m_socket, m_streamBuffer, boost::asio::transfer_at_least(1),boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
				//	boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				return;
			}
			std::istream inData(&m_streamBuffer);
			m_currentHeader = VegaHeader(inData);
		}
		if(!m_currentHeader.isNull())
		{
			if(m_logLevel == 1)
			{
				char msgBuffer[1000];
				sprintf(msgBuffer, "DataDimension %d", m_currentHeader.getDataDimension());
				m_executor->addLogMessage(std::string(msgBuffer));
			}
			if(m_currentHeader.getDataDimension() + VegaHeader::HEADER_SIZE > m_streamBufferLen)
			{
				boost::asio::streambuf::mutable_buffers_type mutableBuffer = m_streamBuffer.prepare(maxLength);
				m_socket.async_read_some(boost::asio::buffer(mutableBuffer), boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				//wait all packet data
				//boost::asio::async_read(m_socket, m_streamBuffer, boost::asio::transfer_at_least(1), boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
				//	boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				return;
			}
		}
		VegaCommandFactory& vegaCommandFactory = VegaCommandFactory::getInstance();
		VegaCommand* command = vegaCommandFactory.createVegaCommand(m_server, m_executor, m_currentHeader);
		if(m_logLevel == 1)
		{
			m_executor->addLogMessage(std::string("recv header ") + std::to_string(m_currentHeader.getCommandCode()));
		}
		if(command)
		{
			std::istream inData(&m_streamBuffer);
			command->parse(inData);
			VegaCommand* response = command->execute();
			if(response)
			{
				try
				{
					boost::asio::streambuf streamBuffer;
					std::ostream outData(&streamBuffer);
					response->serialize(outData);
					boost::asio::write(m_socket, streamBuffer);
				}
				catch(std::exception& e)
				{
					m_executor->addLogMessage(std::string("Exception caught from TCP command session on send: ") + std::string(e.what()));
				}
				delete response;
			}
			delete command;
		}
		m_currentHeader = VegaHeader();

		boost::asio::streambuf::mutable_buffers_type mutableBuffer = m_streamBuffer.prepare(maxLength);
		m_socket.async_read_some(boost::asio::buffer(mutableBuffer), boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		//boost::asio::async_read(m_socket, m_streamBuffer, boost::asio::transfer_at_least(1), boost::bind(&TcpCommandSession::handleRead, shared_from_this(),
		//	boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	catch(std::exception& e)
	{
		m_executor->addLogMessage(std::string("Exception caught from TCP command session on main cycle: ") + std::string(e.what()));
	}
}

std::string TcpCommandSession::bufferToString(const boost::asio::streambuf& buffer)
{
	using boost::asio::buffers_begin;
	auto bufs = buffer.data();
	std::string result(buffers_begin(bufs), buffers_begin(bufs) + buffer.size());
	return result;
}
