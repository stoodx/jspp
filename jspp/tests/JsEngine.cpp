#ifdef _GOOGLE_TEST

#include <gtest\gtest.h>
#include "../script_engine/duktape_jse.h"
#include <fstream>

class ValidData
{
public:
	std::string m_strJsFile;
	std::string m_strJsFileWithInvalidSourceCode;
	std::string m_strSourceCode;
	std::string m_strSnssFile;
	std::string m_strSqlJsFile;
	std::string m_strJsonParams;
} validData;

class InValidData
{
public:
	std::string m_strJsFile;
	std::string m_strSourceCode;
	std::string m_strSnssFile;
	std::string m_strJsonParams;
} invalidData;

class JsEngine : public ::testing::Test
{
public:
	stood::DuktapeJSE js_engine;

	virtual void SetUp()
	{
		// Create valid data (files, source code, etc.)
		validData.m_strSourceCode = "function main(json_data) {\n"
												"return json_data;\n"
												"}\n";
		validData.m_strJsFile = "js_test_file.js";
		validData.m_strSqlJsFile = "..\\test_resources\\sql-debug.js";
		std::ofstream js_fs(validData.m_strJsFile);
		js_fs << validData.m_strSourceCode;
		js_fs.close();

		validData.m_strSnssFile = "snss_test_file";
		std::ofstream snss_fs(validData.m_strSnssFile);
		snss_fs << "Some data";
		snss_fs.close();

		validData.m_strJsonParams.assign( "{\"key1\":\"value1\", \"key2\":10}" );

		// Create invalid data
		invalidData.m_strSourceCode = "function main(json_data) {\n"
												"redturn json_data;\n"
												"}\n";
		invalidData.m_strJsonParams.assign( "{\"key1\":\"value1\", \"key2\":10}" );
		validData.m_strJsFileWithInvalidSourceCode = "js_test_file_with_invalid_source.js";
		std::ofstream js_ifs(validData.m_strJsFileWithInvalidSourceCode);
		js_ifs << invalidData.m_strSourceCode;
		js_ifs.close();
	}
};


TEST_F(JsEngine, BasicTest)
{
	ASSERT_EQ(1,1);
}
		
TEST_F(JsEngine, eval_code)
{
	stood::typeSourceCode strValidCode = "function main(){}";
	int res = js_engine.eval(strValidCode);
	ASSERT_EQ(0, res);
}

TEST_F(JsEngine,eval_invalid_code)
{
	stood::typeSourceCode strInValidCode = "invalidfunction main(){}";
	int res = js_engine.eval(strInValidCode);
	ASSERT_EQ(-1, res);
}

TEST_F(JsEngine,eval_from_file)
{
	stood::DuktapeJSE::Status nRes = js_engine.eval_from_file(validData.m_strJsFile);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::OK),
						static_cast<int>(nRes));
}
		
TEST_F(JsEngine, analyzeSNSS_withInvalidSourceCode)
{
	std::string strOutput;
	strOutput.reserve(1024);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS(validData.m_strJsFileWithInvalidSourceCode,
															validData.m_strSnssFile,
															validData.m_strJsonParams,
															strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::INVALID_SOURCE_CODE),
						static_cast<int>(nRes));
}

TEST_F(JsEngine, analyzeSNSS_withNoneExistingJsFile)
{
	std::string strOutput;
	strOutput.reserve(1024);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS(invalidData.m_strJsFile,
															validData.m_strSnssFile,
															validData.m_strJsonParams,
															strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::JSFILE_NOT_EXISTS),
						static_cast<int>(nRes));
}

TEST_F(JsEngine, analyzeSNSS_withNoneExistingSnssFile)
{
	std::string strOutput;
	strOutput.reserve(1024);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS(validData.m_strJsFile,
															invalidData.m_strSnssFile,
															validData.m_strJsonParams,
															strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::SNSS_FILE_NOT_VALID),
						static_cast<int>(nRes));
}
/*
TEST_F(JsEngine, analyzeSNSS_withInvalidJsonParam)
{
	std::string strOutput;
	strOutput.reserve(1024);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS(validData.m_strJsFile,
															validData.m_strSnssFile,
															invalidData.m_strJsonParams,
															strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::INVALID_JSON_PARAMS),
						static_cast<int>(nRes));
}*/
		
TEST_F(JsEngine, analyzeSNSS)
{
	std::string strOutput;
	strOutput.reserve(1024);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS(validData.m_strJsFile,
															validData.m_strSnssFile,
															validData.m_strJsonParams,
															strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::OK),
						static_cast<int>(nRes));
			
	ASSERT_STREQ("{\"key1\":\"value1\", \"key2\":10}", strOutput.c_str());
}

/*
TEST_F(JsEngine, analyzeSNSS_real)
{
	std::string strOutput;
	strOutput.reserve(102400);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS("..\\test_resources\\snss_analyser.js",
															"..\\test_resources\\Last Session",
															"",
															strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::OK),
						static_cast<int>(nRes));
			
	ASSERT_FALSE(strOutput.empty());
}
	
TEST_F(JsEngine, analyzeSyncData_real)
{
	std::string strOutput;
	strOutput.reserve(102400);
	stood::DuktapeJSE::Status nRes = js_engine.analyzeSNSS("..\\test_resources\\sql.js",
															   "..\\test_resources\\SyncData.sqlite3",
															   "",
															   strOutput);
	ASSERT_EQ(static_cast<int>(stood::DuktapeJSE::Status::OK),
						static_cast<int>(nRes));
			
	//ASSERT_FALSE(strOutput.empty());
}

TEST_F(JsEngine, isJsonValid)
{
	ASSERT_TRUE(js_engine.isJsonValid(validData.m_strJsonParams));
	ASSERT_TRUE(js_engine.isJsonValid(""));
	ASSERT_FALSE(js_engine.isJsonValid(invalidData.m_strJsonParams));
}
*/
#endif //_GOOGLE_TEST