#include "vegacommand26001_readfile.h"

#include "vegatag.h"
#include "vegacommand26001_readfile_response.h"
#include "vegacommandexecutor.h"

#include <fstream>

VegaCommand26001_ReadFile::VegaCommand26001_ReadFile() : VegaCommand(26001)
{

}

VegaCommand26001_ReadFile::VegaCommand26001_ReadFile(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand26001_ReadFile::~VegaCommand26001_ReadFile()
{

}

VegaCommand* VegaCommand26001_ReadFile::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	std::string homeDir = "";
	if(m_executor)
	{
		homeDir = m_executor->getHomeDir();
	}
	if(!homeDir.empty() && homeDir[homeDir.size() - 1] != '/')
	{
		homeDir += "/";
	}
	VegaCommand* result = 0;
	VegaTag* fileNameTag = getTag<VegaTag>(VegaTag::FILE_NAME_TAG);
	if(fileNameTag)
	{
		std::string fileName = homeDir + fileNameTag->getDataString();
		if(!fileName.empty())
		{
			std::ifstream file(fileName, std::ios::in | std::ios::binary);
			file.unsetf(std::ios::skipws);
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			if(fileSize >= 0)
			{
				file.seekg(0, std::ios::beg);
				std::vector<unsigned char> dataVector;
				dataVector.reserve(fileSize);
				dataVector.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
				result = new VegaCommand26001_ReadFile_Response(dataVector);
			}
			file.close();
		}
	}
	return result;
}
