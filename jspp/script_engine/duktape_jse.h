#pragma once
#include <string>

namespace stood
{
	typedef std::string typeSourceCode;
	
	class DuktapeJSE
	{
	public:
		enum class Status : int
		{
			OK						= 0,
			JSFILE_NOT_EXISTS		= 1,
			SNSS_FILE_NOT_VALID		= 2,
			INVALID_JSON_PARAMS		= 3,
			INVALID_SOURCE_CODE		= 4,
			HEAP_CREATION_ERROR		= 5,
			SCRIPT_PARAMS_NOT_FOUND	= 6,
			NULL_PTR_RESULT			= 7
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

		Status analyzeSyncData(const std::string& strJsFilePath,
							   const std::string& strSynDataFilePath,
							   std::string& strResult);

		bool FileExists(const std::string& strFilePath);
		bool isJsonValid(const std::string& strJson);
	};
}
