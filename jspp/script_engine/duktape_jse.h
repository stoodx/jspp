#pragma once
#include <string>

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
			NO_MEMORY
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
		static Status analyzeSyncData(const std::string& strJsFilePath,
							   const std::string& strSynDataFilePath,
							   std::string& strResult);
		static DuktapeJSE* m_pDuktapeJSE;

	private:
		SqliteAPI* m_pSQL;
		///////////Access to SQLite database///////////////////////////

	};
}
