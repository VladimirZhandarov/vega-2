#ifndef VEGACOMMAND25134_RESPONSE_H
#define VEGACOMMAND25134_RESPONSE_H

#include "vegacommand.h"

class VegaCommand25134_ReadDigitalInputOutput_Response : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand25134_ReadDigitalInputOutput_Response();

	//TODO comment
	VegaCommand25134_ReadDigitalInputOutput_Response(uint32_t digitalInputNumber, uint32_t digitalInputStatus, uint32_t digitalOutputNumber,
		uint32_t digitalOutputStatus);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand25134_ReadDigitalInputOutput_Response(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand25134_ReadDigitalInputOutput_Response();
};

#endif // VEGACOMMAND25134_RESPONSE_H
