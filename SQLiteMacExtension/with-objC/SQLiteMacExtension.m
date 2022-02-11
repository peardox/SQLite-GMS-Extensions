#import "SQLiteMacExtension.h"
#import <Foundation/Foundation.h>
#import "sqlite3.h"
//======================================================================================================
//======================================================================================================
static sqlite3 *database;
static sqlite3_stmt *qry;
static NSString *sql_count_rows;
//======================================================================================================
//======================================================================================================
@implementation SQLiteMacExtension

-(NSString *)SQLiteMacGetDocumentsPath{
        NSArray  *paths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsPath = [paths objectAtIndex:0];
        NSLog (@"SQLiteMacGetDocumentsPath: %@", documentsPath);
        return documentsPath;
}

-(NSString *)SQLiteMacOpenDB:(NSString *)db_filename {
        NSString *rslt = @"-1";
        const char *dbpath = [db_filename UTF8String];
        if ( !database ) {
                int rc = sqlite3_open(dbpath, &database);
                if( rc != SQLITE_OK ){
                        rslt = [NSString stringWithFormat:@"SQLiteMacOpenDB:ERROR:%d:%@", rc, [NSString stringWithUTF8String:dbpath]];
                } else {
                        rslt = [NSString stringWithFormat:@"%d", rc];
                }
        }
        return rslt;
}
-(NSString *)SQLiteMacGetLibVersion{
        const char *libversion = sqlite3_libversion();
        const char *sourceid = sqlite3_sourceid();
        NSString *rslt = [NSString stringWithFormat:@"SQLiteMacGetLibVersion:%@:%@:%d",
                [NSString stringWithUTF8String:libversion],
                [NSString stringWithUTF8String:sourceid],
                sqlite3_libversion_number()];
        return rslt;
}
-(NSString *)SQLiteMacCloseDB{
        NSString *rslt = @"-1";
        if ( database ) {
                int rc = sqlite3_close(database);
                database = 0;
                if( rc != SQLITE_OK ){
                        rslt = [NSString stringWithFormat:@"SQLiteMacCloseDB:ERROR:%d", rc];
                } else {
                        rslt = [NSString stringWithFormat:@"%d", rc];
                }
        }
        return rslt;
}
//======================================================================================================
-(NSString *)SQLiteMacExecuteSQL:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
        int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
        NSLog (@"SQLiteMacExecuteSQL result: %d ", rc);
        if ( rc != SQLITE_OK ) {
                rslt = [NSString stringWithFormat:@"SQLiteMacExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
        }
        return rslt;
}
//======================================================================================================
-(NSString *)SQLiteMacExecuteInsert:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
        int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
        NSLog (@"SQLiteMacExecuteInsert result: %d ", rc);
        if ( rc != SQLITE_OK ) {
                rslt = [NSString stringWithFormat:@"SQLiteMacExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
        } else {
                int row_id = sqlite3_last_insert_rowid(database);
                rslt = [NSString stringWithFormat:@"%d", row_id];
        }
        return rslt;
}
-(NSString *)SQLiteMacExecuteUpdate:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
        int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
        NSLog (@"SQLiteMacExecuteInsert result: %d ", rc);
        if ( rc != SQLITE_OK ) {
                rslt = [NSString stringWithFormat:@"SQLiteMacExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
        } else {
                int row_count = sqlite3_changes(database);
                rslt = [NSString stringWithFormat:@"%d", row_count];
        }
        return rslt;
}
-(NSString *)SQLiteMacExecuteDelete:(NSString *)sql {
        NSString *rslt = @"0";
        char *err_msg;
        int rc = sqlite3_exec(database, [sql UTF8String], NULL, NULL, &err_msg);
        NSLog (@"SQLiteMacExecuteInsert result: %d ", rc);
        if ( rc != SQLITE_OK ) {
                rslt = [NSString stringWithFormat:@"SQLiteMacExecuteSQL:ERROR:%d:%@", rc, [NSString stringWithUTF8String:err_msg]];
        } else {
                int row_count = sqlite3_changes(database);
                rslt = [NSString stringWithFormat:@"%d", row_count];
        }
        return rslt;
}
//======================================================================================================
-(NSString *)SQLiteMacQueryOpen:(NSString *)sql {
        NSString *rslt = @"0";
        sql_count_rows = [NSString stringWithFormat:@"SELECT COUNT(*) AS row_count FROM ( %@ )", sql];
        sql_count_rows = [sql_count_rows stringByReplacingOccurrencesOfString:@";" withString:@" "];
        if ( !qry ){
                int rc = sqlite3_prepare_v2(database, [sql UTF8String], -1, &qry, NULL);
                if ( rc != SQLITE_OK ) {
                        const char *msg = sqlite3_errmsg(database);
                        rslt = [NSString stringWithFormat:@"SQLiteMacQueryOpen:ERROR:%d:%@", rc, [NSString stringWithUTF8String:msg]];
                        sqlite3_finalize(qry);
                        qry = 0;
                }
        }
        return rslt;
}
-(NSString *)SQLiteMacQueryGetRowCount {
        NSString *rslt = @"-1";
        sqlite3_stmt *count_qry;
        NSLog (@"SQLiteMacQueryGetRowCount:sql: %@", sql_count_rows);
        int rc = sqlite3_prepare_v2(database, [sql_count_rows UTF8String], -1, &count_qry, NULL);
        if ( rc == SQLITE_OK ) {
                rc = sqlite3_step(count_qry);
                if ( rc == SQLITE_ROW || rc == SQLITE_DONE ){
                        const unsigned char *rslt1 = sqlite3_column_text(count_qry, 0);
                        rslt = [NSString stringWithFormat:@"%s", rslt1];
                }
        }
        sqlite3_finalize(count_qry);
        return rslt;
}
-(NSString *)SQLiteMacQueryGetColumnCount {
        int rc = sqlite3_column_count(qry);
        NSString *rslt = [NSString stringWithFormat:@"%d", rc];
        return rslt;
}
-(NSString *)SQLiteMacQueryGetNext {
        NSString *rslt = @"0";
        int rc = sqlite3_step(qry);
        if ( rc == SQLITE_DONE )
                rslt = @"-1";
        else if ( rc != SQLITE_ROW && rc != SQLITE_DONE ){
                const char *msg = sqlite3_errmsg(database);
                rslt = [NSString stringWithFormat:@"SQLiteMacQueryGetNext:ERROR:%d:%@", rc, [NSString stringWithUTF8String:msg]];
        }
        return rslt;
}
-(NSString *)SQLiteMacQueryGetData:(NSString *)col {
        const unsigned char *rslt1 = sqlite3_column_text(qry, [col intValue]);
                NSString* rslt2 = [[NSString alloc] initWithBytes:rslt1 length:sizeof(rslt1) encoding:NSASCIIStringEncoding];
        return rslt2;
}
-(NSString *)SQLiteMacQueryClose {
        NSString *rslt = @"0";
        if ( qry ){
                int rc = sqlite3_finalize(qry);
                if ( rc != SQLITE_OK ) {
                        const char *msg = sqlite3_errmsg(database);
                        rslt = [NSString stringWithFormat:@"SQLiteMacQueryClose:ERROR:%d:%@", rc, [NSString stringWithUTF8String:msg]];
                }
        }
        qry = 0;
        return rslt;
}
@end
//======================================================================================================
//======================================================================================================