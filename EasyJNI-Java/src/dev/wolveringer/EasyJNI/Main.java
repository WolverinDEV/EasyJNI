package dev.wolveringer.EasyJNI;

import dev.wolveringer.EasyJNI.test.TestStaticFields;

public class Main {
	public static void main(String[] args) throws Exception {
		EasyJNI.inizalisize();
		
		System.out.println("Loading test lib");
		System.load(EasyJNI.EasyJNI_LibaryPath+"/libEasyJNITest.so");
		
		System.out.println("[]------------------- Before test -------------------[]");
		TestStaticFields.printValues();
		System.out.println("[]------------------- Before test -------------------[]");
		
		runNativeTests();
		
		System.out.println("[]------------------- After  test -------------------[]");
		TestStaticFields.printValues();
		System.out.println("[]------------------- After  test -------------------[]");
		
		System.out.println("[]----------------- Finalize & Exit -----------------[]");
		System.runFinalization();
		System.runFinalizersOnExit(true);
		//crashVM();
	}
	
	public static void a(){
		System.out.println("A");
	}
	
	public static native void runNativeTests();
	public static native void crashVM();
}
