#include "vegacommand25051_startdigitaloutput.h"

#include "core.h"
#include "vegadatatag.h"

#include "vegacommand25134_readdigitalinputoutput_response.h"

VegaCommand25051_StartDigitalOutput::VegaCommand25051_StartDigitalOutput() : VegaCommand(25051)
{

}

VegaCommand25051_StartDigitalOutput::VegaCommand25051_StartDigitalOutput(uint32_t number) : VegaCommand(25051)
{
	addTag(new VegaDataTag(VegaDataTag::DIGITAL_OUTPUT_SEL_NUMBER, number));
}

VegaCommand25051_StartDigitalOutput::VegaCommand25051_StartDigitalOutput(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor,
	header)
{

}

VegaCommand25051_StartDigitalOutput::~VegaCommand25051_StartDigitalOutput()
{

}

VegaCommand* VegaCommand25051_StartDigitalOutput::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	VegaDataTag* dataTag = getTag<VegaDataTag>(VegaDataTag::DIGITAL_OUTPUT_SEL_NUMBER);
	if(m_executor && dataTag)
	{
		DigitalInputOutputStatus status = m_executor->startDigitalOutput(dataTag->getValue());
		return new VegaCommand25134_ReadDigitalInputOutput_Response(status.digitalInputNumber, status.digitalInputStatus, status.digitalOutputNumber,
			status.digitalOutputStatus);
	}
	return 0;
}
