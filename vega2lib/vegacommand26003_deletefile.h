#ifndef VEGACOMMAND26003_H
#define VEGACOMMAND26003_H

#include "vegacommand.h"

/**
 * Команда от клиента на удаление файла
 */
class VegaCommand26003_DeleteFile : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand26003_DeleteFile();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand26003_DeleteFile(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand26003_DeleteFile();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND26003_H
