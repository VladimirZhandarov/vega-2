#include "vegacommand26001_readfile_response.h"

#include "vegatag.h"

VegaCommand26001_ReadFile_Response::VegaCommand26001_ReadFile_Response() : VegaCommand(26001)
{

}

VegaCommand26001_ReadFile_Response::VegaCommand26001_ReadFile_Response(const std::vector<unsigned char>& data) : VegaCommand(26001)
{
	addTag(new VegaTag(VegaTag::FILE_DATA_TAG, data));
}

VegaCommand26001_ReadFile_Response::VegaCommand26001_ReadFile_Response(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand26001_ReadFile_Response::~VegaCommand26001_ReadFile_Response()
{

}
