#include "vegatag.h"

uint32_t VegaTag::FILE_NAME_TAG = 12038;
uint32_t VegaTag::FILE_DATA_TAG = 12039;

uint32_t VegaTag::DATE_TAG = 14000;
uint32_t VegaTag::TIME_TAG = 14001;
uint32_t VegaTag::PLATE_STRING_TAG = 14004;
uint32_t VegaTag::PLATE_COUNTRY_TAG = 14005;

uint32_t VegaTag::DIRECTION_TAG = 14018;

uint32_t VegaTag::IMAGE_BW_TAG = 14020;
uint32_t VegaTag::IMAGE_COL_TAG = 14021;

uint32_t VegaTag::IMEI_TAG = 14030;

std::string VegaTag::DIRECTION_GOAWAY_VALUE = "GOAWAY";
std::string VegaTag::DIRECTION_APROACH_VALUE = "APROACH";
std::string VegaTag::DIRECTION_UNKNOW_VALUE = "UNKNOW";

VegaTag::VegaTag() : VegaBaseTag()
{

}

VegaTag::VegaTag(uint32_t code) : VegaBaseTag(code)
{

}

VegaTag::VegaTag(uint32_t code, const std::string& data, int32_t len) : VegaBaseTag(code)
{
	std::copy(data.begin(), data.end(), std::back_inserter(m_data));
	if(len == -1)
	{
		m_data.push_back(0);
		m_len = m_data.size();
	}
	else
	{
		if(len > m_data.size())
		{
			int size = m_data.size();
			for(int i = 0; i < len - size; i++)
			{
				m_data.push_back(0);
			}
		}
		else
		{
			m_data.resize(len);
		}
		m_len = len;
	}
	int padding = m_data.size() % 4;
	if(padding > 0)
	{
		padding = 4 - padding;
		for(int i=0; i<padding; i++)
		{
			m_data.push_back(0);
		}
	}
}

VegaTag::VegaTag(uint32_t code, const std::vector<unsigned char>& data, int32_t len) : VegaBaseTag(code)
{
	m_data = data;
	if(len == -1)
	{
		m_len = m_data.size();
	}
	else
	{
		if(len > m_data.size())
		{
			int size = m_data.size();
			for(int i = 0; i < len - size; i++)
			{
				m_data.push_back(0);
			}
		}
		else
		{
			m_data.resize(len);
		}
		m_len = len;
	}
	int padding = m_data.size() % 4;
	if(padding > 0)
	{
		padding = 4 - padding;
		for(int i = 0; i< padding; i++)
		{
			m_data.push_back(0);
		}
	}
}

VegaTag::~VegaTag()
{
	m_data.clear();
	m_data.shrink_to_fit();
}

const std::vector<unsigned char>& VegaTag::getData() const
{
	return m_data;
}

std::string VegaTag::getDataString() const
{
	std::string result;
	std::copy(m_data.begin(), m_data.end(), std::back_inserter(result));
	return result;
}

uint32_t VegaTag::getSize() const
{
	return m_data.size() + 8;
}

void VegaTag::parse(std::istream& inData)
{
	VegaBaseTag::parse(inData);
	if(m_len > 0)
	{
		int32_t len = m_len;
		if(m_len%4 != 0)
		{
			len = (m_len / 4) * 4 + 4;
		}
		m_data.resize(m_len);
		inData.read((char*)&m_data[0], len);
	}
}

void VegaTag::serialize(std::ostream& outData)
{
	VegaBaseTag::serialize(outData);
	outData.write((char*)&m_data[0], m_data.size());
}
