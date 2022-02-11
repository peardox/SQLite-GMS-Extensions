#!/bin/bash
### Run on Linux with gnustep

### gcc -O2 -lpthread -c -Wno-deprecated-declarations sqlite3.c

#gcc mysqlite.m sqlite3.o `gnustep-config --objc-flags` `gnustep-config --objc-libs` -ldl -lobjc -lgnustep-base -o mysqlite

rm SQLiteMacExtension
gcc SQLiteMacExtension.m sqlite3.o `gnustep-config --objc-flags` `gnustep-config --objc-libs` -ldl -lobjc -lgnustep-base -o SQLiteMacExtension
if [ $? -eq 0 ]; then
./SQLiteMacExtension
fi