function main_(databaseName)
{
	try {
		openDatabase(databaseName);
	}	catch (e) {
		print(e.stack);
	}
}