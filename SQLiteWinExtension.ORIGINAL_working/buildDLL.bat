
REM cd /cygdrive/e/Game_Development/GameMaker.Resources/DataBases/sqlite/SQLiteWinExtension.ORIGINAL_working/

REM Make SQLite object file:
REM gcc -O2 -lpthread -m32 -c -Wno-deprecated-declarations sqlite3.c

REM Build DLL:
gcc -O2 -lpthread -m32 -c -DBUILD_DLL SQLiteWinExtension.c
gcc -O2 -lpthread -m32 -shared -static-libgcc -o SQLiteWinExtension.dll SQLiteWinExtension.o sqlite3.o