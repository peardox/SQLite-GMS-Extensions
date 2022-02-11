package ${YYAndroidPackageName};

import java.lang.String;
import com.hat.JNIHatWrapper;
import ${YYAndroidPackageName}.R;
import com.yoyogames.runner.RunnerJNILib;

public class SQLiteExtention{

	private static JNIHatWrapper hat;
	
	public SQLiteExtention(){
		hat = new JNIHatWrapper();
	}
	
	public String secho(String var1) {		
		return "You sent:"+var1;
	}
	
	public double sopen(String var1) {		
		System.out.println("OPENING DB 1:"+var1);
		double var = hat.sopen(var1);
		System.out.println("OPENING DB 2:"+var);
		return var;
	}
	
	public double sclose(double var1) {		
		return hat.sclose(var1);
	}
	
	public double serrcode(double var1) {		
		return hat.serrcode(var1);
	}
	
	public String serrmsg(double var1) {		
		return hat.serrmsg(var1);
	}
	
	public double squery(double var1, String var2) {		
		return hat.squery(var1, var2);
	}
	
	public double sfree(double var1) {		
		return hat.sfree(var1);
	}
	
	public double sstep(double var1) {		
		return hat.sstep(var1);
	}
	
	public double sreset(double var1) {		
		return hat.sreset(var1);
	}
	
	public double sdouble(double var1, double var2) {		
		return hat.sdouble(var1, var2);
	}
	
	public String stext(double var1, double var2) {		
		return hat.stext(var1, var2);
	}

	public double schanges(double var1) {		
		return hat.schanges(var1);
	}
	
	public double slastid(double var1) {		
		return hat.slastid(var1);
	}

}