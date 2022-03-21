#include "vegabasetag.h"

VegaBaseTag::VegaBaseTag() : VegaSwap()
{
	m_code = 0;
	m_len = 0;
}

VegaBaseTag::VegaBaseTag(uint32_t code) : VegaSwap()
{
	m_code = code;
	m_len = 0;
}

VegaBaseTag::~VegaBaseTag()
{

}

void VegaBaseTag::setCode(uint32_t code)
{
	m_code = code;
}

uint32_t VegaBaseTag::getCode() const
{
	return m_code;
}

uint32_t VegaBaseTag::getLen() const
{
	return m_len;
}

bool VegaBaseTag::isNull() const
{
	return (m_code == 0);
}

void VegaBaseTag::parse(std::istream& inData)
{
	m_len = readSwap(inData);
}

void VegaBaseTag::serialize(std::ostream& outData)
{
	writeSwap(outData, m_code);
	writeSwap(outData, m_len);
}
