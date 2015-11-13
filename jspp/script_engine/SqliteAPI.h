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

	//opens database, uses direct access to sqlite API.
	//call from sqlite_open() or from C/C++ context. 
	Status openDatabase(const std::string&  strDatabasePath);
	//closes database, uses direct access to sqlite API.
	//call from sqlite_open() or from C/C++ context.
	void closeDatabase();
	//operations with database, uses direct access to sqlite API.
	//call from sqlite_exe() or from C/C++ context.
	//where:
	//1. strStatemenet - SQL command 
	//for example, SELECT id,url FROM moz_favicons WHERE id BETWEEN 271 AND 1600 LIMIT 10 
	//2. bUseCallback - get a result (bUseCallback = true) form database, 
	//for this case the function calls sql_callback();
	bool execDatabase(const std::string&  strStatement, bool bUseCallback = true);

private: 
	//export functions
	//their call is denied from C/C++ context, only from js

	//export function for js, uses openDatabase() for API access 
	static duk_ret_t sqlite_open(duk_context *ctx);
	//export function for js, uses closeDatabase() for API access
	static duk_ret_t sqlite_close(duk_context *ctx);
	//export function for js, uses execDatabase() for API access
	static duk_ret_t sqlite_exec(duk_context *ctx);
	//export function for js
	//reads results of  sqlite_exec()'s operations (if it needs), 
	//has access to m_deqstrSQLEntries
	static duk_ret_t sqlite_result(duk_context *ctx);

	//sql callback
	//use by execDatabase() if it neeeds to get info from database
	static int sql_callback(void *notUsed, int argc, char **argv, char **strColName);

	Status m_status;
	//use for access to SqliteAPI object  in sql_callback() and sqlite_open()
	static SqliteAPI* m_pSqliteAPI; 
	//database
	sqlite3* m_pDb;
	//array of sql responses, use in sql_callback() and sqlite_result()
	std::deque<std::string> m_deqstrSQLEntries;
	//keeps full database patch for json respnse
	std::string m_strDatabaseFileName; 
	//keeps table name for json respnse
	std::string m_strTableName; 
	//get a table name from sql command for json 
	bool getTableName(const std::string& strStatement, std::string& strTableName);

	//description of the last error
	std::string m_strError;
public:
	const std::string&  getStrError()
	{
		return m_strError;
	}
};

