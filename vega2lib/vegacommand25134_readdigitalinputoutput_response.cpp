#include "vegacommand25134_readdigitalinputoutput_response.h"

#include "vegadatatag.h"

VegaCommand25134_ReadDigitalInputOutput_Response::VegaCommand25134_ReadDigitalInputOutput_Response() : VegaCommand(25134)
{

}

VegaCommand25134_ReadDigitalInputOutput_Response::VegaCommand25134_ReadDigitalInputOutput_Response(
	uint32_t digitalInputNumber, uint32_t digitalInputStatus, uint32_t digitalOutputNumber,
	uint32_t digitalOutputStatus) : VegaCommand(25134)
{
	addTag(new VegaDataTag(VegaDataTag::DIGITAL_INPUT_NUMBER, digitalInputNumber));
	addTag(new VegaDataTag(VegaDataTag::DIGITAL_INPUT_STATUS, digitalInputStatus));
	addTag(new VegaDataTag(VegaDataTag::DIGITAL_OUTPUT_NUMBER, digitalOutputNumber));
	addTag(new VegaDataTag(VegaDataTag::DIGITAL_OUTPUT_STATUS, digitalOutputStatus));
}

VegaCommand25134_ReadDigitalInputOutput_Response::VegaCommand25134_ReadDigitalInputOutput_Response(
	VegaCommandExecutor* executor, const VegaHeader& header) :
	VegaCommand(executor, header)
{

}

VegaCommand25134_ReadDigitalInputOutput_Response::~VegaCommand25134_ReadDigitalInputOutput_Response()
{

}
