#ifndef VEGACOMMAND35000_H
#define VEGACOMMAND35000_H

#include "vegacommand.h"

#include "vegatag.h"

/**
 * Команда от клиента на перечтение листа A
 */
class VegaCommand35000_ReloadListA : public VegaCommand
{
public:
	/**
	 * Конструктор
	 */
	VegaCommand35000_ReloadListA();

	/**
	 * @copydoc VegaCommand::VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header)
	 */
	VegaCommand35000_ReloadListA(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand35000_ReloadListA();

	/**
	 * @copydoc VegaCommand::execute()
	 */
	virtual VegaCommand* execute();
};

#endif // VEGACOMMAND35000_H
