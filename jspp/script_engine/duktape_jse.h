#pragma once
#include <string>
#include <deque>

#include "duktape\duktape.h"


class SqliteAPI;

namespace stood
{
	typedef std::string typeSourceCode;
	
	class DuktapeJSE
	{
	public:
		enum class Status : int
		{
			OK					 = 0,
			JSFILE_NOT_EXISTS		,
			SYNCDATAFILEPATH_NOT_EXISTS,
			SNSS_FILE_NOT_VALID		,
			INVALID_JSON_PARAMS		,
			INVALID_SOURCE_CODE		,
			HEAP_CREATION_ERROR		,
			SCRIPT_PARAMS_NOT_FOUND	,
			NULL_PTR_RESULT,
			NO_MEMORY,
			SCRIPT_RUN_ERROR
		};

		DuktapeJSE(void);
		~DuktapeJSE(void);
		int eval(typeSourceCode& source_code);

		Status eval_from_file(const std::string& strFileName);

		const std::string& eval_with_params(const std::string& strFileName,
											const std::string& strFunction,
											const std::string& strJsonParams);
		
		Status analyzeSNSS(const std::string& strJsFileName,
						   const std::string& strSNSSFileName,
						   const std::string& strJsonParams,
						   std::string& strResult);

		bool FileExists(const std::string& strFilePath);
		bool isJsonValid(const std::string& strJson);

		///////////Access to SQLite database///////////////////////////
	public:
		Status analyzeSyncData(const std::string& strJsFilePath,
							   const std::string& strSynDataFilePath,
							   std::string& strResult);
		DuktapeJSE* getDuktapeJSE()
		{
			return m_pDuktapeJSE;
		}

		SqliteAPI* getSqliteAPIObject()
		{
			return m_pSQL;
		}

		int getSQLEntriesNumber()
		{
			return m_deqstrSQLEntries.empty() ? 0 : m_deqstrSQLEntries.size();
		}


	private:
		SqliteAPI* m_pSQL;
		std::string m_strResult;
		std::deque<std::string> m_deqstrSQLEntries; 
		static DuktapeJSE* m_pDuktapeJSE;
		//callbacks
		static duk_ret_t open_database_native(duk_context *ctx);
		static duk_ret_t close_database_native(duk_context *ctx);
		static duk_ret_t exec_database_native(duk_context *ctx);
		static int read_database_result_native(duk_context *ctx);
		static int sql_callback(void *notUsed, int argc, char **argv, char **strColName);
		///////////Access to SQLite database///////////////////////////

	};
}
