function openDatabase(fileName)
{
	var res = this.sqlite_open(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed open_database()");
}

function main("cdcdc\n")
{
	openDatabase(null);

}