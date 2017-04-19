package dev.wolveringer.EasyJNI.test;

public class TestStaticCalls {
	public static void callMe(){
		System.out.println("I got called!");
	}
	
	public static void callMe(int parm1, int parm2){
		System.out.println("I got called with "+parm1+", "+parm2);
	}
	
	public static int callMe(int args){
		System.out.println("I got called with parm "+args+" and square it");
		return (int) Math.pow(args, 2);
	}
}
