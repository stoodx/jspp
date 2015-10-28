#pragma once

#include <string>
#include <fstream>

class SnssFileAPI
{
public:
	SnssFileAPI(void* ctx,
				const std::string& strSnssFilePath = "",
				const std::string& strSyncDataFilePath = "");
	~SnssFileAPI(void);

	// API for using from javascript side
	static int readByte(void* ctx);
	static int readInt16(void* ctx);
	static int readInt32(void* ctx);
	static int readInt64(void* ctx);
	static int readDouble(void* ctx);

	static int readASCII(void* ctx);
	static int readUTF16(void* ctx);

	static int skipBytes(void* ctx);

	static int saveJson(void* ctx);
	static int loadJson(void* ctx);
	static int c_log(void* ctx);
	static int getSyncData(void* ctx);
	
private:
	static std::ifstream m_SnssFile;
	static std::string m_ParamFilePath;
};

