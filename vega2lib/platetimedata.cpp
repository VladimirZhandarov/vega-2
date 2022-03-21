#include "platetimedata.h"

PlateTimeData::PlateTimeData(const std::string& plateNumber, const std::time_t& dateTime) : m_plateNumber(plateNumber), m_dateTime(dateTime)
{

}

PlateTimeData::~PlateTimeData()
{

}

const std::string& PlateTimeData::getPlateNumber() const
{
	return m_plateNumber;
}

const std::time_t& PlateTimeData::getDateTime() const
{
	return m_dateTime;
}
