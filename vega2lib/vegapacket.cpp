#include "vegapacket.h"

#include "vegacommand.h"

VegaPacket::VegaPacket(VegaCommand* command, const std::string& host, uint16_t port) : m_command(command), m_host(host), m_port(port)
{

}

VegaPacket::~VegaPacket()
{
	delete m_command;
}

VegaCommand* VegaPacket::getVegaCommand() const
{
	return m_command;
}

const std::string& VegaPacket::getHost() const
{
	return m_host;
}

uint16_t VegaPacket::getPort() const
{
	return m_port;
}
