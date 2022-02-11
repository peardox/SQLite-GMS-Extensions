
#Install mingw32 on cygwin

set -x

x86_64-w64-mingw32-gcc -O2 -lpthread -m64 -c -Wno-deprecated-declarations sqlite3.c

x86_64-w64-mingw32-gcc -O2 -lpthread -m64 -c -DBUILD_DLL SQLiteWinExtension.c

x86_64-w64-mingw32-gcc -O2 -lpthread -m64 -shared -o SQLiteWinExtension.dll SQLiteWinExtension.o sqlite3.o

strip --strip-all SQLiteWinExtension.dll