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

function main(databaseName)
{
	try {
		openDatabase(databaseName);
	}	catch (e) {
		print(e.stack);
	}
}