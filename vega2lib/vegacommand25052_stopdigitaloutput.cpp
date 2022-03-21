#include "vegacommand25052_stopdigitaloutput.h"

#include "core.h"
#include "vegadatatag.h"

#include "vegacommand25134_readdigitalinputoutput_response.h"

VegaCommand25052_StopDigitalOutput::VegaCommand25052_StopDigitalOutput() : VegaCommand(25052)
{

}

VegaCommand25052_StopDigitalOutput::VegaCommand25052_StopDigitalOutput(uint32_t number) : VegaCommand(25052)
{
	addTag(new VegaDataTag(VegaDataTag::DIGITAL_OUTPUT_SEL_NUMBER, number));
}

VegaCommand25052_StopDigitalOutput::VegaCommand25052_StopDigitalOutput(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand25052_StopDigitalOutput::~VegaCommand25052_StopDigitalOutput()
{

}

VegaCommand* VegaCommand25052_StopDigitalOutput::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	VegaDataTag* dataTag = getTag<VegaDataTag>(VegaDataTag::DIGITAL_OUTPUT_SEL_NUMBER);
	if(m_executor && dataTag)
	{
		DigitalInputOutputStatus status = m_executor->stopDigitalOutput(dataTag->getValue());
		return new VegaCommand25134_ReadDigitalInputOutput_Response(status.digitalInputNumber, status.digitalInputStatus, status.digitalOutputNumber,
			status.digitalOutputStatus);
	}
	return 0;
}
