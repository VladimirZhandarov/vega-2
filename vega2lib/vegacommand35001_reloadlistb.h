#ifndef VEGACOMMAND35001_H
#define VEGACOMMAND35001_H

#include "vegacommand.h"

#include "vegatag.h"

/**
 * Команда от клиента на перечтение листа B
 */
class VegaCommand35001_ReloadListB : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand35001_ReloadListB();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand35001_ReloadListB(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand35001_ReloadListB();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND35001_H
