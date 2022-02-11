package ${YYAndroidPackageName};

import android.util.Log;
import android.database.sqlite.SQLiteDatabase;
import android.database.Cursor;
import android.content.ContentValues;
import java.util.Map;
import java.util.List;
import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
 
import ${YYAndroidPackageName}.R;
import com.yoyogames.runner.RunnerJNILib;
import ${YYAndroidPackageName}.RunnerActivity;

public class AndroidSQLiteGMExtension { 
//-----------------------------------------------------------------------------------------------------------
	private void printout(String msg){
		Log.d("yoyo", msg);
	}
//-----------------------------------------------------------------------------------------------------------
	protected static String DATABASE_NAME = "mysqlite.db";	
	protected static int DATABASE_VERSION = 1;
	private static SQLiteDatabase db      = null;
	public static String SetDBNameAndVersion(String dbName, String version){
		//TODO: Version Upgrade
		String result = "0";
		try{		
			DATABASE_NAME    = dbName;
			DATABASE_VERSION = Integer.parseInt(version);
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:SetDBNameAndVersion:Exception="+e.getMessage();
		}
		return result;			
	}
	public String OpenDB(){
		String result = "0";
		Cursor cursor = null;
		try{
			db = AndroidSQLiteGMExtensionSingleton.getInstance().getWritableDatabase();		
			cursor = db.rawQuery("SELECT name FROM sqlite_master WHERE type='table'", null);
			if (cursor.moveToFirst()) {
				do {
					printout("AndroidSQLiteGMExtension:OpenDB:table_list="+cursor.getString(0));
				} while (cursor.moveToNext());
			}		
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:OpenDB:Exception="+e.getMessage();
		}finally{
			if(cursor != null)
				cursor.close();
		}
		return result;
	}
//-----------------------------------------------------------------------------------------------------------	
	public String PrintDebugInfo(){
		String result = "0";
		Cursor cursor = null;
		try{
			printout("AndroidSQLiteGMExtension:PrintDebugInfo:DB_NAME="+this.DATABASE_NAME);
			printout("AndroidSQLiteGMExtension:PrintDebugInfo:getAttachedDbs="+ db.getAttachedDbs() );
			printout("AndroidSQLiteGMExtension:PrintDebugInfo:getPath="+ db.getPath() );
			printout("AndroidSQLiteGMExtension:PrintDebugInfo:getVersion="+ db.getVersion() );			
			cursor = db.rawQuery("SELECT name FROM sqlite_master WHERE type='table'", null);
			if (cursor.moveToFirst()) {
				do {
					printout("AndroidSQLiteGMExtension:PrintDebugInfo:table_list="+cursor.getString(0));
				} while (cursor.moveToNext());
			}		
		}catch(Exception e){
			printout("AndroidSQLiteGMExtension:PrintDebugInfo:Exception="+e.getMessage());
			result = "AndroidSQLiteGMExtension:PrintDebugInfo:Exception="+e.getMessage();
		}finally{
			if(cursor != null)
				cursor.close();
		}
		return result;
	}	
//-----------------------------------------------------------------------------------------------------------
	public String ExecuteSQL(String statement){
		String result = "0";
		try{
			db.execSQL(statement);
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:PrintDebugInfo:Exception="+e.getMessage();
		}
		return result;
	}	
//-----------------------------------------------------------------------------------------------------------
	private static Map<String, ContentValues> contentValues = new ConcurrentHashMap<String, ContentValues>();
	private static long contentValuesSequence = 1;	
	public synchronized String ContentValuesCreate() {
		String result = "0";
		try{			
			ContentValues data = new ContentValues();
			contentValues.put( ""+contentValuesSequence, data );
			contentValuesSequence += 1;
			result = ""+(contentValuesSequence - 1);		
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:ContentValuesCreate:Exception="+e.getMessage();
		}
		return result;			
	}	
	public String ContentValuesPut(String contentValueId, String key, String value) {
		String result = "0";
		try{			
			ContentValues data = contentValues.get( contentValueId );
			data.put(key, value);
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:ContentValuesPut:Exception="+e.getMessage();
		}
		return result;				
	}
	public String ContentValuesDelete(String contentValueId) {
		String result = "0";
		try{		
			contentValues.remove( contentValueId );
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:ContentValuesDelete:Exception="+e.getMessage();
		}
		return result;			
	}
//-----------------------------------------------------------------------------------------------------------	
	public String ExecuteInsert(String contentValueId, String tableName) {
		String result = "0";
		try{
			ContentValues data = contentValues.get( contentValueId );
			long insertedId = db.insert(tableName, null, data);
			result = ""+insertedId;
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:ExecuteInsert:Exception="+e.getMessage();
		}
		return result;
	}
	public String ExecuteUpdate(String contentValueId, String tableName, String id) {
		String result = "0";
		try{		
			ContentValues data = contentValues.get( contentValueId );
			int updatedCount = db.update(tableName, data, "_ID = ?", new String[]{id});
			result = ""+updatedCount;
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:ExecuteUpdate:Exception="+e.getMessage();
		}
		return result;			
	}
	public String ExecuteDelete(String tableName, String id) {
		String result = "0";
		try{			
			int deletedCount = db.delete(tableName, "_ID = ?", new String[]{id});
			result = ""+deletedCount;
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:ExecuteDelete:Exception="+e.getMessage();
		}
		return result;			
	}
//-----------------------------------------------------------------------------------------------------------
	private static Map<String, List> cursors = new ConcurrentHashMap<String, List>();
	private static long cursorSequence = 1;
	private List<List> putCursorToList(Cursor cursor){
		List<List> 	 data 	 = null;
		List<String> rowdata = null;
        if (cursor.moveToFirst()) {
			data = new ArrayList<List>();
            do {
				rowdata = new ArrayList<String>();				
				for(int i=0; i < cursor.getColumnCount(); i++){
					rowdata.add( cursor.getString(i) );
				}				
				data.add(rowdata);
            } while (cursor.moveToNext());
        }
		return data;
	}	
	private synchronized long putDataToCursors(List<List> data){
		long cursorSequenceId = -1;
		if (data != null) {
			cursors.put( ""+cursorSequence, data );
			cursorSequence += 1;
			cursorSequenceId = cursorSequence - 1;
		}
		return cursorSequenceId;
	}	
//-----------------------------------------------------------------------------------------------------------	
	public String CursorOpen(String query) {
		List<List> data = null;
		String result = "0";
		Cursor cursor = null;
		try{
			cursor = db.rawQuery(query, null);
			data = putCursorToList(cursor);
			result = ""+putDataToCursors(data);
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:CursorOpen:Exception="+e.getMessage();
		}finally{
			if(cursor != null)
				cursor.close();
		}
		return result;
	}
	public String CursorClose(String cursorId) {
		String result = "0";
		try{		
			cursors.remove( ""+cursorId );
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:CursorClose:Exception="+e.getMessage();
		}
		return result;	
	}	
//-----------------------------------------------------------------------------------------------------------
	public String CursorGetRowCount(String cursorId) {
		String result = "0";
		try{
			List<List> data = cursors.get( cursorId );
			result = ""+data.size();
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:CursorGetRowCount:Exception="+e.getMessage();
		}
		return result;		
	}
	public String CursorGetColumnCount(String cursorId) {
		String result = "0";
		try{		
			List<List> data = cursors.get( cursorId );
			List<String> rowdata = (List<String>)data.get(0);
			result = ""+rowdata.size();
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:CursorGetColumnCount:Exception="+e.getMessage();
		}		
		return result;	
	}
	public String CursorGetData(String cursorId, String row, String column) {
		String result = "0";
		try{		
			List<List> data = cursors.get( cursorId );
			List<String> rowdata = (List<String>)data.get(Integer.parseInt(row));
			result = rowdata.get(Integer.parseInt(column));
		}catch(Exception e){
			result = "AndroidSQLiteGMExtension:CursorGetData:Exception="+e.getMessage();
		}		
		return result;				
	}
//-----------------------------------------------------------------------------------------------------------
}