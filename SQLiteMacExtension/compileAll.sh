#!/bin/bash

echo "COMPILE sqlite3.c"
gcc -Wall -arch i386 -O2 -c -Wno-deprecated-declarations sqlite3.c

echo "COMPILE SQLiteMacExtension.m"
gcc -Wall -arch i386 -c SQLiteMacExtension.c

echo "Create SQLiteMacExtension.dylib Library"
#gcc -shared -O2 -lpthread -m32 -o SQLiteLinuxExtension.so SQLiteLinuxExtension.o sqlite3.o
gcc -Wall -arch i386 -dynamiclib SQLiteMacExtension.o sqlite3.o -o SQLiteMacExtension.dylib

echo "Verify library"
file SQLiteMacExtension.dylib

echo "otool -Tv on library"
otool -Tv SQLiteMacExtension.dylib

echo "otool -L on library"
otool -L SQLiteMacExtension.dylib

echo "nm -agon library"
nm -ag SQLiteMacExtension.dylib

#echo "ldd on library"
#ldd SQLiteMacExtension.dylib

echo "Done"