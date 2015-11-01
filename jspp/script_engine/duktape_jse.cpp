#include "duktape_jse.h"
#include <iostream>
#include <fstream>
#include "SnssFileAPI.h"
#include "../logger/ILogger.h"

#ifdef SDK
#include "../helpers/json/json.h"
#endif

#include "SqliteAPI.h"

namespace stood
{

	DuktapeJSE* DuktapeJSE::m_pDuktapeJSE = NULL;
	
	DuktapeJSE::DuktapeJSE(void)
	{
		m_pDuktapeJSE = this;
		m_pSQL = NULL;
	}

	DuktapeJSE::~DuktapeJSE(void)
	{
		m_pDuktapeJSE = NULL;
		if (m_pSQL)
			delete m_pSQL;
	}

	int DuktapeJSE::eval(std::string& source_code)
	{
		duk_context *ctx = duk_create_heap_default();
		if( duk_peval_string(ctx, source_code.c_str()) != 0)
		{
			duk_destroy_heap(ctx);
			return -1;
		}
		duk_destroy_heap(ctx);
		return 0;
	}

	DuktapeJSE::Status DuktapeJSE::eval_from_file(const std::string& strFileName)
	{
		if( !FileExists(strFileName) )
			return Status::JSFILE_NOT_EXISTS;

		duk_context* ctx = duk_create_heap_default();
		if( duk_peval_file(ctx, strFileName.c_str()) != 0)
		{
			std::cout << "Error: " << duk_safe_to_string(ctx, -1) << std::endl;
			duk_destroy_heap(ctx);
			return Status::INVALID_SOURCE_CODE;
		}
		duk_destroy_heap(ctx);
		return Status::OK;
	}


	DuktapeJSE::Status DuktapeJSE::analyzeSNSS(const std::string& strJsFileName,
											   const std::string& strSNSSFileName,
											   const std::string& strJsonParams,
											   std::string& strResult)
	{	
		if( !FileExists(strJsFileName) )
			return Status::JSFILE_NOT_EXISTS;
		
		if( !FileExists(strSNSSFileName) )
			return Status::SNSS_FILE_NOT_VALID;

		if( !isJsonValid(strJsonParams))
			return Status::INVALID_JSON_PARAMS;

		duk_context* ctx = duk_create_heap_default();
		if(!ctx)
			return Status::HEAP_CREATION_ERROR;
		
        duk_push_global_object(ctx);
		SnssFileAPI snssFileApi(ctx, strSNSSFileName);

		if( duk_peval_file(ctx, strJsFileName.c_str()) != 0 )
		{
			LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "DuktapeJSE: %s", duk_safe_to_string(ctx, -1));
			duk_destroy_heap(ctx);
			return Status::INVALID_SOURCE_CODE;
		}
        
