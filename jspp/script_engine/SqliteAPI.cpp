#include <windows.h>
#include "..\sqlite3\sqlite3.h"

#include "SqliteAPI.h"


SqliteAPI::SqliteAPI(const char* strDatabasePath)
	: m_status(Status::INIT)
	, m_pDb(NULL)
	, m_strError("OK")
{
	if (strDatabasePath)
	{
		int nRes = 0;
		std::wstring strFilename;
		std::string str("12345");
		strFilename = s2ws(str); 
		nRes = sqlite3_open16(strFilename.c_str(), &m_pDb);
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
	}
}


SqliteAPI::~SqliteAPI(void)
{
	if (m_status == Status::DATABASE_OPEN)
		sqlite3_close(m_pDb);
}
