#import <Foundation/Foundation.h>
//======================================================================================================
@interface SQLiteMacExtension : NSObject {
        NSString *databasePath;
}

-(NSString *)SQLiteMacGetDocumentsPath;
-(NSString *)SQLiteMacOpenDB:(NSString *)db_filename;
-(NSString *)SQLiteMacGetLibVersion;
-(NSString *)SQLiteMacCloseDB;
-(NSString *)SQLiteMacExecuteSQL:(NSString *)sql;
-(NSString *)SQLiteMacExecuteInsert:(NSString *)sql;
-(NSString *)SQLiteMacExecuteUpdate:(NSString *)sql;
-(NSString *)SQLiteMacExecuteDelete:(NSString *)sql;
-(NSString *)SQLiteMacQueryOpen:(NSString *)sql;
-(NSString *)SQLiteMacQueryGetRowCount;
-(NSString *)SQLiteMacQueryGetColumnCount;
-(NSString *)SQLiteMacQueryGetNext;
-(NSString *)SQLiteMacQueryGetData:(NSString *)col;
-(NSString *)SQLiteMacQueryClose;

@end
//======================================================================================================