		duk_get_prop_string(ctx, -2, "main");
		duk_push_string(ctx, strJsonParams.c_str());
        if (duk_pcall(ctx, 1) != 0)
		{
			LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "DuktapeJSE: %s", duk_safe_to_string(ctx, -1));
			duk_destroy_heap(ctx);
			return Status::SCRIPT_PARAMS_NOT_FOUND;
		}

		const char* chBuf = duk_get_string(ctx, -1);
		if( chBuf != NULL )
			strResult.assign(chBuf);
		else
		{
			LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "DuktapeJSE: %s", duk_safe_to_string(ctx, -1));
			duk_destroy_heap(ctx);
			return Status::NULL_PTR_RESULT;
		}
		duk_pop(ctx);
		duk_destroy_heap(ctx);

		return Status::OK;
	}
	
	bool DuktapeJSE::FileExists(const std::string& strFilePath)
	{
		std::ifstream fs(strFilePath);
		if( !fs.is_open() )
			return false;
		fs.seekg(0, std::ios::end);
		if( 0 == fs.tellg() )
		{
			fs.close();
			return false;
		}
		fs.close();
		return true;		
	}

	bool DuktapeJSE::isJsonValid(const std::string& strJson)
	{
		if( strJson.empty() )
			return true;
#ifdef SDK
		Json::Reader reader;
		Json::Value jsonObj;
		return reader.parse(strJson, jsonObj);
#else
		return true;
#endif
	}

	DuktapeJSE::Status DuktapeJSE::analyzeSyncData(const std::string& strJsFilePath,
												   const std::string& strSynDataFilePath,
												   std::string& strResult)
	{
		DuktapeJSE duk;
		Status status = Status::OK;
		strResult = "OK";
		duk.m_strResult = strResult;

		if( !duk.FileExists(strJsFilePath) )
		{
			strResult = "JSFILE_NOT_EXISTS";
			return Status::JSFILE_NOT_EXISTS;
		}
		if( !duk.FileExists(strJsFilePath) )
		{
			strResult = "SYNCDATAFILEPATH_NOT_EXISTS";
			return Status::JSFILE_NOT_EXISTS;
		}
		duk_context* ctx = duk_create_heap_default();
		if(!ctx)
		{
			strResult = "HEAP_CREATION_ERROR";
			return Status::HEAP_CREATION_ERROR;
		}
		duk_push_global_object(ctx);

		//init openDatabaseNative()
		duk_push_c_function(ctx, open_database_native, 1);
		duk_put_prop_string(ctx, -2,  "openDatabaseNative");

		//init closeDatabaseNative()
		duk_push_c_function(ctx, close_database_native, 1);
		duk_put_prop_string(ctx, -2,  "closeDatabaseNative");

		//init execDatabaseNative()
		duk_push_c_function(ctx, exec_database_native, 2);
		duk_put_prop_string(ctx, -2,  "execDatabaseNative");

		//init readDatabaseEntiesNative()
		duk_push_c_function(ctx, read_database_entries_native, 1);
		duk_put_prop_string(ctx, -2,  "readDatabaseEntiesNative");

		//run js
		if (duk_peval_file(ctx, strJsFilePath.c_str()) == 0)
		{
			duk_pop(ctx);
			duk_get_prop_string(ctx, -1, "main");
			duk_push_string(ctx, strSynDataFilePath.c_str());
 			if (duk_pcall(ctx, 1) != 0)
			{
				strResult = duk_safe_to_string(ctx, -1);
				status = Status::SCRIPT_RUN_ERROR;
			}
			duk_pop(ctx);
		}
		else
		{
			strResult = duk_safe_to_string(ctx, -1);
			status = Status::SCRIPT_PARAMS_NOT_FOUND;
		}

		duk_destroy_heap(ctx);
		strResult = duk.m_strResult;
		return status;
	}	

	duk_ret_t  DuktapeJSE::open_database_native(duk_context *ctx)
	{
		std::string strFileName = duk_require_string(ctx, 0);
		if (strFileName.empty() || !m_pDuktapeJSE)
		{
			duk_push_null(ctx);
			return 1;
		}
		if (m_pDuktapeJSE->m_pSQL)
		{
			delete m_pDuktapeJSE->m_pSQL;
			m_pDuktapeJSE->m_pSQL = NULL;
		}
		m_pDuktapeJSE->m_pSQL = new SqliteAPI(strFileName.c_str());
		if (!m_pDuktapeJSE->m_pSQL || 
			m_pDuktapeJSE->m_pSQL->m_status != m_pDuktapeJSE->m_pSQL->DATABASE_OPEN)
		{
			m_pDuktapeJSE->m_strResult = m_pDuktapeJSE->m_pSQL->m_strError;
			duk_push_null(ctx);
			return 1;
		}

		duk_push_pointer(ctx, (void*) m_pDuktapeJSE->m_pSQL);
		return 1;
	}

	duk_ret_t DuktapeJSE::close_database_native(duk_context *ctx)
	{
		if (!m_pDuktapeJSE)            
		{
			duk_push_boolean(ctx, 0);
			return 1;
		}
		SqliteAPI* pDB = (SqliteAPI*)duk_require_pointer(ctx, 0);
		if (!m_pDuktapeJSE->m_pSQL || m_pDuktapeJSE->m_pSQL != pDB)
		{
			m_pDuktapeJSE->m_strResult = "DB not init";
			duk_push_boolean(ctx, 0);
		}
		else
		{
			delete m_pDuktapeJSE->m_pSQL;
			m_pDuktapeJSE->m_pSQL = NULL;
			duk_push_boolean(ctx, 1);
		}
		return 1;
	}

	duk_ret_t DuktapeJSE::exec_database_native(duk_context *ctx)
	{
		if (!m_pDuktapeJSE)            
		{
			duk_push_boolean(ctx, 0);
			return 1;
		}
		SqliteAPI* pDB = (SqliteAPI*)duk_require_pointer(ctx, 0);
		std::string strStatement = duk_require_string(ctx, -1);
		if (!m_pDuktapeJSE->m_pSQL || 
			m_pDuktapeJSE->m_pSQL != pDB || 
			strStatement.empty())
		{
			m_pDuktapeJSE->m_strResult = "DB not init";
			duk_push_null(ctx);
		}
		m_pDuktapeJSE->m_deqstrSQLEntries.clear();
		if (!m_pDuktapeJSE->m_pSQL->sqlExec(strStatement.c_str(), sql_callback))
		{
			m_pDuktapeJSE->m_strResult = m_pDuktapeJSE->m_pSQL->m_strError;
			m_pDuktapeJSE->m_deqstrSQLEntries.clear();
			duk_push_boolean(ctx, 0);
			return 1;
		}
		if (m_pDuktapeJSE->m_deqstrSQLEntries.empty())
		{
			duk_push_boolean(ctx, 0);
		}
		else
		{
			duk_push_boolean(ctx, 1);
		}
		return 1;
	}

	int DuktapeJSE::sql_callback(void *notUsed, int argc, char **strRow, char **strColName)
	{
		if (!m_pDuktapeJSE)
			return 1;
		for(int i = 0; i < argc; i++)
		{
			m_pDuktapeJSE->m_deqstrSQLEntries.push_back(strRow[i] ? strRow[i] : NULL );
		}
		return 0;
	}

	int DuktapeJSE::read_database_entries_native(duk_context *ctx)
	{
		if (!m_pDuktapeJSE || m_pDuktapeJSE->m_deqstrSQLEntries.empty())            
		{
			duk_push_null(ctx);
			return 1;
		}
		duk_push_string(ctx, m_pDuktapeJSE->m_deqstrSQLEntries[0].c_str());
		m_pDuktapeJSE->m_deqstrSQLEntries.pop_front();
		return 1;
	}
}