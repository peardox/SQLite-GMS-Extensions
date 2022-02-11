#!/bin/bash

rm win.db
### gcc -O2 -lpthread -c -Wno-deprecated-declarations sqlite3.c
#### gcc -O2 -lpthread -o hat hat.c sqlite3.o
gcc -O2 -pthread -o runme hat.c sqlite3.o -ldl
./runme
