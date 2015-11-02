# jspp access to SQLite database via MSVC and Duktape


****Usage**** 

jspp.exe <path_to_start_js_module> <path_to_sql_database>

For example, start from Debug project:
 
jspp.exe ".\Files\SyncData.js" "D:\Work\jspp\trunk\jspp\Files\places.sqlite"

Result:

**********Start SQLite test************
D:\Work\jspp\trunk\jspp\Files\places.sqlite
openDatabase()  pointer
execDatabase() - SELECT id,url FROM moz_favicons WHERE id BETWEEN 271 AND 1600 LIMIT 10
271 http://www.abitant.com/favicon.ico

278 http://24tv.ua/favicon.ico

283 https://ninjablocks-discuss-uploads.s3.amazonaws.com/11886cb29bfeb3a257b828f
020cca322d008f8ebbc.png

284 https://edit.europe.yahoo.com/favicon.ico

285 http://wallpaperswide.com/favicon.ico

286 http://www.hdwallpapers.in/favicon.ico

287 http://wallpaperfx.com/favicon.png

288 http://hdw.eweb4.com/favicon.ico

289 http://nevseoboi.com.ua/favicon.ico

292 http://magic.wizards.com/sites/all/themes/wiz_mtg/favicon.ico

closeDatabase()  true
Test finish
**********Finish SQLite test************
OK
****
