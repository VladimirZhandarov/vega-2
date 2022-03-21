#include "vegacommandfactory.h"

VegaCommandFactory& VegaCommandFactory::getInstance()
{
	static VegaCommandFactory instance;
	return instance;
}
