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

function execDatabase(pDB, statement)
{
	if (this.sqlite_exec == null)
	{
		throw new Error("No sqlite_exec()");
	}	
	if (typeof pDB !== 'pointer' ||
		typeof statement !== 'string')
	{
		throw new Error("execDatabase() - invalid parameters: " +  
				typeof pDB + ", " + typeof statement );
	}
	
	var bRes = this.sqlite_exec(pDB, statement, true);
	if (bRes == false)
	{
		throw new Error("execDatabase() - no entries");
	}

	if (this.sqlite_result == null)
			throw new Error("sqlite_result() - no function");
	
	var nCount = 0;
	while(true)
	{
		var rows = this.sqlite_result();
		if (rows == null || typeof rows === "undefined")
			break;
		nCount ++;
	}
	if (nCount != 2)
		throw new Error("sqlite_result() - incorrected entries");
}

function main(databaseName)
{
	try {
		var pointDB = openDatabase(databaseName);
		var strStatement = "SELECT id,url FROM moz_favicons WHERE id == 271";
		execDatabase(pointDB, strStatement, null);
		closeDatabase(pointDB);
	}	catch (e) {
		print(e.stack);
	}
}