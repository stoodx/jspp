function openDatabase(fileName)
{
	var res = this.sqlite_open(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed sqlite_open()");
}

function closeDatabase(pDB)
{
	if (typeof pDB === 'pointer') 
	{
		if (this.sqlite_close(pDB) == true)
			return true;
		else
		    throw new Error("Failed sqlite_close()");			
	}
	else
	{
		throw new Error("closeDatabase() - no pointer");
	}
}


function main(databaseName)
{
	try{
		var pointDB = openDatabase(databaseName);
		closeDatabase(pointDB);
	}	catch (e) {
		print(e.stack);
	}
}