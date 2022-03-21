#ifndef VEGACOMMAND26002_H
#define VEGACOMMAND26002_H

#include "vegacommand.h"

/**
 * Команда от клиента на запись файла
 */
class VegaCommand26002_WriteFile : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand26002_WriteFile();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand26002_WriteFile(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand26002_WriteFile();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND26002_H
