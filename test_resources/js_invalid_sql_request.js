function openDatabase(fileName)
{
	if (this.open_database == null)
	{
		throw new Error("No open_database()");
	}
	var res = this.open_database(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed open_database()");
}

function closeDatabase(pDB)
{
	if (this.close_database == null)
	{
		throw new Error("No closeDatabaseNative()");
	}
	if (typeof pDB === 'pointer') 
	{
		return this.close_database(pDB);
	}
	else
	{
		throw new Error("closeDatabase() - no pointer");
	}
}

function execDatabase(pDB, statement, func)
{
	if (this.exec_database == null)
	{
		throw new Error("No exec_database()");
	}	
	if (typeof pDB !== 'pointer' ||
		typeof statement !== 'string')
	{
		throw new Error("execDatabase() - invalid parameters: " +  
				typeof pDB + ", " + typeof statement + ", " +  typeof func);
	}
	
	var bRes = this.exec_database(pDB, statement, false);
	if (bRes == false)
	{
		throw new Error("exec_database() - error");
	}
}

function main(databaseName)
{
	var pointDB = openDatabase(databaseName);
	var strStatement = "SELECT id,url FROM moz_invalid_nametable WHERE id BETWEEN 271 AND 1600 LIMIT 10";
	execDatabase(pointDB, strStatement, null);
	closeDatabase(pointDB);
}