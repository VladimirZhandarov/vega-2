#include "vegaheader.h"

uint32_t VegaHeader::HEADER_SIZE = 24;

VegaHeader::VegaHeader() : VegaSwap()
{
	m_headerDimension = HEADER_SIZE;
	m_commandCode = 0;
	m_sender = 0;
	m_receiver = 0;
	m_error = 0;
	m_dataDimension = 0;
}

VegaHeader::VegaHeader(std::istream& inData) : VegaSwap()
{
	m_headerDimension = readSwap(inData);
	m_commandCode = readSwap(inData);
	m_sender = readSwap(inData);
	m_receiver = readSwap(inData);
	m_error = readSwap(inData);
	m_dataDimension = readSwap(inData);
}

VegaHeader::~VegaHeader()
{

}

uint32_t VegaHeader::getHeaderDimension() const
{
	return m_headerDimension;
}

void VegaHeader::setCommandCode(uint32_t commandCode)
{
	m_commandCode = commandCode;
}

uint32_t VegaHeader::getCommandCode() const
{
	return m_commandCode;
}

uint32_t VegaHeader::getSender() const
{
	return m_sender;
}

uint32_t VegaHeader::getReceiver() const
{
	return m_receiver;
}

uint32_t VegaHeader::getError() const
{
	return m_error;
}

void VegaHeader::setDataDimension(uint32_t dataDimension)
{
	m_dataDimension = dataDimension;
}

uint32_t VegaHeader::getDataDimension() const
{
	return m_dataDimension;
}

bool VegaHeader::isNull() const
{
	return (m_commandCode == 0);
}

void VegaHeader::serialize(std::ostream& outData)
{
	writeSwap(outData, m_headerDimension);
	writeSwap(outData, m_commandCode);
	writeSwap(outData, m_sender);
	writeSwap(outData, m_receiver);
	writeSwap(outData, m_error);
	writeSwap(outData, m_dataDimension);
}
