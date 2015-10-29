#include <windows.h>
#include "..\sqlite3\sqlite3.h"

#include "SqliteAPI.h"


SqliteAPI::SqliteAPI(const char* strDatabasePath)
	: m_strDatabasePath(NULL)
	, m_status(Status::OK)
{
	if (strDatabasePath)
	{
		int nLen = strlen(strDatabasePath);
		m_strDatabasePath = new char[nLen + 1];
		if (m_strDatabasePath)
			strcpy_s(m_strDatabasePath, nLen + 1,  strDatabasePath);
		else
		{
			m_status = Status::NO_MEMORY;
		}
	}
}


SqliteAPI::~SqliteAPI(void)
{
	if (m_strDatabasePath)
		delete [] m_strDatabasePath;
}
