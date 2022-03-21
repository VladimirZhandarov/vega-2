#include "vegacommand35001_reloadlistb.h"

#include "core.h"

VegaCommand35001_ReloadListB::VegaCommand35001_ReloadListB() : VegaCommand(35001)
{

}

VegaCommand35001_ReloadListB::VegaCommand35001_ReloadListB(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand35001_ReloadListB::~VegaCommand35001_ReloadListB()
{

}

VegaCommand* VegaCommand35001_ReloadListB::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	if(m_executor)
	{
		m_executor->reloadBList();
	}
	return 0;
}
