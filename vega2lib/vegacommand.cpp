#include "vegacommand.h"

#include "vegatagfactory.h"
#include "vegatag.h"
#include "vegacommandexecutor.h"
#include "tcpcommandserver.h"

VegaCommand::VegaCommand(uint32_t commandCode) : VegaSwap()
{
	m_tcpCommandServer = 0;
	m_executor = 0;
	m_header.setCommandCode(commandCode);
}

VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header) : VegaSwap(),
	m_tcpCommandServer(0), m_executor(executor), m_header(header)
{

}

VegaCommand::~VegaCommand()
{
	for(std::map<uint32_t, VegaBaseTag*>::iterator iter = m_tagMap.begin(); iter != m_tagMap.end(); iter++)
	{
		delete iter->second;
		iter->second = 0;
	}
	m_tagMap.clear();
}

void VegaCommand::setTcpCommandServer(TcpCommandServer* tcpCommandServer)
{
	m_tcpCommandServer = tcpCommandServer;
}

void VegaCommand::setExecutor(VegaCommandExecutor* executor)
{
	m_executor = executor;
}

void VegaCommand::setHeader(const VegaHeader& header)
{
	m_header = header;
}

uint32_t VegaCommand::getCommandCode() const
{
	return m_header.getCommandCode();
}

void VegaCommand::addTag(VegaBaseTag* tag)
{
	if(tag)
	{
		m_tagMap[tag->getCode()] = tag;
	}
}

void VegaCommand::parse(std::istream& inData)
{
	VegaTagFactory& vegaTagFactory = VegaTagFactory::getInstance();
	while(!inData.eof())
	{
		VegaBaseTag* tag = vegaTagFactory.createVegaTag(inData);
		if(tag && tag->getCode() > 0)
		{
			m_tagMap[tag->getCode()] = tag;
		}
		else
		{
			delete tag;
			break;
		}
	};
}

uint32_t VegaCommand::serialize(std::ostream& outData)
{
	uint32_t dataDimension = 0;
	for(std::map<uint32_t, VegaBaseTag*>::iterator iter = m_tagMap.begin(); iter != m_tagMap.end(); iter++)
	{
		if(iter->second)
		{
			dataDimension += iter->second->getSize();
		}
	}
	m_header.setDataDimension(dataDimension);
	m_header.serialize(outData);
	for(std::map<uint32_t, VegaBaseTag*>::iterator iter = m_tagMap.begin(); iter != m_tagMap.end(); iter++)
	{
		if(iter->second)
		{
			iter->second->serialize(outData);
		}
	}
	return dataDimension;
}

VegaCommand* VegaCommand::execute()
{
	return 0;
}

int VegaCommand::getTagMapSize() const
{
	return m_tagMap.size();
}

bool VegaCommand::checkImei()
{
	if(m_tcpCommandServer && m_tcpCommandServer->isEmeiProtocol())
	{
		VegaTag* imeiTag = getTag<VegaTag>(VegaTag::IMEI_TAG);
		if(m_executor && imeiTag)
		{
			return m_executor->checkImei(imeiTag->getDataString());
		}
		return false;
	}
	return true;
}
