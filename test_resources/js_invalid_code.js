function openDatabase(fileName)
{
	var res = this.open_database(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed open_database()");
}

function main("cdcdc\n")
{
	openDatabase(null);

}