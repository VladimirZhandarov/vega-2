#ifndef VEGATAGFACTORY_H
#define VEGATAGFACTORY_H

#include "dynamicfactory.h"

#include <fstream>

#include "vegabasetag.h"

/**
 * Фабрика тегов
 */
class VegaTagFactory : public DynamicFactory<VegaBaseTag>
{
public:
	/**
	 * Получить экземпляр класса
	 */
	static VegaTagFactory& getInstance();

	/**
	 * Добавить тег к фабрике
	 */
	template<class Base>
	void add(uint32_t code)
	{
		Base base;
		VegaBaseTag* ptr = dynamic_cast<VegaBaseTag*>(&base);
		if(ptr)
		{
			DynamicFactory<VegaBaseTag>::add<Base>(code);
		}
	}

	/**
	 * Создать тег
	 * @param inData поток данных полученный от клиента по сети
	 */
	VegaBaseTag* createVegaTag(std::istream& inData)
	{
		inData.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		VegaBaseTag* result = 0;
		try
		{
			VegaSwap vegaSwap;
			uint32_t code = vegaSwap.readSwap(inData);
			result = create(code);
			if(result)
			{
				result->setCode(code);
				result->parse(inData);
			}
		}
		catch(const std::ifstream::failure& exc)
		{
			delete result;
			result = 0;
		}
		return result;
	}

private:
	/**
	 * Конструктор
	 */
	VegaTagFactory()
	{

	}

	VegaTagFactory(const VegaTagFactory&);

	void operator=(const VegaTagFactory&);
};

#endif // VEGATAGFACTORY_H
