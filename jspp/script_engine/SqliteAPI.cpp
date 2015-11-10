#include <windows.h>
#include "..\sqlite3\sqlite3.h"

#include "../logger/ILogger.h"
#include "SqliteAPI.h"

SqliteAPI* SqliteAPI::m_pSqliteAPI = NULL;

SqliteAPI::SqliteAPI(duk_context* ctx,  const std::string&  strDatabasePath)
	: BaseAPI(ctx)	
	, m_pDb(NULL)
	, m_status(Status::NOT_INIT)
{
	PUSH_C_FUNCTION(open_database_native, 1);
	PUSH_C_FUNCTION(close_database_native, 1);
	PUSH_C_FUNCTION(exec_database_native, 2);
	PUSH_C_FUNCTION( read_database_result_native, 0);
	
	if (!strDatabasePath.empty())
	{
		int nRes = 0;
		nRes = sqlite3_open_v2(strDatabasePath.c_str(), &m_pDb, SQLITE_OPEN_READONLY, NULL);
		if (nRes != SQLITE_OK)
		{
			m_status = Status::ERROR_OPEN_DATABASE;
			m_strError =  sqlite3_errmsg(m_pDb);
		}
		else
		{
			m_status = Status::DATABASE_OPEN;
		}
	}
	else
	{
		m_status = Status::INVALID_PARAMETER;
		m_strError = "INVALID_PARAMETER";
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by openning %s.", m_strError.c_str());
	}
}


SqliteAPI::~SqliteAPI(void)
{
	if (m_status == Status::DATABASE_OPEN)
		sqlite3_close(m_pDb);
	m_pSqliteAPI = NULL;
}

bool SqliteAPI::sqlExec(const std::string&  strStaemenet, int (*callback)(void*, int, char**, char**))
{
	if (m_status != Status::DATABASE_OPEN)
	{
		m_strError = "DATABASE_CLOSE";
		return false;
	}
	if (strStaemenet.empty())
	{
		m_strError = "INVALID_PARAMETER";
		return false;
	}

	int nRes = 0;
	char* strErr = NULL;
	nRes = sqlite3_exec(m_pDb, strStaemenet.c_str(), callback,  0, &strErr);
	if( nRes != SQLITE_OK )
	{
		m_strError = strErr;
		sqlite3_free(strErr);
		return false;
    }
	return true;
}

duk_ret_t SqliteAPI::open_database_native(duk_context *ctx)
{
	std::string strFileName = duk_require_string(ctx, 0);
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::open_database_native() - open database %s", strFileName.c_str());

	if (strFileName.empty() || !m_pSqliteAPI)
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by init %s");
		duk_push_null(ctx);
		return 1;
	}
	if (m_pSqliteAPI->m_status != Status::DATABASE_OPEN)
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by openning %s.", m_pSqliteAPI->m_strError.c_str());
		duk_push_null(ctx);
		return 1;
	}

	duk_push_pointer(ctx, (void*) m_pSqliteAPI);
	return 1;
}

duk_ret_t SqliteAPI::close_database_native(duk_context *ctx)
{
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::close_database_native()");
	SqliteAPI* pSqliteAPI = (SqliteAPI*)duk_require_pointer(ctx, 0);
	if (!pSqliteAPI)
	{
		pSqliteAPI->m_strError = "DB not init";
		pSqliteAPI->m_status = Status::NOT_INIT;
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by closing %s.", m_pSqliteAPI->m_strError.c_str());
		duk_push_boolean(ctx, 0);
	}
	else
	{
		delete pSqliteAPI;
		duk_push_boolean(ctx, 1);
	}
	return 1;
}

duk_ret_t SqliteAPI::exec_database_native(duk_context *ctx)
{
	SqliteAPI* pSqliteAPI = (SqliteAPI*)duk_require_pointer(ctx, 0);
	std::string strStatement = duk_require_string(ctx, -1);
	if (strStatement.empty())
	{
		pSqliteAPI->m_strError = "DB not init";
		pSqliteAPI->m_status = Status::NOT_INIT;
		duk_push_null(ctx);
		return 1;
	}
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::exec_database_native() - %", strStatement.c_str());
	m_pDuktapeJSE->m_deqstrSQLEntries.clear();
	if (!m_pDuktapeJSE->m_pSQL->sqlExec(strStatement, sql_callback))
	{
		m_pDuktapeJSE->m_strResult = m_pDuktapeJSE->m_pSQL->m_strError;
		m_pDuktapeJSE->m_deqstrSQLEntries.clear();
		duk_push_boolean(ctx, 0);
		return 1;
	}
	if (m_pDuktapeJSE->m_deqstrSQLEntries.empty())
	{
		duk_push_boolean(ctx, 0);
	}
	else
	{
		duk_push_boolean(ctx, 1);
	}
	return 1;
}

int SqliteAPI::sql_callback(void *notUsed, int argc, char **strRow, char **strColName)
{
	if (!m_pDuktapeJSE)
		return 1;
	for(int i = 0; i < argc; i++)
	{
		m_pDuktapeJSE->m_deqstrSQLEntries.push_back(strRow[i] ? strRow[i] : NULL );
	}
	return 0;
}

int SqliteAPI::read_database_result_native(duk_context *ctx)
{
	if (!m_pDuktapeJSE || m_pDuktapeJSE->m_deqstrSQLEntries.empty())            
	{
		duk_push_null(ctx);
		return 1;
	}
	duk_push_string(ctx, m_pDuktapeJSE->m_deqstrSQLEntries[0].c_str());
	m_pDuktapeJSE->m_deqstrSQLEntries.pop_front();
	return 1;
}
