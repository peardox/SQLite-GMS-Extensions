
REM Install MinGW-get-setup
REM Install mingw-developer-toolkit, mingw32-base, mingw32-gcc-g++
REM         mingw32-gcc-objc, msys-base
REM Add C:\MinGW\bin folder to path
REM Run this BAT file from Windows command line

REM Make SQLite object file: It takes a while!!!
REM gcc -O2 -lpthread -m32 -c -Wno-deprecated-declarations sqlite3.c

REM Build DLL: Edit line // FOR DLL \n #if 1
gcc -O2 -lpthread -m32 -c -DBUILD_DLL SQLiteWinExtension.c
gcc -O2 -m32 -s -shared -o SQLiteWinExtension.dll SQLiteWinExtension.o sqlite3.o

REM Build EXE: Edit line // FOR DLL \n #if 0
REM gcc -O2 -m32 -o SQLiteWinExtension.exe SQLiteWinExtension.c sqlite3.o

REM Reduce size: runs but does not have any effect on windows
REM strip --strip-all SQLiteWinExtension.dll