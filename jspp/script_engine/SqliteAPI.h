#pragma once
#include <string>

#include "BaseAPI.h"
#include "duktape\duktape.h"

struct sqlite3;

class SqliteAPI :  public BaseAPI
{
public:
	SqliteAPI(duk_context* ctx,  const std::string&  strDatabasePath);
	~SqliteAPI(void);
	enum Status
	{
		ERROR_OPEN_DATABASE = -2,
		INVALID_PARAMETER,
		NOT_INIT,
		DATABASE_OPEN
	};
	std::string m_strError;
	bool sqlExec(const std::string&  strStaemenet, int (*callback)(void*, int, char**, char**) = NULL);

	static duk_ret_t open_database_native(duk_context *ctx);
	static duk_ret_t close_database_native(duk_context *ctx);
	static duk_ret_t exec_database_native(duk_context *ctx);
	static int read_database_result_native(duk_context *ctx);
	static int sql_callback(void *notUsed, int argc, char **argv, char **strColName);

	Status m_status;
	static SqliteAPI* m_pSqliteAPI;
private:
	sqlite3 * m_pDb;
};

