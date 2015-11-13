function openDatabase(fileName)
{
	if (this.sqlite_open == null)
	{
		throw new Error("No sqlite_open()");
	}
	var res = this.sqlite_open(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed sqlite_open()");
}

function closeDatabase(pDB)
{
	if (this.sqlite_close == null)
	{
		throw new Error("No sqlite_close()");
	}
	if (typeof pDB === 'pointer') 
	{
		return this.sqlite_close(pDB);
	}
	else
	{
		throw new Error("closeDatabase() - no pointer");
	}
}

function execDatabase(pDB, statement, func)
{
	if (this.sqlite_exec == null)
	{
		throw new Error("No sqlite_exec()");
	}	
	if (typeof pDB !== 'pointer' ||
		typeof statement !== 'string')
	{
		throw new Error("execDatabase() - invalid parameters: " +  
				typeof pDB + ", " + typeof statement + ", " +  typeof func);
	}
	
	var bRes = this.sqlite_exec(pDB, statement, false);
	if (bRes == false)
	{
		throw new Error("sqlite_exec() - error");
	}
}

function main(databaseName)
{
	var pointDB = openDatabase(databaseName);
	var strStatement = "SELECT id,url FROM moz_invalid_nametable WHERE id BETWEEN 271 AND 1600 LIMIT 10";
	execDatabase(pointDB, strStatement, null);
	closeDatabase(pointDB);
}