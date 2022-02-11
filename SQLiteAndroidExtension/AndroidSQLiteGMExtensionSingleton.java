package ${YYAndroidPackageName};

import android.database.sqlite.SQLiteDatabase;
import com.readystatesoftware.sqliteasset.SQLiteAssetHelper;
import android.util.Log;
 
import ${YYAndroidPackageName}.R;
import com.yoyogames.runner.RunnerJNILib;
import ${YYAndroidPackageName}.RunnerActivity;

public class AndroidSQLiteGMExtensionSingleton extends SQLiteAssetHelper { 
//-----------------------------------------------------------------------------------------------------------
	private static class AndroidSQLiteGMExtensionSingletonSingletonHelper{
        private static final AndroidSQLiteGMExtensionSingleton INSTANCE = new AndroidSQLiteGMExtensionSingleton();
    }	
    public static AndroidSQLiteGMExtensionSingleton getInstance(){
        return AndroidSQLiteGMExtensionSingletonSingletonHelper.INSTANCE;
    }	
//-----------------------------------------------------------------------------------------------------------
	private static final int DATABASE_VERSION = AndroidSQLiteGMExtension.DATABASE_VERSION;	
	private static final String DATABASE_NAME = AndroidSQLiteGMExtension.DATABASE_NAME;	
	private void printout(String msg){
		Log.d("yoyo", msg);
	}
	private AndroidSQLiteGMExtensionSingleton() {
		super(RunnerJNILib.ms_context, DATABASE_NAME, null, DATABASE_VERSION);  
		printout("AndroidSQLiteGMExtensionSingleton:"+RunnerJNILib.ms_context.toString());	
	}
//-----------------------------------------------------------------------------------------------------------	
}