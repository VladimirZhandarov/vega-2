#include "vegaswap.h"

VegaSwap::VegaSwap()
{

}

VegaSwap::~VegaSwap()
{

}

uint32_t VegaSwap::readSwap(std::istream& inData)
{
	uint32_t data = 0;
	inData.readsome(reinterpret_cast<char*>(&data), sizeof(uint32_t));
	return data;
}

void VegaSwap::writeSwap(std::ostream& outData, uint32_t data)
{
	outData.write(reinterpret_cast<char*>(&data), sizeof(uint32_t));
}
