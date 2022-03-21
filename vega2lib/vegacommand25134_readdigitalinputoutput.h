#ifndef VEGACOMMAND25134_H
#define VEGACOMMAND25134_H

#include "vegacommand.h"

class VegaCommand25134_ReadDigitalInputOutput : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand25134_ReadDigitalInputOutput();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand25134_ReadDigitalInputOutput(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand25134_ReadDigitalInputOutput();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND25134_H
