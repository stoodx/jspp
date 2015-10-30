#pragma once
#include <string>
#include <codecvt>

struct sqlite3;

class SqliteAPI
{
public:
	SqliteAPI(const char* strDatabasePath);
	~SqliteAPI(void);
	enum Status
	{
		ERROR_OPEN_DATABASE = -2,
		INVALID_PARAMETER,
		INIT,
		DATABASE_OPEN
	};
	Status m_status;
	std::string m_strError;


private:
	sqlite3 * m_pDb;

	inline std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf16<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	inline std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf16<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
};

