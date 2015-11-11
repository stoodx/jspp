#pragma once
#include <string>
#include <deque>

#include "BaseAPI.h"
#include "duktape\duktape.h"

struct sqlite3;

class SqliteAPI :  public BaseAPI
{
public:
	SqliteAPI(duk_context* ctx);
	~SqliteAPI(void);
	enum Status
	{
		ERROR_OPEN_DATABASE = -2,
		INVALID_PARAMETER,
		INIT,
		DATABASE_OPEN
	};
	std::string m_strError;

	Status openDatabase(const std::string&  strDatabasePath);
	void closeDatabase();
	bool execDatabase(const std::string&  strStatemenet, bool bUseCallback = true);

private:
	//call only from js
	static duk_ret_t open_database(duk_context *ctx);
	//call only from js
	static duk_ret_t close_database(duk_context *ctx);
	//call only from js
	static duk_ret_t exec_database(duk_context *ctx);
	//call only from js
	static duk_ret_t read_database_result(duk_context *ctx);
	//sql callback
	static int readSqlResponse_callback(void *notUsed, int argc, char **argv, char **strColName);

	Status m_status;
	static SqliteAPI* m_pSqliteAPI; //use in js static functions 
	sqlite3* m_pDb;
	std::deque<std::string> m_deqstrSQLEntries;
	std::string m_strDatabaseFileName; //for json
	std::string m_strTableName; ////for json

	bool getTableName(const std::string& strStatement, std::string& strTableName);

};

