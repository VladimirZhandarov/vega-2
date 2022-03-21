#include "vegadatetimetag.h"

uint32_t VegaDateTimeTag::IMAGE_TIME = 2001;

VegaDateTimeTag::VegaDateTimeTag() : VegaBaseTag()
{

}

VegaDateTimeTag::VegaDateTimeTag(uint32_t code, std::time_t dateTime) : VegaBaseTag(code)
{
	m_len = 12;
	m_data = Data(dateTime);
}

VegaDateTimeTag::~VegaDateTimeTag()
{

}

uint32_t VegaDateTimeTag::getSize() const
{
	return 8 + sizeof(Data);
}

void VegaDateTimeTag::parse(std::istream& inData)
{
	VegaBaseTag::parse(inData);
	if(m_len == sizeof(Data))
	{
		inData.read((char*)&m_data, sizeof(Data));
	}
}

void VegaDateTimeTag::serialize(std::ostream& outData)
{
	VegaBaseTag::serialize(outData);
	outData.write((char*)&m_data, sizeof(Data));
}
