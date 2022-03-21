#include "vegadatatag.h"

uint32_t VegaDataTag::JPEG_QUALITY = 10015;
uint32_t VegaDataTag::DIGITAL_INPUT_NUMBER = 12041;
uint32_t VegaDataTag::DIGITAL_OUTPUT_NUMBER = 12042;
uint32_t VegaDataTag::DIGITAL_INPUT_STATUS = 12043;
uint32_t VegaDataTag::DIGITAL_OUTPUT_STATUS = 12044;
uint32_t VegaDataTag::DIGITAL_OUTPUT_SEL_NUMBER = 12062;

uint32_t VegaDataTag::PLATE_MIN_X = 14010;
uint32_t VegaDataTag::PLATE_MIN_Y = 14011;
uint32_t VegaDataTag::PLATE_MAX_X = 14012;
uint32_t VegaDataTag::PLATE_MAX_Y = 14013;

VegaDataTag::VegaDataTag() : VegaBaseTag()
{
	m_value = 0;
}

VegaDataTag::VegaDataTag(uint32_t code) : VegaBaseTag(code)
{

}

VegaDataTag::VegaDataTag(uint32_t code, uint32_t value) : VegaBaseTag(code)
{
	m_len = 4;
	m_value = value;
}


VegaDataTag::~VegaDataTag()
{

}

uint32_t VegaDataTag::getValue() const
{
	return m_value;
}

uint32_t VegaDataTag::getSize() const
{
	return 12;
}

void VegaDataTag::parse(std::istream& inData)
{
	VegaBaseTag::parse(inData);
	if(m_len > 0)
	{
		m_value = readSwap(inData);
	}
}

void VegaDataTag::serialize(std::ostream& outData)
{
	VegaBaseTag::serialize(outData);
	writeSwap(outData, m_value);
}
