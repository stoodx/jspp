// jspp.cpp : Defines the entry point for the console application.
//

#ifdef _GOOGLE_TEST

#include <gtest\gtest.h>

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	int nRes = RUN_ALL_TESTS();
	system("pause");
	return nRes;
}
#else

#include <tchar.h>
#include <iostream>
#include ".\script_engine\duktape_jse.h"

int main(int argc, char* argv[])
{
	std::cout << "**********************" << std::endl << "Start SQLite test" << std::endl;
	if (argc < 3 || !argv[1] || !argv[2] )
	{
		std::cout <<  "Error:" << std::endl << 
			"Need the pathes to files" << std::endl;
		std::cout <<  "Usage:" << std::endl << 
			"jssqlapp.exe <path_to_start_js_module> <path_to_sql_database>" << std::endl; 
	}
	else
	{
		std::string strResult;
		stood::DuktapeJSE::analyzeSyncData(argv[1], argv[2], strResult);
		std::cout << "**********************" << std::endl <<  "Finish SQLite test: " << strResult << std::endl;
	}
	system("pause");
	return 0;
}
#endif