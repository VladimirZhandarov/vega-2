#include "vegapointtag.h"

uint32_t VegaPointTag::POINT = 10069;

VegaPointTag::VegaPointTag() : VegaBaseTag(POINT)
{

}

VegaPointTag::VegaPointTag(uint32_t xCoordinate, uint32_t yCoordinate) : VegaBaseTag(POINT)
{
	m_len = 8;
	m_data.xCoordinate = xCoordinate;
	m_data.yCoordinate = yCoordinate;
}

VegaPointTag::~VegaPointTag()
{

}

uint32_t VegaPointTag::getXCoordinate() const
{
	return m_data.xCoordinate;
}

uint32_t VegaPointTag::getYCoordinate() const
{
	return m_data.yCoordinate;
}

uint32_t VegaPointTag::getSize() const
{
	return 8 + sizeof(Data);
}

void VegaPointTag::parse(std::istream& inData)
{
	VegaBaseTag::parse(inData);
	if(m_len == sizeof(Data))
	{
		inData.read((char*)&m_data, sizeof(Data));
	}
}

void VegaPointTag::serialize(std::ostream& outData)
{
	VegaBaseTag::serialize(outData);
	outData.write((char*)&m_data, sizeof(Data));
}
