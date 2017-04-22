package dev.wolveringer.EasyJNI;

import java.lang.reflect.Method;

import dev.wolveringer.EasyJNI.test.TestStaticFields;

public class Main {
	public static void main(String[] args) throws Exception {
		EasyJNI.inizalisize();
		
		System.load(EasyJNI.EasyJNI_LibaryPath+"/libEasyJNITest.so");
		
		TestStaticFields.printValues();
		//runNativeTests();
		runNativeTests();
		TestStaticFields.printValues();
		
		System.runFinalization();
		System.runFinalizersOnExit(true);
	}
	
	public static void a(){
		System.out.println("A");
	}
	
	public static native void runNativeTests();
}
