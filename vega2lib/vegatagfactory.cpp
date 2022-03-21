#include "vegatagfactory.h"

VegaTagFactory& VegaTagFactory::getInstance()
{
	static VegaTagFactory instance;
	return instance;
}
