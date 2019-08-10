#include "ConditionRecordsDAO.h"

DrvFtaeAlarm::ConditionRecordsDAO::ConditionRecordsDAO(const std::shared_ptr<DatabaseEngine>& databaseEngine) :_databaseEngine(databaseEngine)
{

}

DrvFtaeAlarm::ConditionRecordsDAO::~ConditionRecordsDAO()
{
	_databaseEngine.reset();
}