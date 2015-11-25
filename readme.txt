# jspp access to SQLite database via MSVC and Duktape


****Usage**** 

jspp.exe <path_to_start_js_module> <path_to_sql_database>

For example, start from Debug project:
 
jspp.exe "D:\Work\jspp\branches\develop\test_resources\js_full_main.js" "D:\Work\jspp\branches\develop\test_resources\places.sqlite"

Result:

**********Start SQLite test************
D:\Work\jspp\trunk\jspp\Files\places.sqlite
openDatabase()  pointer
execDatabase() -> SELECT id,url FROM moz_favicons WHERE id BETWEEN 271 AND 1600
LIMIT 10
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "271"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://www.abitant.com/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "278"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://24tv.ua/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "283"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "https://ninjablocks-discuss-uploads.s3.amazonaws
.com/11886cb29bfeb3a257b828f020cca322d008f8ebbc.png"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "284"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "https://edit.europe.yahoo.com/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "285"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://wallpaperswide.com/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "286"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://www.hdwallpapers.in/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "287"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://wallpaperfx.com/favicon.png"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "288"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://hdw.eweb4.com/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "289"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://nevseoboi.com.ua/favicon.ico"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "id", "Value": "292"}
{"Database": "D:\Work\jspp\trunk\jspp\Files\places.sqlite", "Table": "moz_favico
ns", "Column": "url", "Value": "http://magic.wizards.com/sites/all/themes/wiz_mt
g/favicon.ico"}
closeDatabase() ->  true
JS finish
**********Finish SQLite test************
