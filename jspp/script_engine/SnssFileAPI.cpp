#include <string>
#include "SnssFileAPI.h"

#include <iostream>
#include <fstream>
#include "../logger/ILogger.h"

#ifdef SDK
#include "../ServicesManager.h"
#include "../ConfigurationService.h"
#endif


std::ifstream SnssFileAPI::m_SnssFile;
std::string SnssFileAPI::m_ParamFilePath = "";

SnssFileAPI::SnssFileAPI(duk_context* ctx,
						 const std::string& strSnssFilePath,
						 const std::string& strSyncDataFilePath)
						 : BaseAPI(ctx)
{	
	PUSH_C_FUNCTION(readByte, 0);
	PUSH_C_FUNCTION(readInt16, 0);
	PUSH_C_FUNCTION(readInt32, 0);
	PUSH_C_FUNCTION(readInt64, 0);
	PUSH_C_FUNCTION(readDouble, 0);

	PUSH_C_FUNCTION(readASCII, 1);
	PUSH_C_FUNCTION(readUTF16, 1);

	PUSH_C_FUNCTION(skipBytes, 1);
	
	PUSH_C_FUNCTION(saveJson, 1);
	PUSH_C_FUNCTION(loadJson, 0);
	
	PUSH_C_FUNCTION(c_log, 2);
	
	PUSH_C_FUNCTION(getSyncData, 0);

	m_SnssFile.open(strSnssFilePath, std::ios::in | std::ios::binary);
	if( !m_SnssFile.is_open() )
	{
		//TODO: Do proper action.
		//Log massage to file (or console for DEBUG mode )
		//printf("Error: file %s is not opened", strFilePath);
	}
	m_ParamFilePath = strSnssFilePath.substr(0, strSnssFilePath.find_last_of("\\/"));
	m_ParamFilePath.append("\\script_params.json");
}

SnssFileAPI::~SnssFileAPI(void)
{
	m_SnssFile.close();
}

int SnssFileAPI::readByte(duk_context* ctx)
{
	int value = 0;
	m_SnssFile.read(reinterpret_cast<char*>(&value), 1);
	if( m_SnssFile.eof() )
		return 0;
	
	duk_push_int(ctx, value);
	return 1;
}

int SnssFileAPI::readInt16(duk_context* ctx)
{
	int value = 0;
	m_SnssFile.read(reinterpret_cast<char*>(&value), 2);
	if( m_SnssFile.eof() )
		return 0;

	duk_push_int(ctx, value);
	return 1;
}

int SnssFileAPI::readInt32(duk_context* ctx)
{
	int value = 0;
	m_SnssFile.read(reinterpret_cast<char*>(&value), 4);
	if( m_SnssFile.eof() )
		return 0;

	duk_push_int(ctx, value);
	return 1;
}

int SnssFileAPI::readInt64(duk_context* ctx)
{
	long long value = 0;
	m_SnssFile.read(reinterpret_cast<char*>(&value), 8);
	if( m_SnssFile.eof() )
		return 0;

	duk_push_number(ctx, (duk_double_t)value);
	return 1;
}


int SnssFileAPI::readDouble(duk_context* ctx)
{
	duk_double_t value = 0;
	m_SnssFile.read(reinterpret_cast<char*>(&value), 8);
	if( m_SnssFile.eof() )
		return 0;

	duk_push_number(ctx, value);
	return 1;
}

int SnssFileAPI::readASCII(duk_context* ctx)
{
	int nRes = 1;
	int nLength = duk_get_int(ctx, -1);
	
	char* chBuf = new char[nLength];
	memset(chBuf, 0, nLength);
	m_SnssFile.read(chBuf, nLength);
	if( m_SnssFile.eof() )
		nRes = 0;
	else
		duk_push_lstring(ctx, chBuf, nLength);
	
	delete [] chBuf;
	return nRes;
}

int SnssFileAPI::readUTF16(duk_context* ctx)
{
	int nRes = 1;
	int nLength = duk_get_int(ctx, -1);
	nLength *= sizeof(wchar_t);

	char* chBuf = new char[nLength];
	m_SnssFile.read(chBuf, nLength);
	if( m_SnssFile.eof() )
		nRes = 0;
	else
		duk_push_lstring(ctx, chBuf, nLength);
	
	delete [] chBuf;
	return nRes;
}

int SnssFileAPI::skipBytes(duk_context* ctx)
{
	int nRes = 1;
	int nLength = duk_get_int(ctx, -1);
	char* chBuf = new char[nLength];
	m_SnssFile.read(chBuf, nLength);
	if( m_SnssFile.eof() )
		nRes = 0;
	delete [] chBuf;
	return 1;
}

int SnssFileAPI::saveJson(duk_context* ctx)
{	
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SnssFileAPI - save json to %s", m_ParamFilePath.c_str());
	std::ofstream fileStream(m_ParamFilePath, std::ofstream::out | std::ofstream::trunc);
	fileStream << duk_json_encode(ctx, -1);
	return 1;
}

int SnssFileAPI::loadJson(duk_context* ctx)
{
	std::string strJson = "{}";

	// Try to load settings from profile path
	// If file does not exist open the default one
	std::ifstream param_file(m_ParamFilePath);
	
	LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SnssFileAPI - load json from %s", m_ParamFilePath.c_str());
	if(param_file.is_open())
	{
		param_file >> strJson;
		param_file.close();
	}
	else
	{
		LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SnssFileAPI - %s does not exist yet.", m_ParamFilePath.c_str());

#ifdef SDK
		std::string strScriptParamsPath = "script_params.json";
		ServicesManager& svcManager = ServicesManager::instance();
		std::shared_ptr<ConfigurationService> configurationService =  svcManager.get<ConfigurationService>();
		if(configurationService.get() != NULL)
			strScriptParamsPath = configurationService->get(TXET("script_params_file_path"));
		LOG_MSG(ILogger::default(), LOG_LEVEL_DEBUG, "SnssFileAPI - load json from %s", strScriptParamsPath.c_str());

		std::ifstream file(strScriptParamsPath);
		if(file.is_open())
			file >> strJson;
		else
			LOG_MSG(ILogger::default(), LOG_LEVEL_WARN, "SnssFileAPI - error by openning %s.", strScriptParamsPath.c_str());
#endif
	}

	duk_push_string(ctx, strJson.c_str());
	duk_json_decode(ctx, -1);
	return 1;
}

int SnssFileAPI::c_log(duk_context* ctx)
{
	const char* chLogMsg = duk_get_string(ctx, -1);
	const char* chLogLevel = duk_get_string(ctx, -2);
	
	if(chLogMsg == NULL || chLogLevel == NULL)
		return 0;

	LOG_MSG(ILogger::default(), chLogLevel, "(analyser): %s", chLogMsg);
	return 1;
}

int SnssFileAPI::getSyncData(duk_context* ctx)
{	
	std::string strData = "";
	if(m_SnssFile.is_open())
	{
		m_SnssFile >> strData;
		m_SnssFile.close();
	}

	duk_push_string(ctx, strData.c_str());
	return 1;
}