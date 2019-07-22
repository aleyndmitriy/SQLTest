#pragma once

#include"IEnvironment.h"
#include<Windows.h>
#include <sql.h>
#include <sqlext.h>

namespace DrvFtaeAlarm {

	class SQLServerEnvironment final : public IEnvironment {
	public:
		SQLServerEnvironment();
		~SQLServerEnvironment() override;
		SQLServerEnvironment(SQLServerEnvironment&& rhs) = delete;
		SQLServerEnvironment& operator=(SQLServerEnvironment&& rhs) = delete;
		SQLServerEnvironment(const SQLServerEnvironment& src) = default;
		SQLServerEnvironment& operator=(const SQLServerEnvironment& rhs) = default;
		void* GetInterface() override;
	private:
		SQLHENV sqlEnvironment;
	};
}
