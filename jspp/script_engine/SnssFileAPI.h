#pragma once

#include <string>
#include <fstream>

#include "duktape\duktape.h"

class SnssFileAPI
{
public:
	SnssFileAPI(duk_context* ctx,
				const std::string& strSnssFilePath = "",
				const std::string& strSyncDataFilePath = "");
	~SnssFileAPI(void);

	// API for using from javascript side
	static int readByte(duk_context* ctx);
	static int readInt16(duk_context* ctx);
	static int readInt32(duk_context* ctx);
	static int readInt64(duk_context* ctx);
	static int readDouble(duk_context* ctx);

	static int readASCII(duk_context* ctx);
	static int readUTF16(duk_context* ctx);

	static int skipBytes(duk_context* ctx);

	static int saveJson(duk_context* ctx);
	static int loadJson(duk_context* ctx);
	static int c_log(duk_context* ctx);
	static int getSyncData(duk_context* ctx);
	
private:
	static std::ifstream m_SnssFile;
	static std::string m_ParamFilePath;
};

