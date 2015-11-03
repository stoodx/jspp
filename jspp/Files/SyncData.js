function openDatabase(fileName)
{
	if (this.openDatabaseNative == null)
	{
		throw new Error("No openDatabaseNative()");
	}
	var res = this.openDatabaseNative(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed openDatabaseNative()");
}

function closeDatabase(pDB)
{
	if (this.closeDatabaseNative == null)
	{
		throw new Error("No closeDatabaseNative()");
	}
	if (typeof pDB === 'pointer') 
	{
		return this.closeDatabaseNative(pDB);
	}
	else
	{
		throw new Error("closeDatabase() - no pointer");
	}
}

function execDatabase(pDB, statement, func)
{
	if (this.execDatabaseNative == null)
	{
		throw new Error("No execDatabaseNative()");
	}	
	if (typeof pDB !== 'pointer' ||
		typeof statement !== 'string' ||
		typeof func !== 'function')
	{
		throw new Error("execDatabase() - invalid parameters: " +  
				typeof pDB + ", " + typeof statement + ", " +  typeof func);
	}
	
	var bRes = this.execDatabaseNative(pDB, statement);
	if (bRes == false)
	{
		print("execDatabase() - no entries");
		return;
	}

	if (this.readDatabaseResultNative == null)
			return; //no response
	
	var nField = 0;
	var row = ['id', 'url'];
	while(true)
	{
		var rows = this.readDatabaseResultNative();
		if (rows == null || typeof rows === "undefined")
			break;
		if (nField == 0)
		{
			row.id = rows;
			nField = 1;
		}
		else
		{
			row.url = rows;
			nField = 0;
			func(row);
			print();
		}
	}
}

function main(databaseName)
{
	try {
		print(databaseName);
		
		var pointDB = openDatabase(databaseName);
		print("openDatabase() ", typeof(pointDB));

		var strStatement = "SELECT id,url FROM moz_favicons WHERE id BETWEEN 271 AND 1600 LIMIT 10";
		print("execDatabase() - " + strStatement);
		execDatabase(pointDB, strStatement, function(row) {
			print(row.id, row.url)
		});

		var res = closeDatabase(pointDB);
		print("closeDatabase() ", res);
		
		print("Test finish");
	}	catch (e) {
		print(e.stack);
	}
}