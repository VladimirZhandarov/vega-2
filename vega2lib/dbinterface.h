#ifndef MONGODB_DBINTERFACE_H
#define MONGODB_DBINTERFACE_H

#include "vega2libdef.h"

#ifdef USE_MONGODB

#include <mongoc.h>

#include <time.h>

#include <string.h>
#include <memory>
#include <map>
#include <vector>
#include <queue>

#include <boost/thread.hpp>

class VegaCommandExecutor;
class VegaCommand40000_DataMessage;

struct OidTimeObject
{
	std::string oid;
	time_t time;

	OidTimeObject(const std::string& oidArg, time_t timeArg)
	{
		oid = oidArg;
		time = timeArg;
	}

	static bool sort(OidTimeObject object1, OidTimeObject object2)
	{
		return (object1.time < object2.time);
	}
};

class VEGA2LIB_EXPORT DbInterface
{
	static std::string vegaPlateCollectionName;		///< Имя коллекции MongoDB для хранения номеров
	static std::string mongoDbOidString;
	static std::string portNameString;
	static std::string timeString;
	static std::string plateString;
	static std::string countryString;
	static std::string directionTypeString;
	static std::string plateRectXString;
	static std::string plateRectYString;
	static std::string plateRectWidthString;
	static std::string plateRectHeightString;
	static std::string imageString;
	static std::string plateImageString;

public:
	/**
	* Конструктор
	*/
	DbInterface(VegaCommandExecutor* executor, int storeNoSendedPlate, int logLevel, const std::string& uriString, const std::string& database,
		const std::vector<std::string>& portNameVector);

	/**
	* Деструктор
	*/
	~DbInterface();

	bool init();

	/**
	* Проверить соединение с MongoDB
	* @return =TRUE - соединение установлено
	*/
	bool ping();

	int getVegaCommand40000Count(const std::string& portName);
	int getAllVegaCommand40000Count() const;

	/**
	* Добавить номер
	*/
	bool addVegaCommand40000(const std::string& portName, VegaCommand40000_DataMessage* command);

	VegaCommand40000_DataMessage* getNextVegaCommand40000(const std::string& portName);

	bool removeVegaCommand40000(const std::string& portName);

	bool freeDb(int addedNumber = 0);

	bool removeFirstVegaCommand40000();

private:
	bool open();

	void close();

	int fillVegaCommand40000Vector(const std::string& portName);

	std::string getOlderOidPortName();

	VegaCommandExecutor* m_executor;
	int m_storeNoSendedPlate;
	int m_logLevel;
	std::string m_uriString;
	std::string m_databaseName;
	std::vector<std::string> m_portNameVector;

	mongoc_client_t* m_client;
	mongoc_database_t* m_database;
	mongoc_collection_t* m_vegaPlateCollection;

	std::map<std::string, std::queue<OidTimeObject>* > m_vegaCommand40000QueueMap;
	int m_allVegaCommand40000Count;
	boost::mutex m_dbMutex;
};

#endif

#endif // MONGODB_DBINTERFACE_H
