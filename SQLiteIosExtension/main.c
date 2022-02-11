
int main (int argc, const char *argv[]) {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        NSLog (@"Get Instance!");
        //DataBaseManager *mydb = (DataBaseManager *) [DataBaseManager getInstance];
		DataBaseManager *mydb = [[DataBaseManager alloc] init];
        NSLog (@"Open DB!");
        NSString *open_db = [mydb SQLiteIosOpenDB: @"/root/obj-C/ios.db"];
        NSLog (@"Open DB result: %@ ", open_db);
        NSString *ver = [mydb SQLiteIosGetLibVersion];
        NSLog (@"%@", ver);
        NSString *crt_tbl = [mydb SQLiteIosExecuteSQL: @"create table if not exists EMPLOYEE (employeeID integer primary key NOT NULL, employeeName text NOT NULL, employeeImage blob, employeePhoneNumber text NOT NULL);"];
        NSLog (@"Create table result: %@", crt_tbl);
        NSString *exct_sql = [mydb SQLiteIosExecuteSQL: @"INSERT INTO EMPLOYEE (employeeID, employeeName, employeePhoneNumber) values (10, 'marios', '555-112233');"];
        NSLog (@"Insert statement result: %@", exct_sql);
		
		NSString *insert_stmt = [mydb SQLiteIosExecuteInsert: @"INSERT INTO EMPLOYEE (employeeID, employeeName, employeePhoneNumber) values (110, 'sotos', '555-992233');"];
        NSLog (@"Insert: %@", insert_stmt);
		NSString *update_stmt = [mydb SQLiteIosExecuteUpdate: @"UPDATE EMPLOYEE SET employeeName = 'sotos2' WHERE employeeID = 110;"];
        NSLog (@"Update: %@", update_stmt);
		NSString *delete_stmt = [mydb SQLiteIosExecuteDelete: @"DELETE FROM EMPLOYEE WHERE employeeID = 110;"];
        NSLog (@"Delete: %@", delete_stmt);

		NSString *qry_rslt = [mydb SQLiteIosQueryOpen: @"SELECT * FROM EMPLOYEE;"];
		NSLog (@"Query Open: %@", qry_rslt);
		NSString *col_count = [mydb SQLiteIosQueryGetColumnCount];
		NSLog (@"Query Column Count: %@", col_count);
		NSString *get_next = [mydb SQLiteIosQueryGetNext];
		NSLog (@"Query Get Next: %@", get_next);
		NSString *qry_data = [mydb SQLiteIosQueryGetData: @"0"];
		NSLog (@"Query Get Data [0]: %@", qry_data);
		qry_data = [mydb SQLiteIosQueryGetData: @"1"];
		NSLog (@"Query Get Data [1]: %@", qry_data);
		get_next = [mydb SQLiteIosQueryGetNext];
		NSLog (@"Query Get Next: %@", get_next);	
		NSString *qry_close = [mydb SQLiteIosQueryClose];
		NSLog (@"Query Close: %@", qry_close);	

        NSString *close_db = [mydb SQLiteIosCloseDB];
        NSLog (@"Close DB result: %@ ", close_db);
        NSLog (@"end!");
        [pool drain];
        return 0;
}
//======================================================================================================
//======================================================================================================