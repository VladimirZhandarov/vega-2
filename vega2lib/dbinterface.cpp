#include "dbinterface.h"

#ifdef USE_MONGODB

#include "vegacommand40000_datamessage.h"
#include "vegatag.h"
#include "vegacommandexecutor.h"

#include <bson.h>

#define MAX_COUNT_TIMEOUT 600

std::string DbInterface::vegaPlateCollectionName = "vegaPlateCollection";
std::string DbInterface::mongoDbOidString = "_id";

std::string DbInterface::portNameString = "port_name";
std::string DbInterface::timeString = "time";
std::string DbInterface::plateString = "plate";
std::string DbInterface::countryString = "country";
std::string DbInterface::directionTypeString = "direction_type";
std::string DbInterface::plateRectXString = "rect_x";
std::string DbInterface::plateRectYString = "rect_y";
std::string DbInterface::plateRectWidthString = "rect_w";
std::string DbInterface::plateRectHeightString = "rect_h";
std::string DbInterface::imageString = "image";
std::string DbInterface::plateImageString = "plateImage";

DbInterface::DbInterface(VegaCommandExecutor* executor, int storeNoSendedPlate, int logLevel, const std::string& uriString, const std::string& databaseName,
	const std::vector<std::string>& portNameVector) : m_executor(executor), m_storeNoSendedPlate(storeNoSendedPlate), m_logLevel(logLevel),
	m_uriString(uriString), m_databaseName(databaseName), m_portNameVector(portNameVector), m_client(0), m_database(0), m_vegaPlateCollection(0),
	m_allVegaCommand40000Count(0)
{
	mongoc_init();
	for(int i = 0; i < m_portNameVector.size(); i++)
	{
		m_vegaCommand40000QueueMap[m_portNameVector[i]] = new std::queue<OidTimeObject>();
	}
}

DbInterface::~DbInterface()
{
	for(int i = 0; i < m_portNameVector.size(); i++)
	{
		delete m_vegaCommand40000QueueMap[m_portNameVector[i]];
		m_vegaCommand40000QueueMap[m_portNameVector[i]] = 0;
	}
	mongoc_cleanup();
}

bool DbInterface::init()
{
	bool result = true;
	m_allVegaCommand40000Count = 0;
	std::string message = std::string("MongoDB vega2app plate container count: ");
	for(int i = 0; i < m_portNameVector.size(); i++)
	{
		std::string portName = m_portNameVector[i];
		int64_t count = fillVegaCommand40000Vector(portName);
		if(count >= 0)
		{
			message += portName + std::string(" = ") + std::to_string(count) + std::string(" ; ");
			m_allVegaCommand40000Count += count;
		}
		else
		{
			result = false;
		}
	}
	if(m_executor)
	{
		m_executor->addLogMessage(message);
	}
	return result;
}

bool DbInterface::ping()
{
	bool result = true;
	m_dbMutex.lock();
	m_client = mongoc_client_new(m_uriString.c_str());
	if(m_client)
	{
		bson_t command;
		bson_init(&command);
		bson_append_int32(&command, "ping", -1, 1);
		bson_t reply;
		bson_error_t error;
		if(!mongoc_client_command_simple(m_client, "admin", &command, 0, &reply, &error))
		{
			result = false;
		}
		bson_destroy(&reply);
		bson_destroy(&command);
		mongoc_client_destroy(m_client);
		m_client = 0;
	}
	m_dbMutex.unlock();
	return result;
}

int DbInterface::getVegaCommand40000Count(const std::string& portName)
{
	std::queue<OidTimeObject>* oidTimeQueue = m_vegaCommand40000QueueMap[portName];
	if(oidTimeQueue)
	{
		return m_vegaCommand40000QueueMap[portName]->size();
	}
	return 0;
}

int DbInterface::getAllVegaCommand40000Count() const
{
	return m_allVegaCommand40000Count;
}

