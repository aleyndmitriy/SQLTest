/*	Copyright (c) Ocean Data Systems
	All rights reserved

	USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED WITHOUT THE PRIOR
	EXPRESS WRITTEN PERMISSION OF OCEAN DATA SYSTEMS INC.
*/

/**
	@file Log.cpp
	@brief Log implementation
*/

#include <stdio.h>
#include <tchar.h>
#include "Log.h"
#include "ILog.h"
#include "OdsErr.h"
#include "OdsStringUtils.h"

namespace DrvFtaeAlarm
{

	static const TCHAR MODULE_NAME_PREFIX[] = _T("ODBC HDA ALARM| ");

	Log* Log::m_pInstance = 0;

	struct Log::LogBody
	{
		LogBody();

		ODS::System::ILog* m_pILog;
	};

	Log::LogBody::LogBody()
	{
		m_pILog = 0;
	}

	Log::Log()
	{
		m_pBody = new LogBody;
	}

	Log::~Log()
	{
		delete m_pBody;
	}

	Log* Log::GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new Log;

		return m_pInstance;
	}

	void Log::Init(ODS::System::ILog* pILog)
	{
		m_pBody->m_pILog = pILog;
	}

	void Log::WriteImpl(int nLevel, int nMsgType, const TCHAR* szFormat, va_list argList)
	{
		if (!m_pBody->m_pILog)
			return;

		ODS::OdsString msgPrefix = MODULE_NAME_PREFIX;

		ODS::OdsString msg;

		ODS::OdsStringUtils::FormatOdsStringFromArgList(szFormat, argList, &msg);

		msgPrefix += msg;

		m_pBody->m_pILog->Write(nLevel, nMsgType, _T("%s"), msgPrefix);
	}

	void Log::WriteInfo(const TCHAR* szFormat, ...)
	{
		if (!m_pBody->m_pILog)
			return;

		va_list argList;
		va_start(argList, szFormat);

		WriteImpl(ODS::System::ILog::Level::LOG_NORMAL, ODS::System::ILog::MsgType::INFO, szFormat, argList);

		va_end(argList);
	}

	void Log::WriteInfoDebug(const TCHAR* szFormat, ...)
	{
		if (!m_pBody->m_pILog)
			return;

		va_list argList;
		va_start(argList, szFormat);

		WriteImpl(ODS::System::ILog::Level::LOG_DEBUG, ODS::System::ILog::MsgType::INFO, szFormat, argList);

		va_end(argList);
	}

	void Log::WriteError(const TCHAR* szFormat, ...)
	{
		if (!m_pBody->m_pILog)
			return;

		va_list argList;
		va_start(argList, szFormat);

		WriteImpl(ODS::System::ILog::Level::LOG_NORMAL, ODS::System::ILog::MsgType::ERR, szFormat, argList);

		va_end(argList);
	}

	void Log::WriteErrorDebug(const TCHAR* szFormat, ...)
	{
		if (!m_pBody->m_pILog)
			return;

		va_list argList;
		va_start(argList, szFormat);

		WriteImpl(ODS::System::ILog::Level::LOG_DEBUG, ODS::System::ILog::MsgType::ERR, szFormat, argList);

		va_end(argList);
	}

} // namespace