package dev.wolveringer.EasyJNI;

import dev.wolveringer.EasyJNI.test.TestStaticFields;

public class Main {
	public static void main(String[] args) {
		EasyJNI.inizalisize();
		
		System.load(EasyJNI.EasyJNI_LibaryPath+"/libEasyJNITest.so");
		
		TestStaticFields.printValues();
		runNativeTests();
		TestStaticFields.printValues();
	}
	
	public static native void runNativeTests();
}
