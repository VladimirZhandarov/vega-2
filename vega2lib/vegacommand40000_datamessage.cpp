#include "vegacommand40000_datamessage.h"

#include "vegatag.h"
#include "vegadatatag.h"

VegaCommand40000_DataMessage::VegaCommand40000_DataMessage() : VegaCommand(40000), m_plate(""), m_country(""), m_directionType(NoDirectionType),
	m_imageVegaTag(0), m_plateImageVegaTag(0), m_messageDateTime(0), m_recognizeDateTime(0), m_plateImageStatus(0), m_fromStore(false)
{

}

VegaCommand40000_DataMessage::VegaCommand40000_DataMessage(const std::time_t& messageDateTime, const std::string& plate, const std::string& country,
	DirectionType directionType, const cv::Mat& image, const CvRect& plateRect, const cv::Mat& plateImage) : VegaCommand(40000), m_plate(plate),
	m_country(country), m_directionType(directionType), m_imageVegaTag(0), m_plateImageVegaTag(0), m_plateRect(plateRect), m_plateImageStatus(0),
	m_fromStore(false)
{
	m_messageDateTime = messageDateTime;
	m_recognizeDateTime = m_messageDateTime;
	std::tm* dateTimeTM = std::localtime(&m_messageDateTime);
	char dateBuffer[13];
	char timeBuffer[17];
	std::strftime(dateBuffer, 13, "%Y-%m-%d", dateTimeTM);
	std::strftime(timeBuffer, 17, "%H-%M-%S-000", dateTimeTM);

	init(std::string(dateBuffer), std::string(timeBuffer), plate, country, directionType, plateRect);
	if(!image.empty())
	{
		std::vector<unsigned char> jpegBuffer;
		std::vector<int> params;
		params.push_back(CV_IMWRITE_JPEG_QUALITY);
		params.push_back(75);
		if(cv::imencode(".jpg", image, jpegBuffer, params))
		{
			int32_t len = jpegBuffer.size();
			int addZeroCount = len%4;
			if(addZeroCount != 0)
			{
				addZeroCount = 4 - addZeroCount;
			}
			for(int i = 0; i < addZeroCount; i++)
			{
				jpegBuffer.push_back(0);
			}
			m_imageVegaTag = new VegaTag(VegaTag::IMAGE_BW_TAG, jpegBuffer, len);
			addTag(m_imageVegaTag);
		}
	}
	if(!plateImage.empty())
	{
		std::vector<unsigned char> jpegBuffer;
		std::vector<int> params;
		params.push_back(CV_IMWRITE_JPEG_QUALITY);
		params.push_back(75);
		if(cv::imencode(".jpg", plateImage, jpegBuffer, params))
		{
			int32_t len = jpegBuffer.size();
			int addZeroCount = len%4;
			if(addZeroCount != 0)
			{
				addZeroCount = 4 - addZeroCount;
			}
			for(int i = 0; i < addZeroCount; i++)
			{
				jpegBuffer.push_back(0);
			}
			m_plateImageVegaTag = new VegaTag(VegaTag::IMAGE_COL_TAG, jpegBuffer, len);
			addTag(m_plateImageVegaTag);
			m_plateImageStatus = 1;
		}
		else
		{
			m_plateImageStatus = -1;
		}
	}
}

