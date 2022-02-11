#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

sqlite3 *db;
sqlite3_stmt *qry = NULL;
char *rslt_str = NULL;
char *sql_count_rows = NULL;
//======================================================================================================
char *SQLiteLinuxOpenDB(char * db_filename) {
	if ( rslt_str == NULL )
		rslt_str = (char *) malloc(1024);
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( db == NULL ){
		int rc = sqlite3_open( db_filename, &db );
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteLinuxOpenDB:ERROR:%d:%s:%s", rc, sqlite3_errmsg(db), db_filename);
			sqlite3_close(db);
			db = NULL;
		}
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteLinuxGetLibVersion(){
	char * rslt = (char *) malloc(1024);
	sprintf(rslt, "SQLiteLinuxGetLibVersion:%s:%s:%d", sqlite3_libversion(), sqlite3_sourceid(), sqlite3_libversion_number());
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteLinuxCloseDB() {
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_close_v2(db);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteLinuxCloseDB:ERROR:%d:%s", rc, sqlite3_errmsg(db));	
	}
	db = NULL;
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
char *SQLiteLinuxExecuteSQL(char *sql){
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	char *err_msg;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteLinuxExecuteSQL:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
char *SQLiteLinuxExecuteInsert(char *sql){
	char *err_msg = 0;
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteLinuxExecuteInsert:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	} else {
		sprintf( rslt, "%lld", sqlite3_last_insert_rowid(db) );
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteLinuxExecuteUpdate(char * sql){
	char *err_msg = 0;
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteLinuxExecuteUpdate:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	} else {
		sprintf(rslt, "%d", sqlite3_changes(db));
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteLinuxExecuteDelete(char * sql){
	char *err_msg = 0;
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteLinuxExecuteDelete:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	} else {
		sprintf(rslt, "%d", sqlite3_changes(db));
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
void replaceCharInString(char *s, char old, char new){
	while( *s ) {
		if(*s == old)
		*s = new;
		++s;
	}
}
//======================================================================================================
char *SQLiteLinuxQueryOpen(char *sql) {
	if ( sql_count_rows == NULL )
		sql_count_rows = (char *) malloc(4096);
	sprintf(sql_count_rows, "SELECT COUNT(*) AS row_count FROM ( %s )", sql);
	replaceCharInString(sql_count_rows, ';', ' ');
	printf("Count Query = %s \n", sql_count_rows);
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( qry == NULL ){
		int rc = sqlite3_prepare_v2(db, sql, -1, &qry, NULL);
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteLinuxQueryOpen:ERROR:%d:%s", rc, sqlite3_errmsg(db));
			sqlite3_finalize(qry);
			qry = NULL;
		}
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteLinuxQueryGetRowCount() {
	sqlite3_stmt *count_qry = NULL;
	strcpy(rslt_str, "-1");
	int rc = sqlite3_prepare_v2(db, sql_count_rows, -1, &count_qry, NULL);
	if ( rc == SQLITE_OK ) {		
		rc = sqlite3_step(count_qry);
		if ( rc == SQLITE_DONE || rc == SQLITE_ROW ){
			strcpy(rslt_str, (char *) sqlite3_column_text(count_qry, 0));
		}
	}	
	sqlite3_finalize(count_qry);
	return rslt_str;
}
char *SQLiteLinuxQueryGetColumnCount() {
	int rc = sqlite3_column_count(qry);
	sprintf(rslt_str, "%d", rc);
	return rslt_str;
}
char *SQLiteLinuxQueryGetNext() {
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_step(qry);
	if ( rc == SQLITE_DONE )
		strcpy(rslt, "-1");	
	else if ( rc != SQLITE_ROW && rc != SQLITE_DONE )
		sprintf(rslt, "SQLiteLinuxQueryGetNext:ERROR:%d:%s", rc, sqlite3_errmsg(db));
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteLinuxQueryGetData(char * col) {
	return (char *)sqlite3_column_text(qry, atoi(col));
}
char *SQLiteLinuxQueryClose() {
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( qry != NULL ){
		int rc = sqlite3_finalize(qry);
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteLinuxQueryClose:ERROR:%d:%s", rc, sqlite3_errmsg(db));
		}
	}
	qry = NULL;
	strcpy(rslt_str, rslt);
	free( rslt );
	return rslt_str;
}
//======================================================================================================
// FOR DLL
#if 1
int main() { return 0; }
#else
//======================================================================================================
// FOR EXE
void execQuery(){
	char *rslt = SQLiteLinuxQueryOpen("SELECT * FROM t1 ORDER BY b;");
	printf("Query open result = %s \n", rslt);
	rslt = SQLiteLinuxQueryGetRowCount();
	printf("SQLiteLinuxQueryGetRowCount = %s \n", rslt);
	rslt = SQLiteLinuxQueryGetColumnCount();
	printf("SQLiteLinuxQueryGetColumnCount = %s \n", rslt);	
	
	if ( strcmp( rslt, "0" ) == 0 ) {
		int i = 0;
		rslt = SQLiteLinuxQueryGetNext();
		while ( strcmp( rslt, "0" ) == 0 ) {
			printf("%d: %s - %s \n", i, SQLiteLinuxQueryGetData("0"), SQLiteLinuxQueryGetData("1") );
			i++;
			rslt = SQLiteLinuxQueryGetNext();
		}
		printf("Query end = %s \n", rslt);
		rslt = SQLiteLinuxQueryClose();
		printf("Query close result = %s \n", rslt);
	}
}

int main() {
	char *rslt = SQLiteLinuxOpenDB("win.db");
	if ( strcmp( rslt, "0" ) != 0 ) {
		printf("Cannot open database. result is %s \n", rslt);
		return 10;
	}

	printf("Opened database = %s \n", rslt);
	rslt = SQLiteLinuxExecuteSQL("DROP TABLE IF EXISTS t1;");
	printf("Drop table = %s \n", rslt);
	rslt = SQLiteLinuxExecuteSQL("CREATE TABLE t1 (a INTEGER PRIMARY KEY, b INTEGER);");
	printf("Created table = %s \n", rslt);
	rslt = SQLiteLinuxExecuteInsert("INSERT INTO t1 VALUES (1, 123)");
	printf("Insert values = %s \n", rslt);
	rslt = SQLiteLinuxExecuteInsert("INSERT INTO t1 VALUES (2, 50)");
	printf("Insert values = %s \n", rslt);

	execQuery();
	
	rslt = SQLiteLinuxExecuteUpdate("UPDATE t1 SET b = 1150 WHERE a = 2;");
	printf("Updated rows = %s \n", rslt);
	
	execQuery();

	rslt = SQLiteLinuxExecuteDelete("DELETE FROM t1 WHERE a = 1;");
	printf("Deleted rows = %s \n", rslt);
	
	execQuery();

	//rslt = SQLiteLinuxQueryOpen("SELECT * FROM t1 ORDER BY b;");
	//printf("Query open result = %s \n", rslt);
	
	rslt = SQLiteLinuxCloseDB();
	printf("Close DB: %s \n", rslt);
	free( rslt );
	return 0;
}
//======================================================================================================
#endif