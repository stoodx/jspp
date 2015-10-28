#include "duktape\duktape.h"
#include "duktape_jse.h"
#include <iostream>
#include <fstream>
#include "SnssFileAPI.h"
#include "../logger/ILogger.h"

#ifdef SDK
#include "../helpers/json/json.h"
#endif

namespace stood
{

	DuktapeJSE::DuktapeJSE(void)
	{
	}

	DuktapeJSE::~DuktapeJSE(void)
	{
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
		// To implement
		return Status::OK;
	}	

}