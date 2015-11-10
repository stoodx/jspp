#pragma once
#include <string>

struct sqlite3;

//? Inherit from BaseAPI
class SqliteAPI
{
public:
	SqliteAPI(const std::string&  strDatabasePath);
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

	//? Description
	bool sqlExec(const std::string&  strStaemenet, int (*callback)(void*, int, char**, char**) = NULL);

	//? Export this function
	//static duk_ret_t sqlite_open(duk_context *ctx);
	//static duk_ret_t sqlite_close(duk_context *ctx);
	//static duk_ret_t sqlite_exec(duk_context *ctx);
	//static int sqlite_read(duk_context *ctx);
	//static int sql_callback(void *notUsed, int argc, char **argv, char **strColName);


private:
	sqlite3 * m_pDb;

	//? Why this class should know about it. He should not be responsible for this methods
	//? Move them to separate function or class.
	bool convertAsciiToUnicode(const char * strAscii, wchar_t * strUnicode);
	bool convertUnicodeToAscii(const wchar_t * strUnicode, char * strAscii);
};

