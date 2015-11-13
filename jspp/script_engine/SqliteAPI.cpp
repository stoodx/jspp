#include <windows.h>
#include <locale> 

#include "..\sqlite3\sqlite3.h"
#include "../logger/ILogger.h"
#include "SqliteAPI.h"

SqliteAPI* SqliteAPI::m_pSqliteAPI = NULL;

SqliteAPI::SqliteAPI(duk_context* ctx)
	: BaseAPI(ctx)	
	, m_pDb(NULL)
	, m_status(Status::INIT) 
{
	m_pSqliteAPI = this;
	PUSH_C_FUNCTION(sqlite_open, 1);
	PUSH_C_FUNCTION(sqlite_close, 1);
	PUSH_C_FUNCTION(sqlite_exec, 3);
	PUSH_C_FUNCTION(sqlite_result, 0);
}

SqliteAPI::Status SqliteAPI::openDatabase(const std::string&  strDatabasePath)
{
	if (!strDatabasePath.empty())
	{
		int nRes = 0;
		closeDatabase();
		m_strDatabaseFileName = strDatabasePath; //save for json
		nRes = sqlite3_open_v2(strDatabasePath.c_str(), &m_pDb, SQLITE_OPEN_READONLY, NULL);
		if (nRes != SQLITE_OK)
		{
			m_status = Status::ERROR_OPEN_DATABASE;
			m_strError =  sqlite3_errmsg(m_pDb);
			LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by openning %s.", m_strError.c_str());
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

	return m_status;
}

void SqliteAPI::closeDatabase()
{
	if (m_status == Status::DATABASE_OPEN)
	{
		sqlite3_close(m_pDb);
		m_pDb = NULL;
		m_status = Status::INIT;
	}
}

SqliteAPI::~SqliteAPI(void)
{
	closeDatabase();
	m_pSqliteAPI = NULL;
}

bool SqliteAPI::execDatabase(const std::string&  strStatement,  bool bUseCallback)
{
	if (m_status != Status::DATABASE_OPEN)
	{
		m_strError = "DATABASE_CLOSE";
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI::execDatabase() - error %s", m_strError);
		return false;
	}
	if (strStatement.empty())
	{
		m_strError = "INVALID_PARAMETER";
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI::execDatabase() - error %s", m_strError);
		return false;
	}

	getTableName(strStatement, m_strTableName); //for json

	int nRes = 0;
	char* strErr = NULL;
	if (bUseCallback)
		nRes = sqlite3_exec(m_pDb, strStatement.c_str(), sql_callback,  0, &strErr);
	else
		nRes = sqlite3_exec(m_pDb, strStatement.c_str(), NULL,  0, &strErr);

	if( nRes != SQLITE_OK )
	{
		m_strError = strErr;
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI::execDatabase() - error %s", m_strError);
		sqlite3_free(strErr);
		return false;
    }
	return true;
}

bool SqliteAPI::getTableName(const std::string& strStatement, std::string& strTableName)
{
	//false - name is not found 
	if (strStatement.empty())
		return false;
	std::locale loc;
	std::string strLowStatement("");
	for(auto elem : strStatement)
		strLowStatement +=  std::tolower(elem, loc);
	strTableName = "";
	int nPos  = strLowStatement.find(" from ");
	if (nPos == std::string::npos)
		return false;
	nPos += strlen(" from ");
	int nSize = strStatement.size();
	for (nPos; nPos < nSize; nPos++)
	{
		char c = strStatement.at(nPos);
		if (c == ' ')
			break;
		strTableName += c; 
	}
	return true;
}

duk_ret_t SqliteAPI::sqlite_open(duk_context *ctx)
{
	std::string strFileName = duk_require_string(ctx, 0);
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::sqlite_open() - open database %s", strFileName.c_str());

	if (!m_pSqliteAPI)
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by init");
		duk_push_null(ctx);
		return 1;
	}
	if (m_pSqliteAPI->openDatabase(strFileName.c_str()) != Status::DATABASE_OPEN)
	{
		duk_push_null(ctx);
		return 1;
	}

	duk_push_pointer(ctx, (void*) m_pSqliteAPI);
	return 1;
}

duk_ret_t SqliteAPI::sqlite_close(duk_context *ctx)
{
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::sqlite_close()");
	SqliteAPI* pSqliteAPI = (SqliteAPI*)duk_require_pointer(ctx, 0);
	if (!pSqliteAPI)
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by init");
		duk_push_boolean(ctx, 0);
	}
	else
	{
		pSqliteAPI->closeDatabase();
		duk_push_boolean(ctx, 1);
	}
	return 1;
}

duk_ret_t SqliteAPI::sqlite_exec(duk_context *ctx)
{
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::sqlite_exec()");
	SqliteAPI* pSqliteAPI = (SqliteAPI*)duk_require_pointer(ctx, 0);
	std::string strStatement = duk_require_string(ctx, 1);
	bool bUseCallback = duk_require_boolean(ctx, 2) == 0 ? false : true;
	if (!pSqliteAPI)
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by init");
		duk_push_boolean(ctx, 0);
		return 1;
	}
	pSqliteAPI->m_deqstrSQLEntries.clear();
	if (!pSqliteAPI->execDatabase(strStatement, bUseCallback))
	{
		pSqliteAPI->m_deqstrSQLEntries.clear();
		duk_push_boolean(ctx, 0);
	}
	else
	{
		duk_push_boolean(ctx, 1);
	}
	return 1;
}

duk_ret_t SqliteAPI::sqlite_result(duk_context *ctx)
{
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SqliteAPI::sqlite_result()");
	if (!m_pSqliteAPI)
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SqliteAPI - error by init");
		duk_push_null(ctx);;
		return 1;
	}
	if (m_pSqliteAPI->m_deqstrSQLEntries.empty())            
	{
		duk_push_null(ctx);
		return 1;
	}
	duk_push_string(ctx, m_pSqliteAPI->m_deqstrSQLEntries[0].c_str());
	m_pSqliteAPI->m_deqstrSQLEntries.pop_front();
	return 1;
}


int SqliteAPI::sql_callback(void *notUsed, int argc, char **strRow, char **strColName)
{
	if (!m_pSqliteAPI)
		return 1;
	for(int i = 0; i < argc; i++)
	{
		char strJson[2 * MAX_PATH] = {0};
		sprintf_s(strJson, 2 * MAX_PATH, "{\"Database\": \"%s\", \"Table\": \"%s\", \"Column\": \"%s\", \"Value\": \"%s\"}", 
			m_pSqliteAPI->m_strDatabaseFileName.c_str(),
			m_pSqliteAPI->m_strTableName.c_str(),
			strColName[i],
			strRow[i]);
		m_pSqliteAPI->m_deqstrSQLEntries.push_back(strJson);
	}
	return 0;
}
