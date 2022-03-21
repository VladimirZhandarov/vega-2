#include "vegacommand25134_readdigitalinputoutput.h"

#include "core.h"

#include "vegacommand25134_readdigitalinputoutput_response.h"

VegaCommand25134_ReadDigitalInputOutput::VegaCommand25134_ReadDigitalInputOutput() : VegaCommand(25134)
{

}

VegaCommand25134_ReadDigitalInputOutput::VegaCommand25134_ReadDigitalInputOutput(VegaCommandExecutor* executor, const VegaHeader& header) :
	VegaCommand(executor, header)
{

}

VegaCommand25134_ReadDigitalInputOutput::~VegaCommand25134_ReadDigitalInputOutput()
{

}

VegaCommand* VegaCommand25134_ReadDigitalInputOutput::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	if(m_executor)
	{
		DigitalInputOutputStatus status = m_executor->readDigitalInputOutput();
		return new VegaCommand25134_ReadDigitalInputOutput_Response(status.digitalInputNumber, status.digitalInputStatus, status.digitalOutputNumber,
			status.digitalOutputStatus);
	}
	return 0;
}
