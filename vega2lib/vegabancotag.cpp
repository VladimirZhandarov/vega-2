#include "vegabancotag.h"

uint32_t VegaBancoTag::IMAGE = 2000;

VegaBancoTag::VegaBancoTag() : VegaBaseTag(IMAGE)
{

}

VegaBancoTag::VegaBancoTag(uint32_t jpegQuality, const cv::Mat& image) : VegaBaseTag(IMAGE)
{
	m_len = m_data.headerSize;
	if(image.empty())
	{
		return;
	}
	std::vector<int> params;
	params.push_back(CV_IMWRITE_JPEG_QUALITY);
	int usedJpegQuality = jpegQuality;
	if(usedJpegQuality <= 0)
	{
		usedJpegQuality = 0;
	}
	if(usedJpegQuality > 100)
	{
		usedJpegQuality = 100;
	}
	params.push_back(usedJpegQuality);
	if(cv::imencode(".jpg", image, m_jpegBuffer, params))
	{
		m_jpegBufferLen = m_jpegBuffer.size();
		m_data.xResolution = image.cols;
		m_data.yResolution = image.rows;
		m_data.bitPerPixel = image.channels();
		m_data.usefulDataDimension = m_jpegBufferLen;
		int addZeroCount = m_jpegBufferLen%4;
		if(addZeroCount != 0)
		{
			addZeroCount = 4 - addZeroCount;
		}
		for(int i=0; i<addZeroCount; i++)
		{
			m_jpegBuffer.push_back(0);
		}
		m_len = m_data.headerSize + m_jpegBuffer.size();
	}
}

VegaBancoTag::~VegaBancoTag()
{
	m_jpegBuffer.clear();
	m_jpegBuffer.shrink_to_fit();
}

uint32_t VegaBancoTag::getSize() const
{
	return 8 + sizeof(Data) + m_jpegBuffer.size();
}

void VegaBancoTag::parse(std::istream& inData)
{
//TODO
	/*VegaBaseTag::parse(inData);
	if(m_len == sizeof(Data))
	{
		inData.read((char*)&m_data, sizeof(Data));
	}*/
}

void VegaBancoTag::serialize(std::ostream& outData)
{
	VegaBaseTag::serialize(outData);
	outData.write((char*)&m_data, sizeof(Data));
	outData.write((char*)&m_jpegBuffer[0], m_jpegBuffer.size());
}
