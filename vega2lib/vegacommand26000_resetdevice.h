#ifndef VEGACOMMAND26000_H
#define VEGACOMMAND26000_H

#include "vegacommand.h"

/**
 * Команда от клиента на перечитывание файла настроек
 */
class VegaCommand26000_ResetDevice : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand26000_ResetDevice();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand26000_ResetDevice(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand26000_ResetDevice();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND26000_H
