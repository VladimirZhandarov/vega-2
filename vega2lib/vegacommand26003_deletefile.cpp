#include "vegacommand26003_deletefile.h"

#include "vegatag.h"
#include "vegacommandexecutor.h"

#include <cstdio>

VegaCommand26003_DeleteFile::VegaCommand26003_DeleteFile() : VegaCommand(26003)
{

}

VegaCommand26003_DeleteFile::VegaCommand26003_DeleteFile(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand26003_DeleteFile::~VegaCommand26003_DeleteFile()
{

}

VegaCommand* VegaCommand26003_DeleteFile::execute()
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
	VegaTag* fileNameTag = getTag<VegaTag>(VegaTag::FILE_NAME_TAG);
	if(fileNameTag)
	{
		std::string fileName = homeDir + fileNameTag->getDataString();
		if(!fileName.empty())
		{
			std::remove(fileName.c_str());
		}
	}
	return 0;
}
