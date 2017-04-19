package dev.wolveringer.EasyJNI;

import dev.wolveringer.EasyJNI.test.TestStaticFields;

public class Main {
	public static void main(String[] args) {
		EasyJNI.inizalisize();
		
		System.load("/home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug/libEasyJNITest.so");
		
		TestStaticFields.printValues();
		runNativeTests();
		TestStaticFields.printValues();
	}
	
	public static native void runNativeTests();
}
