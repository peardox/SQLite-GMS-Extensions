#!/bin/bash

echo "Make SQLite object file"
# gcc -fPIC -O2 -lpthread -m32 -c -Wno-deprecated-declarations sqlite3.c

echo "Build SQLiteLinuxExtension"
gcc -fPIC -O2 -lpthread -m32 -c SQLiteLinuxExtension.c

echo "Create SO Library"
gcc -shared -O2 -lpthread -m32 -o SQLiteLinuxExtension.so SQLiteLinuxExtension.o sqlite3.o