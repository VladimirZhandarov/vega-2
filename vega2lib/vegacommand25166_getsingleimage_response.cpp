#include "vegacommand25166_getsingleimage_response.h"

#include "vegadatetimetag.h"
#include "vegabancotag.h"

VegaCommand25166_GetSingleImage_Response::VegaCommand25166_GetSingleImage_Response() : VegaCommand(25166)
{

}

VegaCommand25166_GetSingleImage_Response::VegaCommand25166_GetSingleImage_Response(std::time_t imageDateTime, uint32_t jpegQuality, const cv::Mat& image) :
	VegaCommand(25166)
{
	addTag(new VegaDateTimeTag(VegaDateTimeTag::IMAGE_TIME, imageDateTime));
	addTag(new VegaBancoTag(jpegQuality, image));
}

VegaCommand25166_GetSingleImage_Response::VegaCommand25166_GetSingleImage_Response(VegaCommandExecutor* executor, const VegaHeader& header) :
	VegaCommand(executor, header)
{

}

VegaCommand25166_GetSingleImage_Response::~VegaCommand25166_GetSingleImage_Response()
{

}