bool DbInterface::addVegaCommand40000(const std::string& portName, VegaCommand40000_DataMessage* command)
{
	if(!command)
	{
		return false;
	}
	bson_t bson;
	bson_init(&bson);

	bson_oid_t oid;
	bson_oid_init(&oid, 0);
	char oidStr[25];
	bson_oid_to_string(&oid, oidStr);
	std::string oidValue = std::string(oidStr);
	bson_append_oid(&bson, mongoDbOidString.c_str(), -1, &oid);

	time_t recognizeDateTime = command->getRecognizeDateTime();
	bson_append_utf8(&bson, portNameString.c_str(), -1, portName.c_str(), -1);
	bson_append_time_t(&bson, timeString.c_str(), -1, recognizeDateTime);
	bson_append_utf8(&bson, plateString.c_str(), -1, command->getPlate().c_str(), -1);
	bson_append_utf8(&bson, countryString.c_str(), -1, command->getCountry().c_str(), -1);
	bson_append_int32(&bson, directionTypeString.c_str(), -1, (int32_t)command->getDirectionType());

	CvRect plateRect = command->getPlateRect();
	bson_append_int32(&bson, plateRectXString.c_str(), -1, (int32_t)plateRect.x);
	bson_append_int32(&bson, plateRectYString.c_str(), -1, (int32_t)plateRect.y);
	bson_append_int32(&bson, plateRectWidthString.c_str(), -1, (int32_t)plateRect.width);
	bson_append_int32(&bson, plateRectHeightString.c_str(), -1, (int32_t)plateRect.height);

	VegaTag* imageVegaTag = command->getImageVegaTag();
	if(imageVegaTag)
	{
		const std::vector<unsigned char>& dataImage = imageVegaTag->getData();
		uint32_t dataImageLen = dataImage.size();
		bson_append_binary(&bson, imageString.c_str(), -1, BSON_SUBTYPE_BINARY, dataImage.data(), dataImageLen);
	}

	VegaTag* plateImageVegaTag = command->getPlateImageVegaTag();
	if(plateImageVegaTag)
	{
		const std::vector<unsigned char>& dataImage = plateImageVegaTag->getData();
		uint32_t dataImageLen = dataImage.size();
		bson_append_binary(&bson, plateImageString.c_str(), -1, BSON_SUBTYPE_BINARY, dataImage.data(), dataImageLen);
	}

	bool result = false;
	if(!freeDb(1))
	{
		if(m_logLevel == 1 && m_executor)
		{
			m_executor->addLogMessage(std::string("Cannot remove data from MongoDB"));
		}
	}
	else
	{
		m_dbMutex.lock();
		result = true;
		if(open())
		{
			bson_error_t error;
			if(mongoc_collection_insert(m_vegaPlateCollection, MONGOC_INSERT_NONE, &bson, 0, &error))
			{
				std::queue<OidTimeObject>* oidTimeQueue = m_vegaCommand40000QueueMap[portName];
				if(oidTimeQueue)
				{
					if(m_executor)
					{
						m_executor->addLogMessage(std::string("Generate OID: ") + oidValue);
					}
					oidTimeQueue->push(OidTimeObject(oidValue, recognizeDateTime));
					m_allVegaCommand40000Count++;
				}
			}
			else
			{
				result = false;
			}
		}
		close();
		m_dbMutex.unlock();
	}
	bson_destroy(&bson);
	return result;
}

