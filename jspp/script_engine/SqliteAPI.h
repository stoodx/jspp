#pragma once
#include <string>

struct sqlite3;

class SqliteAPI
{
public:
	SqliteAPI(const char* strDatabasePath);
	~SqliteAPI(void);
	enum Status
	{
		ERROR_OPEN_DATABASE = -2,
		INVALID_PARAMETER,
		INIT,
		DATABASE_OPEN
	};
	Status m_status;
	std::string m_strError;


private:
	sqlite3 * m_pDb;
	bool convertAsciiToUnicode(const char * strAscii, wchar_t * strUnicode);
	bool convertUnicodeToAscii(const wchar_t * strUnicode, char * strAscii);
};
