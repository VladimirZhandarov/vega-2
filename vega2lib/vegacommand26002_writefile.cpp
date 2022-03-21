#include "vegacommand26002_writefile.h"

#include "vegatag.h"
#include "vegacommandexecutor.h"

#include <fstream>

VegaCommand26002_WriteFile::VegaCommand26002_WriteFile() : VegaCommand(26002)
{

}

VegaCommand26002_WriteFile::VegaCommand26002_WriteFile(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand26002_WriteFile::~VegaCommand26002_WriteFile()
{

}

VegaCommand* VegaCommand26002_WriteFile::execute()
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
	VegaTag* fileDataTag = getTag<VegaTag>(VegaTag::FILE_DATA_TAG);
	if(fileNameTag && fileDataTag)
	{
		std::string fileName = homeDir + fileNameTag->getDataString();
		if(!fileName.empty())
		{
			std::ofstream outFile(fileName, std::ofstream::out);
			const std::vector<unsigned char>& data = fileDataTag->getData();
			outFile.write((const char*)&data[0], data.size());
			//outFile << fileDataTag->getDataString();
			if(m_executor)
			{
				m_executor->addLogMessage(std::string("WriteFile ") + fileName + std::string("   data size = ") + std::to_string(fileDataTag->getSize()));
				//m_executor->addLogMessage(std::string("WriteFile ") + fileName + std::string(" string size = ") + std::to_string(fileDataTag->getDataString().length()));
				//m_executor->addLogMessage(std::string("WriteFile ") + fileName + std::string(" data = ") + fileDataTag->getDataString());
			}
			outFile.flush();
			outFile.close();
		}
	}
	return 0;
}
