int main (int argc, const char *argv[]) {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        NSLog (@"Get Instance!");
        //DataBaseManager *mydb = (DataBaseManager *) [DataBaseManager getInstance];
        SQLiteMacExtension *mydb = [[SQLiteMacExtension alloc] init];
        NSLog (@"Open DB!");
        NSString *docPath = [mydb SQLiteMacGetDocumentsPath];
        NSLog (@"My document's path is : %@", docPath);
        NSString *open_db = [mydb SQLiteMacOpenDB: @"/root/obj-C/ios.db"];
        NSLog (@"Open DB result: %@ ", open_db);
        NSString *ver = [mydb SQLiteMacGetLibVersion];
        NSLog (@"%@", ver);
        NSString *crt_tbl = [mydb SQLiteMacExecuteSQL: @"create table if not exists EMPLOYEE (employeeID integer primary key NOT NULL, employeeName text NOT NULL, employeeImage blob, employeePhoneNumber text NOT NULL);"];
        NSLog (@"Create table result: %@", crt_tbl);
        NSString *exct_sql = [mydb SQLiteMacExecuteSQL: @"INSERT INTO EMPLOYEE (employeeID, employeeName, employeePhoneNumber) values (10, 'marios', '555-112233');"];
        NSLog (@"Insert statement result: %@", exct_sql);

	NSString *insert_stmt = [mydb SQLiteMacExecuteInsert: @"INSERT INTO EMPLOYEE (employeeID, employeeName, employeePhoneNumber) values (110, 'sotos', '555-992233');"];
        NSLog (@"Insert: %@", insert_stmt);
	NSString *update_stmt = [mydb SQLiteMacExecuteUpdate: @"UPDATE EMPLOYEE SET employeeName = 'sotos2' WHERE employeeID = 110;"];
        NSLog (@"Update: %@", update_stmt);
	NSString *delete_stmt = [mydb SQLiteMacExecuteDelete: @"DELETE FROM EMPLOYEE WHERE employeeID = 110;"];
        NSLog (@"Delete: %@", delete_stmt);

	NSString *qry_rslt = [mydb SQLiteMacQueryOpen: @"SELECT * FROM EMPLOYEE;"];
	NSLog (@"Query Open: %@", qry_rslt);
	NSString *row_count = [mydb SQLiteMacQueryGetRowCount];
	NSLog (@"Query ROW Count: %@", row_count);
	NSString *col_count = [mydb SQLiteMacQueryGetColumnCount];
	NSLog (@"Query Column Count: %@", col_count);
	NSString *get_next = [mydb SQLiteMacQueryGetNext];
	NSLog (@"Query Get Next: %@", get_next);
	NSString *qry_data = [mydb SQLiteMacQueryGetData: @"0"];
	NSLog (@"Query Get Data [0]: %@", qry_data);
	qry_data = [mydb SQLiteMacQueryGetData: @"1"];
	NSLog (@"Query Get Data [1]: %@", qry_data);
	get_next = [mydb SQLiteMacQueryGetNext];
	NSLog (@"Query Get Next: %@", get_next);
	NSString *qry_close = [mydb SQLiteMacQueryClose];
	NSLog (@"Query Close: %@", qry_close);

        NSString *close_db = [mydb SQLiteMacCloseDB];
        NSLog (@"Close DB result: %@ ", close_db);
        NSLog (@"end!");
        [pool drain];
        return 0;
}
//======================================================================================================
//======================================================================================================