VegaCommand40000_DataMessage* DbInterface::getNextVegaCommand40000(const std::string& portName)
{
	int64_t count = getVegaCommand40000Count(portName);
	if(count <= 0)
	{
		return 0;
	}
	VegaCommand40000_DataMessage* result = 0;
	std::queue<OidTimeObject>* oidTimeQueue = m_vegaCommand40000QueueMap[portName];
	if(!oidTimeQueue || oidTimeQueue->empty())
	{
		return 0;
	}
	std::string oidString = oidTimeQueue->front().oid;
	m_dbMutex.lock();
	if(open())
	{
		bson_t query;
		bson_init(&query);
		bson_oid_t oid;
		bson_oid_init_from_string(&oid, oidString.c_str());
		bson_append_oid(&query, mongoDbOidString.c_str(), -1, &oid);
		mongoc_cursor_t* cursor = mongoc_collection_find(m_vegaPlateCollection, MONGOC_QUERY_NONE, 0, 1, 0, &query, 0, 0);
		const bson_t* item;
		while(mongoc_cursor_next(cursor, &item))
		{
			std::string oidValue;
			bson_iter_t objectIdIter;
			if(bson_iter_init_find(&objectIdIter, item, mongoDbOidString.c_str()))
			{
				const bson_oid_t* oid = bson_iter_oid(&objectIdIter);
				if(oid)
				{
					char oidStr[25];
					bson_oid_to_string(oid, oidStr);
					oidValue = std::string(oidStr);
				}

				time_t timeValue;
				bson_iter_t timeIter;
				if(bson_iter_init_find(&timeIter, item, timeString.c_str()))
				{
					timeValue = bson_iter_time_t(&timeIter);
				}

				std::string plateValue = "";
				bson_iter_t plateIter;
				if(bson_iter_init_find(&plateIter, item, plateString.c_str()))
				{
					plateValue = bson_iter_utf8(&plateIter, 0);
				}

				std::string countryValue = "";
				bson_iter_t countryIter;
				if(bson_iter_init_find(&countryIter, item, countryString.c_str()))
				{
					countryValue = bson_iter_utf8(&countryIter, 0);
				}

				VegaCommand40000_DataMessage::DirectionType directionTypeValue = VegaCommand40000_DataMessage::NoDirectionType;
				bson_iter_t directionTypeIter;
				if(bson_iter_init_find(&directionTypeIter, item, directionTypeString.c_str()))
				{
					int directionTypeInt = bson_iter_int32(&directionTypeIter);
					if(directionTypeInt <= VegaCommand40000_DataMessage::UnknowDirectionType)
					{
						directionTypeValue = (VegaCommand40000_DataMessage::DirectionType)directionTypeInt;
					}
				}
				CvRect plateRect;
				bson_iter_t plateRectXIter;
				if(bson_iter_init_find(&plateRectXIter, item, plateRectXString.c_str()))
				{
					plateRect.x = bson_iter_int32(&plateRectXIter);
				}
				bson_iter_t plateRectYIter;
				if(bson_iter_init_find(&plateRectYIter, item, plateRectYString.c_str()))
				{
					plateRect.y = bson_iter_int32(&plateRectYIter);
				}
				bson_iter_t plateRectWidthIter;
				if(bson_iter_init_find(&plateRectWidthIter, item, plateRectWidthString.c_str()))
				{
					plateRect.width = bson_iter_int32(&plateRectWidthIter);
				}
				bson_iter_t plateRectHeightIter;
				if(bson_iter_init_find(&plateRectHeightIter, item, plateRectHeightString.c_str()))
				{
					plateRect.height = bson_iter_int32(&plateRectHeightIter);
				}

				std::vector<unsigned char> imageData;
				bson_iter_t imageIter;
				if(bson_iter_init_find(&imageIter, item, imageString.c_str()))
				{
					const unsigned char* buf;
					uint32_t len;
					bson_iter_binary(&imageIter, 0, &len, (const uint8_t**)&buf);
					if(len > 0)
					{
						imageData.insert(imageData.end(), buf, buf + len);
					}
				}

				std::vector<unsigned char> plateImageData;
				bson_iter_t plateImageIter;
				if(bson_iter_init_find(&plateImageIter, item, plateImageString.c_str()))
				{
					const unsigned char* buf;
					uint32_t len;
					bson_iter_binary(&plateImageIter, 0, &len, (const uint8_t**)&buf);
					if(len > 0)
					{
						plateImageData.insert(plateImageData.end(), buf, buf + len);
					}
				}

				std::time_t currentDateTime;
				time(&currentDateTime);
				result = new VegaCommand40000_DataMessage(currentDateTime, timeValue, plateValue, countryValue, directionTypeValue, imageData, plateRect,
					plateImageData);
			}
			break;
		}
		mongoc_cursor_destroy(cursor);
		bson_destroy(&query);
	}
	close();
	m_dbMutex.unlock();
	return result;
}

bool DbInterface::removeVegaCommand40000(const std::string& portName)
{
	bool result = false;
	std::queue<OidTimeObject>* oidTimeQueue = m_vegaCommand40000QueueMap[portName];
	if(!oidTimeQueue || oidTimeQueue->empty())
	{
		return false;
	}
	m_dbMutex.lock();
	if(open())
	{
		std::string oidString = oidTimeQueue->front().oid;
		if(!oidString.empty())
		{
			bson_oid_t oid;
			bson_oid_init_from_string(&oid, oidString.c_str());
			bson_t query;
			bson_error_t error;
			bson_init(&query);
			bson_append_oid(&query, mongoDbOidString.c_str(), -1, &oid);
			result = mongoc_collection_remove(m_vegaPlateCollection, MONGOC_REMOVE_NONE, &query, 0, &error);
			if(result)
			{
				oidTimeQueue->pop();
				m_allVegaCommand40000Count--;
			}
			bson_destroy(&query);
		}
	}
	close();
	m_dbMutex.unlock();
	if(m_allVegaCommand40000Count < 0)
	{
		init();
	}
	return result;
}

bool DbInterface::freeDb(int addedNumber)
{
	while(m_allVegaCommand40000Count + addedNumber > m_storeNoSendedPlate)
	{
		if(!removeFirstVegaCommand40000())
		{
			return false;
		}
	}
	return true;
}

