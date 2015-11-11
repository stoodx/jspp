function openDatabase(fileName)
{
	if (this.open_database == null)
	{
		throw new Error("No openDatabaseNative()");
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
		throw new Error("No close_database()");
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

function execDatabase(pDB, statement)
{
	if (this.exec_database == null)
	{
		throw new Error("No exec_databasee()");
	}	
	if (typeof pDB !== 'pointer' ||
		typeof statement !== 'string')
	{
		throw new Error("execDatabase() - invalid parameters: " +  
				typeof pDB + ", " + typeof statement );
	}
	
	var bRes = this.exec_database(pDB, statement, true);
	if (bRes == false)
	{
		throw new Error("execDatabase() - no entries");
	}

	if (this.read_database_result == null)
			throw new Error("read_database_result() - no function");
	
	var nCount = 0;
	while(true)
	{
		var rows = this.read_database_result();
		if (rows == null || typeof rows === "undefined")
			break;
		nCount ++;
	}
	if (nCount != 2)
		throw new Error("read_database_result() - incorrected entries");
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