#include"Record.h"

std::pair<typename DrvFtaeAlarm::Record::const_iterator, bool> DrvFtaeAlarm::Record::insert(std::string colName, short dataType, std::string value)
{
	typename DrvFtaeAlarm::Record::FieldData val = std::make_pair(dataType, value);
	std::pair<std::string, typename DrvFtaeAlarm::Record::FieldData> rec = std::make_pair(colName, val);
	return recordData.insert(rec);
}

const typename DrvFtaeAlarm::Record::FieldData& DrvFtaeAlarm::Record::at(const std::string& key) const
{
	return recordData.at(key);
}

typename DrvFtaeAlarm::Record::const_iterator DrvFtaeAlarm::Record::cbegin() const
{
	return recordData.cbegin();
}

typename DrvFtaeAlarm::Record::const_iterator DrvFtaeAlarm::Record::cend() const
{
	return recordData.cend();
}

typename DrvFtaeAlarm::Record::const_iterator DrvFtaeAlarm::Record::findColumnValue(const std::string& key) const
{
	return recordData.find(key);
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