#include "vegacommand25166_getsingleimage.h"

#include "vegacommand25166_getsingleimage_response.h"
#include "core.h"
#include "vegadatatag.h"
#include "vegapointtag.h"

VegaCommand25166_GetSingleImage::VegaCommand25166_GetSingleImage() : VegaCommand(25166)
{

}

VegaCommand25166_GetSingleImage::VegaCommand25166_GetSingleImage(uint32_t jpegQuality, uint32_t imageWidth, uint32_t imageHeight) : VegaCommand(25166)
{
	addTag(new VegaDataTag(VegaDataTag::JPEG_QUALITY, jpegQuality));
	addTag(new VegaPointTag(imageWidth, imageHeight));
}


VegaCommand25166_GetSingleImage::VegaCommand25166_GetSingleImage(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand25166_GetSingleImage::~VegaCommand25166_GetSingleImage()
{

}

VegaCommand* VegaCommand25166_GetSingleImage::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	if(m_executor)
	{
		VegaCommand* result = 0;
		SingleImage singleImage = m_executor->getSingleImage();

		VegaDataTag* jpegQuality = getTag<VegaDataTag>(VegaDataTag::JPEG_QUALITY);

		uint32_t jpegQualityValue = 75;
		if(jpegQuality)
		{
			jpegQualityValue = jpegQuality->getValue();
		}
		VegaPointTag* imageSize = getTag<VegaPointTag>(VegaPointTag::POINT);
		cv::Mat image = singleImage.image;
		if(!image.empty())
		{
			if(imageSize && (imageSize->getXCoordinate() != 0 || imageSize->getYCoordinate() != 0))
			{
				int x = imageSize->getXCoordinate();
				if(x <= 0)
				{
					x = image.cols;
				}
				int y = imageSize->getYCoordinate();
				if(y <= 0)
				{
					y = image.rows;
				}
				cv::Mat resizedImage = cv::Mat(y, x, image.type());
				cv::resize(image, resizedImage, cv::Size(x, y), cv::INTER_LINEAR);
				result = new VegaCommand25166_GetSingleImage_Response(singleImage.imageDateTime, jpegQualityValue, resizedImage);
				if(!resizedImage.empty())
				{
					resizedImage.release();
				}
			}
			else
			{
				result = new VegaCommand25166_GetSingleImage_Response(singleImage.imageDateTime, jpegQualityValue, image);
			}
			image.release();
		}
		else
		{
			result = new VegaCommand25166_GetSingleImage_Response(singleImage.imageDateTime, jpegQualityValue, image);
		}
		return result;
	}
	return 0;
}