bool DbInterface::removeFirstVegaCommand40000()
{
	if(m_allVegaCommand40000Count <= 0)
	{
		return false;
	}
	bool result = false;
	m_dbMutex.lock();
	std::string portName = getOlderOidPortName();
	std::queue<OidTimeObject>* oidTimeQueue = 0;
	if(!portName.empty())
	{
		oidTimeQueue = m_vegaCommand40000QueueMap[portName];
	}
	if(oidTimeQueue && !oidTimeQueue->empty())
	{
		if(open())
		{
			std::string oidString = oidTimeQueue->front().oid;
			if(!oidString.empty())
			{
				bson_oid_t oid;
				bson_oid_init_from_string(&oid, oidString.c_str());
				bson_t query;
				bson_error_t error;
				bson_init(&query);
				bson_append_oid(&query, mongoDbOidString.c_str(), -1, &oid);
				result = mongoc_collection_remove(m_vegaPlateCollection, MONGOC_REMOVE_NONE, &query, 0, &error);
				if(result)
				{
					if(m_logLevel == 1 && m_executor)
					{
						char buffer[64];
						std::tm* dateTimeTM = std::localtime(&oidTimeQueue->front().time);
						std::strftime(buffer, 64, "%Y-%m-%d-%H-%M-%S-000: ", dateTimeTM);
						m_executor->addLogMessage(std::string("MongoDB remove plate with time ") + std::string(buffer));
					}
					oidTimeQueue->pop();
					m_allVegaCommand40000Count--;
				}
				bson_destroy(&query);
			}
		}
	}
	close();
	m_dbMutex.unlock();
	if(m_allVegaCommand40000Count < 0)
	{
		init();
	}
	return result;
}

bool DbInterface::open()
{
	m_client = mongoc_client_new(m_uriString.c_str());
	if(m_client)
	{
		m_database = mongoc_client_get_database(m_client, m_databaseName.c_str());
		m_vegaPlateCollection = mongoc_client_get_collection(m_client, m_databaseName.c_str(), vegaPlateCollectionName.c_str());
		return m_database && m_vegaPlateCollection;
	}
	else
	{
		return false;
	}
}

void DbInterface::close()
{
	if(m_vegaPlateCollection)
	{
		mongoc_collection_destroy(m_vegaPlateCollection);
		m_vegaPlateCollection = 0;
	}
	if(m_database)
	{
		mongoc_database_destroy(m_database);
		m_database = 0;
	}
	if(m_client)
	{
		mongoc_client_destroy(m_client);
		m_client = 0;
	}
}

int DbInterface::fillVegaCommand40000Vector(const std::string& portName)
{
	std::queue<OidTimeObject>* result = m_vegaCommand40000QueueMap[portName];
	std::vector<OidTimeObject> oidTimeVector;
	bool opened = false;
	m_dbMutex.lock();
	if(open())
	{
		opened = true;
		bson_t query;
		bson_init(&query);
		bson_append_utf8(&query, portNameString.c_str(), -1, portName.c_str(), -1);
		bson_t fields;
		bson_init(&fields);
		bson_append_int32(&fields, mongoDbOidString.c_str(), -1, 1);
		bson_append_int32(&fields, timeString.c_str(), -1, 1);
		mongoc_cursor_t* cursor = mongoc_collection_find(m_vegaPlateCollection, MONGOC_QUERY_NONE, 0, 0, 0, &query, &fields, 0);
		const bson_t* item;
		while(mongoc_cursor_next(cursor, &item))
		{
			std::string oidValue;
			bson_iter_t objectIdIter;
			if(bson_iter_init_find(&objectIdIter, item, mongoDbOidString.c_str()))
			{
				const bson_oid_t* oid = bson_iter_oid(&objectIdIter);
				if(oid)
				{
					char oidStr[25];
					bson_oid_to_string(oid, oidStr);
					oidValue = std::string(oidStr);
				}

				time_t timeValue;
				bson_iter_t timeIter;
				if(bson_iter_init_find(&timeIter, item, timeString.c_str()))
				{
					timeValue = bson_iter_time_t(&timeIter);
				}
				oidTimeVector.push_back(OidTimeObject(oidValue, timeValue));
			}
		}
		mongoc_cursor_destroy(cursor);
		bson_destroy(&query);
	}
	close();
	std::sort(oidTimeVector.begin(), oidTimeVector.end(), OidTimeObject::sort);
	for(int i = 0; i < oidTimeVector.size(); i++)
	{
		result->push(oidTimeVector[i]);
	}
	m_dbMutex.unlock();
	if(opened)
	{
		return result->size();
	}
	else
	{
		return -1;
	}
}

std::string DbInterface::getOlderOidPortName()
{
	std::string result = "";
	time_t resultTime = 0;
	for(int i = 0; i < m_portNameVector.size(); i++)
	{
		std::string portName = m_portNameVector[i];
		std::queue<OidTimeObject>* oidTimeQueue = m_vegaCommand40000QueueMap[portName];
		if(oidTimeQueue && !oidTimeQueue->empty())
		{
			if(!resultTime)
			{
				result = portName;
				resultTime = oidTimeQueue->front().time;
			}
			else if(resultTime > oidTimeQueue->front().time)
			{
				result = portName;
				resultTime = oidTimeQueue->front().time;
			}
		}
	}
	return result;
}

#endif
