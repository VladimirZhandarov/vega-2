#ifndef VEGACOMMAND25051_H
#define VEGACOMMAND25051_H

#include "vegacommand.h"

/**
 * Команда от клиента на включение реле
 */
class VegaCommand25051_StartDigitalOutput : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand25051_StartDigitalOutput();

	VegaCommand25051_StartDigitalOutput(uint32_t number);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand25051_StartDigitalOutput(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand25051_StartDigitalOutput();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND25051_H
