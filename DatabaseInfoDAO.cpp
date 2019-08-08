#include "DatabaseInfoDAO.h"

DrvFtaeAlarm::DatabaseInfoDAO::DatabaseInfoDAO(const std::shared_ptr<DatabaseEngine>& databaseEngine):_databaseEngine(databaseEngine)
{

}

DrvFtaeAlarm::DatabaseInfoDAO::~DatabaseInfoDAO()
{
	_databaseEngine.reset();
}