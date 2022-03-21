#ifndef VEGACOMMAND_H
#define VEGACOMMAND_H

#include "vegaheader.h"

#include <map>

class TcpCommandServer;
class VegaCommandExecutor;
class VegaBaseTag;

/**
 * Команда (передается по сети от сервера к клиенту и обратно)
 */
class VegaCommand : public VegaSwap
{
public:
	/**
	 * Конструктор
	 * @param commandCode код команды
	 */
	VegaCommand(uint32_t commandCode);

	/**
	 * Конструктор
	 * @param executor объект реализующий бизнес-логику выполнения команды на
	 *                 сервере
	 * @param header заголовок команды
	 */
	VegaCommand(VegaCommandExecutor* executor, const VegaHeader& header);

	/**
	 * Деструктор
	 */
	virtual ~VegaCommand();

	void setTcpCommandServer(TcpCommandServer* tcpCommandServer);

	/**
	 * Установить объект реализующий бизнес-логику выполнения команды на
	 * сервере
	 * @param executor объект реализующий бизнес-логику выполнения команды на
	 *                 сервере
	 */
	void setExecutor(VegaCommandExecutor* executor);

	/**
	 * Установить заголовок команды
	 * @param header заголовок команды
	 */
	void setHeader(const VegaHeader& header);

	/**
	 * Получить код команды
	 * @return код команды
	 */
	uint32_t getCommandCode() const;

	/**
	 * Добавить тег
	 * @param tag тег
	 */
	void addTag(VegaBaseTag* tag);

	template<class Base>
	Base* getTag(uint32_t code)
	{
		return dynamic_cast<Base*>(m_tagMap[code]);
	}

	/**
	 * Парсить данные полученные от клиента по сети для получения параметров
	 * команды
	 * @param inData поток данных полученный от клиента по сети
	 */
	virtual void parse(std::istream& inData);

	/**
	 * Сериализовать команду в пакет готовый для передачи клиенту
	 * @param outData поток для передачи клиенту
	 */
	virtual uint32_t serialize(std::ostream& outData);

	/**
	 * Выполнить команду
	 */
	virtual VegaCommand* execute();

	int getTagMapSize() const;

	bool checkImei();

protected:
	TcpCommandServer* m_tcpCommandServer;
	VegaCommandExecutor* m_executor;	///< Объект реализующий бизнес-логику
										///< выполнения команды на сервере

	VegaHeader m_header;				///< Заголовок команды
	std::map<uint32_t, VegaBaseTag*> m_tagMap;	///< Карта тагов команды
};

#endif // VEGACOMMAND_H
