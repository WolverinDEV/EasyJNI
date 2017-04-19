package dev.wolveringer.EasyJNI.test;

public class TestStaticFields {
	public static boolean ChangeMe = false;
	public static int ChangeMeTo22 = 11;
	
	public static void printValues(){
		System.out.println("ChangeMe: "+ChangeMe+" ChangeMeTo22: "+ChangeMeTo22);
	}
}
