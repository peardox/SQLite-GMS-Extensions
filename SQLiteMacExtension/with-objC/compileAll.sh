#!/bin/bash
### RUN on MacOS

echo "COMPILE SQLite.c"
#gcc -O2 -lpthread -c -Wno-deprecated-declarations sqlite3.c
###gcc -Wall -arch i386 -arch x86_64 -O2 -c -Wno-deprecated-declarations sqlite3.c
gcc -Wall -arch x86_64 -O2 -c -Wno-deprecated-declarations sqlite3.c

echo "COMPILE SQLiteMacExtension.m"
#gcc -framework Foundation -c SQLiteMacExtension.m sqlite3.o -ldl
###gcc -Wall -arch i386 -arch x86_64 -c SQLiteMacExtension.m
gcc -Wall -arch x86_64 -c SQLiteMacExtension.m

echo "Create SQLiteMacExtension.dylib"
#gcc -Wall -arch i386 -arch x86_64 -framework Foundation -dynamiclib -current_version 1.0 SQLiteMacExtension.o sqlite3.o -o SQLiteMacExtension.dylib
#g++ -Wall -arch i386 -arch x86_64 -framework Foundation -dynamiclib -undefined suppress -flat_namespace SQLiteMacExtension.o sqlite3.o -o SQLiteMacExtension.dylib
#gcc -Wall -arch i386 -arch x86_64 -framework Foundation -dynamiclib -dy SQLiteMacExtension.o sqlite3.o -o SQLiteMacExtension.dylib
#g++ -Wall -arch i386 -arch x86_64 -framework Foundation -dynamiclib -fPIC -o SQLiteMacExtension.dylib SQLiteMacExtension.o sqlite3.o
###gcc -Wall -arch i386 -arch x86_64 -std=c99 -lobjc -framework Foundation -dynamiclib -current_version 1.0 SQLiteMacExtension.o sqlite3.o -o SQLiteMacExtension.dylib
gcc -Wall -arch x86_64 -std=c99 -lobjc -framework Foundation -dynamiclib -current_version 1.0 SQLiteMacExtension.o sqlite3.o -o SQLiteMacExtension.dylib

###clang -dynamiclib -std=gnu99 -current_version 1.0 -compatibility_version 1.0 -fvisibility=hidden -g Ratings.c -o libRatings.A.dylib
echo "Verify library"
file SQLiteMacExtension.dylib

echo "otool -Tv on library"
otool -Tv SQLiteMacExtension.dylib

echo "otool -L on library"
otool -L SQLiteMacExtension.dylib

echo "nm -agon library"
nm -ag SQLiteMacExtension.dylib

echo "ldd on library"
ldd SQLiteMacExtension.dylib

echo "Done"