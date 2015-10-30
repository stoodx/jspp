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

function main(databaseName)
{
	try {
		print(databaseName);
		
		var pointDB = openDatabase(databaseName);
		print("openDatabase() ", typeof(pointDB));
		
/*	
		print("start SELECT")
		db.each("SELECT id,url FROM moz_favicons", function(row) {
			print(row.id, row.url);
		});
	*/

		var res = closeDatabase(pointDB);
		print("closeDatabase() ", res);
		
		print("Test finish");
	}	catch (e) {
		print(e.stack);
	}
}