#include "vegacommand26000_resetdevice.h"

#include "vegacommandexecutor.h"

VegaCommand26000_ResetDevice::VegaCommand26000_ResetDevice() : VegaCommand(26000)
{

}

VegaCommand26000_ResetDevice::VegaCommand26000_ResetDevice(VegaCommandExecutor* executor, const VegaHeader& header) : VegaCommand(executor, header)
{

}

VegaCommand26000_ResetDevice::~VegaCommand26000_ResetDevice()
{

}

VegaCommand* VegaCommand26000_ResetDevice::execute()
{
	if(!checkImei())
	{
		return 0;
	}
	if(m_executor)
	{
		m_executor->resetDevice();
	}
	return 0;
}
