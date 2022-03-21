#ifndef VEGACOMMANDFACTORY_H
#define VEGACOMMANDFACTORY_H

#include "dynamicfactory.h"

#include "vegacommand.h"

/**
 * Фабрика команд
 */
class VegaCommandFactory : public DynamicFactory<VegaCommand>
{
public:
	/**
	 * Получить экземпляр класса
	 */
	static VegaCommandFactory& getInstance();

	/**
	 * Добавить команду к фабрике
	 */
	template<class Base>
	void add()
	{
		Base base;
		VegaCommand* ptr = dynamic_cast<VegaCommand*>(&base);
		if(ptr)
		{
			DynamicFactory<VegaCommand>::add<Base>(ptr->getCommandCode());
		}
	}

	/**
	 * Создать команду
	 * @param tcpCommandServer
	 * @param executor объект реализующий бизнес-логику выполнения команды на сервере
	 * @param header заголовок команды
	 */
	VegaCommand* createVegaCommand(TcpCommandServer* tcpCommandServer, VegaCommandExecutor* executor, const VegaHeader& header)
	{
		VegaCommand* result = create(header.getCommandCode());
		if(result)
		{
			result->setTcpCommandServer(tcpCommandServer);
			result->setExecutor(executor);
			result->setHeader(header);
		}
		return result;
	}

private:
	/**
	 * Конструктор
	 */
	VegaCommandFactory()
	{

	}

	VegaCommandFactory(const VegaCommandFactory&);

	void operator=(const VegaCommandFactory&);
};

#endif // VEGACOMMANDFACTORY_H
