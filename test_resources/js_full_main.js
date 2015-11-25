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
	
	if (typeof func === 'function')
		bRes = this.sqlite_exec(pDB, statement, true);
	else
		bRes = this.sqlite_exec(pDB, statement, false);
	if (bRes == false)
	{
		throw new Error("execDatabase() - failed");
		return;
	}

	if (this.sqlite_result == null || 
		typeof func !== 'function')
			return; //no responses
	
	var nField = 0;
	var row = ['id', 'url'];
	while(true)
	{
		var rows = this.sqlite_result();
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
		print("openDatabase() -> ", typeof(pointDB));

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