#include"Record.h"

std::pair<typename DrvFtaeAlarm::Record::iterator, bool> DrvFtaeAlarm::Record::insert(std::string colName, short dataType, std::string value)
{
	typename DrvFtaeAlarm::Record::FieldData val = std::make_pair(dataType, value);
	std::pair<std::string, typename DrvFtaeAlarm::Record::FieldData> rec = std::make_pair(colName, val);
	return recordData.insert(rec);
}

typename DrvFtaeAlarm::Record::FieldData DrvFtaeAlarm::Record::operator[](std::string key)
{
	return recordData[key];
}

DrvFtaeAlarm::Record::~Record()
{
	recordData.clear();
}

bool DrvFtaeAlarm::operator==(const DrvFtaeAlarm::Record& lhs, const DrvFtaeAlarm::Record& rhs)
{
	return lhs.recordData == rhs.recordData;
}

bool DrvFtaeAlarm::operator!=(const DrvFtaeAlarm::Record& lhs, const DrvFtaeAlarm::Record& rhs)
{
	return lhs.recordData != rhs.recordData;
}

bool DrvFtaeAlarm::operator<(const DrvFtaeAlarm::Record& lhs, const DrvFtaeAlarm::Record& rhs)
{
	return lhs.recordData < rhs.recordData;
}

bool DrvFtaeAlarm::operator<=(const DrvFtaeAlarm::Record& lhs, const DrvFtaeAlarm::Record& rhs)
{
	return lhs.recordData <= rhs.recordData;
}

bool DrvFtaeAlarm::operator>(const DrvFtaeAlarm::Record& lhs, const DrvFtaeAlarm::Record& rhs)
{
	return lhs.recordData > rhs.recordData;
}

bool DrvFtaeAlarm::operator>=(const DrvFtaeAlarm::Record& lhs, const DrvFtaeAlarm::Record& rhs)
{
	return lhs.recordData >= rhs.recordData;
}