VegaCommand40000_DataMessage::VegaCommand40000_DataMessage(const std::time_t& messageDateTime, const std::time_t& recognizeDateTime, const std::string& plate,
	const std::string& country, DirectionType directionType, const std::vector<unsigned char>& imageData, const CvRect& plateRect,
	const std::vector<unsigned char>& plateImageData) : VegaCommand(40000), m_plate(plate), m_country(country), m_directionType(directionType),
	m_imageVegaTag(0), m_plateImageVegaTag(0), m_plateRect(plateRect), m_plateImageStatus(0), m_fromStore(true)
{
	m_messageDateTime = messageDateTime;
	m_recognizeDateTime = recognizeDateTime;
	std::tm* dateTimeTM = std::localtime(&m_recognizeDateTime);
	char dateBuffer[13];
	char timeBuffer[17];
	std::strftime(dateBuffer, 13, "%Y-%m-%d", dateTimeTM);
	std::strftime(timeBuffer, 17, "%H-%M-%S-000", dateTimeTM);
	init(std::string(dateBuffer), std::string(timeBuffer), plate, country, directionType, plateRect);
	if(!imageData.empty())
	{
		m_imageVegaTag = new VegaTag(VegaTag::IMAGE_BW_TAG, imageData, imageData.size());
		addTag(m_imageVegaTag);
	}
	if(!plateImageData.empty())
	{
		m_plateImageVegaTag = new VegaTag(VegaTag::IMAGE_COL_TAG, plateImageData, plateImageData.size());
		addTag(m_plateImageVegaTag);
	}
}

VegaCommand40000_DataMessage::VegaCommand40000_DataMessage(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand40000_DataMessage::~VegaCommand40000_DataMessage()
{

}

const std::string& VegaCommand40000_DataMessage::getPlate() const
{
	return m_plate;
}

const std::string& VegaCommand40000_DataMessage::getCountry() const
{
	return m_country;
}

VegaCommand40000_DataMessage::DirectionType VegaCommand40000_DataMessage::getDirectionType() const
{
	return m_directionType;
}

VegaTag* VegaCommand40000_DataMessage::getImageVegaTag() const
{
	return m_imageVegaTag;
}

VegaTag* VegaCommand40000_DataMessage::getPlateImageVegaTag() const
{
	return m_plateImageVegaTag;
}

const CvRect& VegaCommand40000_DataMessage::getPlateRect() const
{
	return m_plateRect;
}

const std::time_t& VegaCommand40000_DataMessage::getMessageDateTime() const
{
	return m_messageDateTime;
}

const std::time_t& VegaCommand40000_DataMessage::getRecognizeDateTime() const
{
	return m_recognizeDateTime;
}

int VegaCommand40000_DataMessage::getPlateImageStatus() const
{
	return m_plateImageStatus;
}

bool VegaCommand40000_DataMessage::isFromStore() const
{
	return m_fromStore;
}

void VegaCommand40000_DataMessage::init(const std::string& dateString, const std::string& timeString, const std::string& plate, const std::string& country,
	DirectionType directionType, const CvRect& plateRect)
{
	addTag(new VegaTag(VegaTag::DATE_TAG, dateString, 12));
	addTag(new VegaTag(VegaTag::TIME_TAG, timeString, 16));
	addTag(new VegaTag(VegaTag::PLATE_STRING_TAG, plate, 20));
	addTag(new VegaTag(VegaTag::PLATE_COUNTRY_TAG, country, 4));

	switch(directionType)
	{
		case GoAwayDirectionType:
		{
			addTag(new VegaTag(VegaTag::DIRECTION_TAG, VegaTag::DIRECTION_GOAWAY_VALUE, 12));
			break;
		}
		case AproachDirectionType:
		{
			addTag(new VegaTag(VegaTag::DIRECTION_TAG, VegaTag::DIRECTION_APROACH_VALUE, 12));
			break;
		}
		case UnknowDirectionType:
		{
			addTag(new VegaTag(VegaTag::DIRECTION_TAG, VegaTag::DIRECTION_UNKNOW_VALUE, 12));
			break;
		}
		default:
		case NoDirectionType:
		{

		}
	}
	if(plateRect.height > 0 && plateRect.width > 0)
	{
		addTag(new VegaDataTag(VegaDataTag::PLATE_MIN_X, plateRect.x));
		addTag(new VegaDataTag(VegaDataTag::PLATE_MIN_Y, plateRect.y));
		addTag(new VegaDataTag(VegaDataTag::PLATE_MAX_X, plateRect.x + plateRect.width));
		addTag(new VegaDataTag(VegaDataTag::PLATE_MAX_Y, plateRect.y + plateRect.height));
	}
}
