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
int main(int argc, char* argv[])
{
	return 0;
}
#endif