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
		nRes = sqlite3_open_v2(strDatabasePath, &m_pDb, SQLITE_OPEN_READONLY, NULL);
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


bool SqliteAPI::convertAsciiToUnicode(const char * strAscii, wchar_t * strUnicode)
{
	int len, i;
	if((strUnicode == NULL) || (strAscii == NULL))
		return false;
	len = strlen(strAscii);
	for(i=0; i<len+1;i++)
		*strUnicode++ = static_cast<wchar_t>(*strAscii++);
	return true;
}

bool SqliteAPI::convertUnicodeToAscii(const wchar_t * strUnicode, char * strAscii)
{
	int len, i;
	if((strUnicode == NULL) || (strAscii == NULL))
		return false;
	len = wcslen(strUnicode);
	for(i=0;i<len+1;i++)
		*strAscii++ = static_cast<char>(*strUnicode++);
	return true;
}
