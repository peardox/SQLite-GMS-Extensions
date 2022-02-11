package com.hat;
import java.lang.String;

public class JNIHatWrapper  {

	public native double sopen(String var1);
	public native double sclose(double var1);
	public native double serrcode(double var1);
	public native String serrmsg(double var1);
	public native double squery(double var1, String var2);
	public native double sfree(double var1);
	public native double sstep(double var1);
	public native double sreset(double var1);
	public native double sdouble(double var1, double var2);
	public native String stext(double var1, double var2);
	public native double schanges(double var1);
	public native double slastid(double var1);
	
    static {
        System.loadLibrary("hat");
    }
}
