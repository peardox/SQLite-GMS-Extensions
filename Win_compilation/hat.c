#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define export __declspec (dllexport)
#define MAX 5
#define STRING_SIZE 1024
//======================================================================================================
//Make SQLite object file:
//gcc -O2 -lpthread -m32 -c -Wno-deprecated-declarations sqlite3.c
//Windows-Laptop:   19secs
//Linux-odroid:   1m40secs (=5x slower)
//Build for smaller size: (=20% size reduction)
//gcc -Os -lpthread -m32 -c -Wno-deprecated-declarations sqlite3.c
//======================================================================================================
//RUN:
//gcc -O2 -lpthread -m32 -o SQLiteWinExtension.exe SQLiteWinExtension.c sqlite3.o
//======================================================================================================
//Build DLL:
//gcc -O2 -lpthread -m32 -c -DBUILD_DLL SQLiteWinExtension.c
//gcc -O2 -lpthread -m32 -shared -o SQLiteWinExtension.dll SQLiteWinExtension.o sqlite3.o
//Build for smaller size:
//gcc -Os -lpthread -m32 -c -DBUILD_DLL SQLiteWinExtension.c
//gcc -Os -lpthread -m32 -shared -o SQLiteWinExtension.dll SQLiteWinExtension.o sqlite3.o
//======================================================================================================
sqlite3 * db;
sqlite3_stmt * qry = NULL;
char * rslt_str = NULL;
//======================================================================================================
export char *SQLiteWinOpenDB(char * db_filename) {
	if ( rslt_str == NULL )
		rslt_str = (char *) malloc(1024);
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( db == NULL ){
		int rc = sqlite3_open( db_filename, &db );
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteWinOpenDB:ERROR:%d:%s:%s", rc, sqlite3_errmsg(db), db_filename);
			sqlite3_close(db);
			db == NULL;
		}
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
export char *SQLiteWinCloseDB() {
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_close_v2(db);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteWinCloseDB:ERROR:%d:%s", rc, sqlite3_errmsg(db));	
	}
	db = NULL;
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
export char * SQLiteWinExecuteSQL(char * sql){
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	char * err_msg;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteWinExecuteSQL:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
export char * SQLiteWinExecuteInsert(char * sql){
	char * err_msg = 0;
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if ( rc != SQLITE_OK ) {
        sprintf(rslt, "SQLiteWinExecuteInsert:ERROR:%d:%s", rc, err_msg);
        sqlite3_free(err_msg);
    } else {
		sprintf( rslt, "%d", sqlite3_last_insert_rowid(db) );
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
export char * SQLiteWinExecuteUpdate(char * sql){
	char * err_msg = 0;
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if ( rc != SQLITE_OK ) {
        sprintf(rslt, "SQLiteWinExecuteUpdate:ERROR:%d:%s", rc, err_msg);
        sqlite3_free(err_msg);
    } else {
		sprintf(rslt, "%d", sqlite3_changes(db));
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
export char * SQLiteWinExecuteDelete(char * sql){
	char * err_msg = 0;
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if ( rc != SQLITE_OK ) {
        sprintf(rslt, "SQLiteWinExecuteDelete:ERROR:%d:%s", rc, err_msg);
        sqlite3_free(err_msg);
    } else {
		sprintf(rslt, "%d", sqlite3_changes(db));
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
export char * SQLiteWinQueryOpen(char *sql) {
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( qry == NULL ){
		int rc = sqlite3_prepare_v2(db, sql, -1, &qry, NULL);
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteWinQueryOpen:ERROR:%d:%s", rc, sqlite3_errmsg(db));
			sqlite3_finalize(qry);
			qry = NULL;
		}
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
export char * SQLiteWinQueryClose() {
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "-1");
	if ( qry != NULL ){
		int rc = sqlite3_finalize( qry );
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteWinQueryClose:ERROR:%d:%s", rc, sqlite3_errmsg(db));
		}
	}
	qry = NULL;
	strcpy(rslt_str, rslt);
	free( rslt );
	return rslt_str;
}
export char * SQLiteWinQueryGetNext() {
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_step(qry);
	if ( rc != SQLITE_ROW && rc != SQLITE_DONE )
		sprintf(rslt, "SQLiteWinQueryGetNext:ERROR:%d:%s", rc, sqlite3_errmsg(db));
	if ( rc == SQLITE_DONE )
		strcpy(rslt, "-1");
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
export const char * SQLiteWinQueryGetData(char * col) {
	return sqlite3_column_text(qry, atoi(col));
}
//======================================================================================================
// FOR DLL
#if 1
int main() { return 0; }
#else
//======================================================================================================
// FOR EXE
void execQuery(){
	char * rslt = SQLiteWinQueryOpen("SELECT * FROM t1 ORDER BY b;");
	printf("Query open result = %s \n", rslt);
	if ( strcmp( rslt, "0" ) == 0 ) {
		int i = 0;
		rslt = SQLiteWinQueryGetNext();
		while ( strcmp( rslt, "0" ) == 0 ) {
			printf("%d: %s - %s \n", i, SQLiteWinQueryGetData("0"), SQLiteWinQueryGetData("1") );
			i++;
			rslt = SQLiteWinQueryGetNext();
		}
		printf("Query end = %s \n", rslt);
		rslt = SQLiteWinQueryClose();
		printf("Query close result = %s \n", rslt);
	}
}

int main() {
	char * rslt = SQLiteWinOpenDB("win.db");
	if ( strcmp( rslt, "0" ) != 0 ) {
		printf("Cannot open database. result is %s \n", rslt);
		return 10;
	}

	printf("Opened database = %s \n", rslt);
	rslt = SQLiteWinExecuteSQL("DROP TABLE IF EXISTS t1;");
	printf("Drop table = %s \n", rslt);
	rslt = SQLiteWinExecuteSQL("CREATE TABLE t1 (a INTEGER PRIMARY KEY, b INTEGER);");
	printf("Created table = %s \n", rslt);
	rslt = SQLiteWinExecuteInsert("INSERT INTO t1 VALUES (1, 123)");
	printf("Insert values = %s \n", rslt);
	rslt = SQLiteWinExecuteInsert("INSERT INTO t1 VALUES (2, 50)");
	printf("Insert values = %s \n", rslt);

	execQuery();
	
	rslt = SQLiteWinExecuteUpdate("UPDATE t1 SET b = 1150 WHERE a = 2;");
	printf("Updated rows = %s \n", rslt);
	
	execQuery();

	rslt = SQLiteWinExecuteDelete("DELETE FROM t1 WHERE a = 1;");
	printf("Deleted rows = %s \n", rslt);
	
	execQuery();

	//rslt = SQLiteWinQueryOpen("SELECT * FROM t1 ORDER BY b;");
	//printf("Query open result = %s \n", rslt);
	
	rslt = SQLiteWinCloseDB();
	printf("Close DB: %s \n", rslt);
	free( rslt );
	return 0;
}
//======================================================================================================
#endif
