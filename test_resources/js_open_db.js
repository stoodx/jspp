function openDatabase(fileName)
{
	var res = this.sqlite_open(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed sqlite_open()");
}

function main(databaseName)
{
	openDatabase(databaseName);

}