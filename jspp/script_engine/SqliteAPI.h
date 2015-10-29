#pragma once

struct sqlite3;

class SqliteAPI
{
public:
	SqliteAPI(const char* strDatabasePath);
	~SqliteAPI(void);
	enum Status
	{
		OK = 0,
		NO_MEMORY
	};
	Status m_status;


private:
	char* m_strDatabasePath;
};

