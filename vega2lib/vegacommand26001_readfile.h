#ifndef VEGACOMMAND26001_H
#define VEGACOMMAND26001_H

#include "vegacommand.h"

/**
 * Команда от клиента на чтение файла
 */
class VegaCommand26001_ReadFile : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand26001_ReadFile();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand26001_ReadFile(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand26001_ReadFile();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND26001_H
