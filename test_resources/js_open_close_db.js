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
		var pointDB = openDatabase(databaseName);
		closeDatabase(pointDB);
	}	catch (e) {
		print(e.stack);
	}
}