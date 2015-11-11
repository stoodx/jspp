function openDatabase(fileName)
{
	var res = this.open_database(fileName);
	if (typeof res === 'pointer') 
		return res;
	throw new Error("Failed open_database()");
}

function closeDatabase(pDB)
{
	if (typeof pDB === 'pointer') 
	{
		if (this.close_database(pDB) == true)
			return true;
		else
		    throw new Error("Failed close_database()");			
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