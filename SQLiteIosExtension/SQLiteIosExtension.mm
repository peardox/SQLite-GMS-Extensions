#import "SQLiteIosExtension.h"
#import <Foundation/Foundation.h>
#import <sqlite3.h>
//======================================================================================================
//======================================================================================================
static sqlite3 *database;
static sqlite3_stmt *qry;
//======================================================================================================
//======================================================================================================
@implementation SQLiteIosExtension

-(NSString *)SQLiteIosGetDocumentsPath{
        NSArray  *paths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [paths objectAtIndex:0];
        NSLog (@"SQLiteIosGetDocumentsPath: %@", documentsPath);
        return documentsPath;
}

-(NSString *)SQLiteIosOpenDB:(NSString *)db_filename {
        NSString *rslt = @"-1";
        const char *dbpath = [db_filename UTF8String];
        if ( !database ) {
                int rc = sqlite3_open(dbpath, &database);
                if( rc != SQLITE_OK ){
                	rslt = [NSString stringWithFormat:@"SQLiteIosOpenDB:ERROR:%d:%@", rc, [NSString stringWithUTF8String:dbpath]];					
				} else {
					rslt = [NSString stringWithFormat:@"%d", rc];
				}					
        }
        return rslt;
}
-(NSString *)SQLiteIosGetLibVersion{
        const char *libversion = sqlite3_libversion();
        const char *sourceid = sqlite3_sourceid();
        NSString *rslt = [NSString stringWithFormat:@"SQLiteIosGetLibVersion:%@:%@:%d",
                [NSString stringWithUTF8String:libversion],
                [NSString stringWithUTF8String:sourceid],
                sqlite3_libversion_number()];
        return rslt;
}
-(NSString *)SQLiteIosCloseDB{
        NSString *rslt = @"-1";
        if ( database ) {
                int rc = sqlite3_close(database);
				database = 0;
                if( rc != SQLITE_OK ){
					rslt = [NSString stringWithFormat:@"SQLiteIosCloseDB:ERROR:%d", rc];					
				} else {
					rslt = [NSString stringWithFormat:@"%d", rc];
				}
        }
        return rslt;
}
//======================================================================================================
-(NSString *)SQLiteIosExecuteSQL:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
        int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
        NSLog (@"SQLiteIosExecuteSQL result: %d ", rc);
        if ( rc != SQLITE_OK ) {
                rslt = [NSString stringWithFormat:@"SQLiteIosExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
        }
        return rslt;
}
//======================================================================================================
-(NSString *)SQLiteIosExecuteInsert:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
		int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
				NSLog (@"SQLiteIosExecuteInsert result: %d ", rc);
		if ( rc != SQLITE_OK ) {
				rslt = [NSString stringWithFormat:@"SQLiteIosExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
		} else {
                int row_id = sqlite3_last_insert_rowid(database);
                rslt = [NSString stringWithFormat:@"%d", row_id];
        }
        return rslt;
}
-(NSString *)SQLiteIosExecuteUpdate:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
		int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
				NSLog (@"SQLiteIosExecuteInsert result: %d ", rc);
		if ( rc != SQLITE_OK ) {
				rslt = [NSString stringWithFormat:@"SQLiteIosExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
		} else {
                int row_count = sqlite3_changes(database);
                rslt = [NSString stringWithFormat:@"%d", row_count];
        }
        return rslt;
}
-(NSString *)SQLiteIosExecuteDelete:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
		int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
				NSLog (@"SQLiteIosExecuteInsert result: %d ", rc);
		if ( rc != SQLITE_OK ) {
				rslt = [NSString stringWithFormat:@"SQLiteIosExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
		} else {
                int row_count = sqlite3_changes(database);
                rslt = [NSString stringWithFormat:@"%d", row_count];
        }
        return rslt;
}
//======================================================================================================
-(NSString *)SQLiteIosQueryOpen:(NSString *)sql {
        NSString *rslt = @"0";
        if ( !qry ){
                int rc = sqlite3_prepare_v2(database, [sql UTF8String], -1, &qry, NULL);
                if ( rc != SQLITE_OK ) {
                        const char *msg = sqlite3_errmsg(database);
                        rslt = [NSString stringWithFormat:@"SQLiteIosQueryOpen:ERROR:%d:%@", rc, [NSString stringWithUTF8String:msg]];
                        sqlite3_finalize(qry);
                        qry = 0;
                }
        }
        return rslt;
}
-(NSString *)SQLiteIosQueryClose {
        NSString *rslt = @"0";
        if ( qry ){
                int rc = sqlite3_finalize(qry);
                if ( rc != SQLITE_OK ) {
                        const char *msg = sqlite3_errmsg(database);
                        rslt = [NSString stringWithFormat:@"SQLiteIosQueryClose:ERROR:%d:%@", rc, [NSString stringWithUTF8String:msg]];
                }
        }
        qry = 0;
        return rslt;
}
-(NSString *)SQLiteIosQueryGetColumnCount {
        int rc = sqlite3_column_count(qry);
        NSString *rslt = [NSString stringWithFormat:@"%d", rc];
        return rslt;
}
-(NSString *)SQLiteIosQueryGetNext {
        NSString *rslt = @"0";
        int rc = sqlite3_step(qry);
        if ( rc == SQLITE_DONE )
                rslt = @"-1";
        else if ( rc != SQLITE_ROW && rc != SQLITE_DONE ){
                const char *msg = sqlite3_errmsg(database);
                rslt = [NSString stringWithFormat:@"SQLiteIosQueryGetNext:ERROR:%d:%@", rc, [NSString stringWithUTF8String:msg]];
        }
        return rslt;
}
-(NSString *)SQLiteIosQueryGetData:(NSString *)col {
        const unsigned char *rslt1 = sqlite3_column_text(qry, [col intValue]);
		NSString* rslt2 = [[NSString alloc] initWithBytes:rslt1 length:sizeof(rslt1) encoding:NSASCIIStringEncoding];
        return rslt2;
}
@end
//======================================================================================================
//======================================================================================================