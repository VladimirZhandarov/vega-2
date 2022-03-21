#include "vegacommand35000_reloadlista.h"

#include "vegacommandexecutor.h"

VegaCommand35000_ReloadListA::VegaCommand35000_ReloadListA() : VegaCommand(35000)
{

}

VegaCommand35000_ReloadListA::VegaCommand35000_ReloadListA(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand35000_ReloadListA::~VegaCommand35000_ReloadListA()
{

}

VegaCommand* VegaCommand35000_ReloadListA::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	if(m_executor)
	{
		m_executor->reloadAList();
	}
	return 0;
}
