#import <Foundation/Foundation.h>
//======================================================================================================
@interface SQLiteIosExtension : NSObject {
        NSString *databasePath;
}

-(NSString *)SQLiteIosGetDocumentsPath;
-(NSString *)SQLiteIosOpenDB:(NSString *)db_filename;
-(NSString *)SQLiteIosGetLibVersion;
-(NSString *)SQLiteIosCloseDB;
-(NSString *)SQLiteIosExecuteSQL:(NSString *)sql;
-(NSString *)SQLiteIosExecuteInsert:(NSString *)sql;
-(NSString *)SQLiteIosExecuteUpdate:(NSString *)sql;
-(NSString *)SQLiteIosExecuteDelete:(NSString *)sql;
-(NSString *)SQLiteIosQueryOpen:(NSString *)sql;
-(NSString *)SQLiteIosQueryClose;
-(NSString *)SQLiteIosQueryGetColumnCount;
-(NSString *)SQLiteIosQueryGetNext;
-(NSString *)SQLiteIosQueryGetData:(NSString *)col;

@end
//======================================================================================================