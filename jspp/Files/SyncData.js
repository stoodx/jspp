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

function execDatabase(pDB, statement, func)
{
	if (this.exec_database == null)
	{
		throw new Error("No exec_database_native()");
	}	
	if (typeof pDB !== 'pointer' ||
		typeof statement !== 'string')
	{
		throw new Error("execDatabase() - invalid parameters: " +  
				typeof pDB + ", " + typeof statement + ", " +  typeof func);
	}
	
	if (typeof func === 'function')
		bRes = this.exec_database(pDB, statement, true);
	else
		bRes = this.exec_database(pDB, statement, false);
	if (bRes == false)
	{
		throw new Error("execDatabase() - failed");
		return;
	}

	if (this.read_database_result == null || 
		typeof func !== 'function')
			return; //no responses
	
	var nField = 0;
	var row = ['id', 'url'];
	while(true)
	{
		var rows = this.read_database_result();
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
		print("execDatabase() -> " + strStatement);
		execDatabase(pointDB, strStatement, function(row) {
			print(row.id); 
			print(row.url);
		});

		var res = closeDatabase(pointDB);
		print("closeDatabase() -> ", res);
		
		print("JS finish");
	}	catch (e) {
		print(e.stack);
	}
}