/*
function getError(fileName)
{
	print("getError: Not init");
    return null;
}

var readFileHelper = (this.readFileNative || getError);

function readFile(fileName)
{
    return readFileHelper(fileName);
}


Duktape.modSearch = function (id) 
{//require
    print('Loading module: ', id);
    // Return source code for module or throw an error.
	var res = readFile(id);
    if (typeof res === 'buffer') 
	{	
		print('require() - ok');
		return res.toString('ascii');
    }
    throw new Error('module not found: ' + id);	
};
*/
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
		print(databaseName);
		
		var pointDB = openDatabase(databaseName);
		print("openDatabase() ", typeof(pointDB));
		
/*		var filebuffer = readFile(databaseName);

		print("sql.Database")
		var db = new sql.Database(filebuffer);

		print("start SELECT")
		db.each("SELECT id,url FROM moz_favicons", function(row) {
			print(row.id, row.url);
		});

		print("db.close")
		db.close();
		*/
		print("Test finish");
	}	catch (e) {
		print(e.stack);
	}
}