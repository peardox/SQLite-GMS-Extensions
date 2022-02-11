#include "sqlite3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

sqlite3 *db;
sqlite3_stmt *qry = NULL;
char *rslt_str = NULL;
char *sql_count_rows = NULL;
//======================================================================================================
char *SQLiteMacOpenDB(char * db_filename) {
	if ( rslt_str == NULL )
		rslt_str = (char *) malloc(1024);
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( db == NULL ){
		int rc = sqlite3_open( db_filename, &db );
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteMacOpenDB:ERROR:%d:%s:%s", rc, sqlite3_errmsg(db), db_filename);
			sqlite3_close(db);
			db = NULL;
		}
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteMacGetLibVersion(){
	char * rslt = (char *) malloc(1024);
	sprintf(rslt, "SQLiteMacGetLibVersion:%s:%s:%d", sqlite3_libversion(), sqlite3_sourceid(), sqlite3_libversion_number());
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteMacCloseDB() {
	char * rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_close_v2(db);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteMacCloseDB:ERROR:%d:%s", rc, sqlite3_errmsg(db));	
	}
	db = NULL;
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
char *SQLiteMacExecuteSQL(char *sql){
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	char *err_msg;
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteMacExecuteSQL:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
//======================================================================================================
char *SQLiteMacExecuteInsert(char *sql){
	char *err_msg = 0;
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteMacExecuteInsert:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	} else {
		sprintf( rslt, "%lld", sqlite3_last_insert_rowid(db) );
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteMacExecuteUpdate(char * sql){
	char *err_msg = 0;
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteMacExecuteUpdate:ERROR:%d:%s", rc, err_msg);
		sqlite3_free(err_msg);
	} else {
		sprintf(rslt, "%d", sqlite3_changes(db));
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteMacExecuteDelete(char * sql){
	char *err_msg = 0;
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
	if ( rc != SQLITE_OK ) {
		sprintf(rslt, "SQLiteMacExecuteDelete:ERROR:%d:%s", rc, err_msg);
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
char *SQLiteMacQueryOpen(char *sql) {
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
			sprintf(rslt, "SQLiteMacQueryOpen:ERROR:%d:%s", rc, sqlite3_errmsg(db));
			sqlite3_finalize(qry);
			qry = NULL;
		}
	}
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteMacQueryGetRowCount() {
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
char *SQLiteMacQueryGetColumnCount() {
	int rc = sqlite3_column_count(qry);
	sprintf(rslt_str, "%d", rc);
	return rslt_str;
}
char *SQLiteMacQueryGetNext() {
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	int rc = sqlite3_step(qry);
	if ( rc == SQLITE_DONE )
		strcpy(rslt, "-1");	
	else if ( rc != SQLITE_ROW && rc != SQLITE_DONE )
		sprintf(rslt, "SQLiteMacQueryGetNext:ERROR:%d:%s", rc, sqlite3_errmsg(db));
	strcpy(rslt_str, rslt);
	free(rslt);
	return rslt_str;
}
char *SQLiteMacQueryGetData(char * col) {
	return (char *)sqlite3_column_text(qry, atoi(col));
}
char *SQLiteMacQueryClose() {
	char *rslt = (char *) malloc(1024);
	strcpy(rslt, "0");
	if ( qry != NULL ){
		int rc = sqlite3_finalize(qry);
		if ( rc != SQLITE_OK ) {
			sprintf(rslt, "SQLiteMacQueryClose:ERROR:%d:%s", rc, sqlite3_errmsg(db));
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
	char *rslt = SQLiteMacQueryOpen("SELECT * FROM t1 ORDER BY b;");
	printf("Query open result = %s \n", rslt);
	rslt = SQLiteMacQueryGetRowCount();
	printf("SQLiteMacQueryGetRowCount = %s \n", rslt);
	rslt = SQLiteMacQueryGetColumnCount();
	printf("SQLiteMacQueryGetColumnCount = %s \n", rslt);	
	
	if ( strcmp( rslt, "0" ) == 0 ) {
		int i = 0;
		rslt = SQLiteMacQueryGetNext();
		while ( strcmp( rslt, "0" ) == 0 ) {
			printf("%d: %s - %s \n", i, SQLiteMacQueryGetData("0"), SQLiteMacQueryGetData("1") );
			i++;
			rslt = SQLiteMacQueryGetNext();
		}
		printf("Query end = %s \n", rslt);
		rslt = SQLiteMacQueryClose();
		printf("Query close result = %s \n", rslt);
	}
}

int main() {
	char *rslt = SQLiteMacOpenDB("win.db");
	if ( strcmp( rslt, "0" ) != 0 ) {
		printf("Cannot open database. result is %s \n", rslt);
		return 10;
	}

	printf("Opened database = %s \n", rslt);
	rslt = SQLiteMacExecuteSQL("DROP TABLE IF EXISTS t1;");
	printf("Drop table = %s \n", rslt);
	rslt = SQLiteMacExecuteSQL("CREATE TABLE t1 (a INTEGER PRIMARY KEY, b INTEGER);");
	printf("Created table = %s \n", rslt);
	rslt = SQLiteMacExecuteInsert("INSERT INTO t1 VALUES (1, 123)");
	printf("Insert values = %s \n", rslt);
	rslt = SQLiteMacExecuteInsert("INSERT INTO t1 VALUES (2, 50)");
	printf("Insert values = %s \n", rslt);

	execQuery();
	
	rslt = SQLiteMacExecuteUpdate("UPDATE t1 SET b = 1150 WHERE a = 2;");
	printf("Updated rows = %s \n", rslt);
	
	execQuery();

	rslt = SQLiteMacExecuteDelete("DELETE FROM t1 WHERE a = 1;");
	printf("Deleted rows = %s \n", rslt);
	
	execQuery();

	//rslt = SQLiteMacQueryOpen("SELECT * FROM t1 ORDER BY b;");
	//printf("Query open result = %s \n", rslt);
	
	rslt = SQLiteMacCloseDB();
	printf("Close DB: %s \n", rslt);
	free( rslt );
	return 0;
}
//======================================================================================================
#endif