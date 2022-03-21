#ifndef VEGACOMMAND25052_H
#define VEGACOMMAND25052_H

#include "vegacommand.h"

/**
 * Команда от клиента на выключение реле
 */
class VegaCommand25052_StopDigitalOutput : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand25052_StopDigitalOutput();

	VegaCommand25052_StopDigitalOutput(uint32_t number);

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand25052_StopDigitalOutput(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand25052_StopDigitalOutput();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND25052_H
