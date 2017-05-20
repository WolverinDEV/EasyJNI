package dev.wolveringer.EasyJNI.test.speed;

import java.util.ArrayList;
import java.util.List;

import dev.wolveringer.EasyJNI.EasyJNI;

public class Main {
	public static native void setupTest();
	public static native void runNativeTests();
	
	public static List<StringClass> strings = new ArrayList<>(150 * 1000);
	
	private static void runTimedTest(String name, Runnable run){
		System.out.println();
		System.out.println("Running test "+name);
		long start, end;
		start = System.currentTimeMillis();
		run.run();
		end = System.currentTimeMillis();
		System.out.println();
		System.out.println("Test "+name);
		System.out.println("Needed time: "+(end-start)+"ms");
	}
	
	private static void doJava(){
		for(int i = 0;i<150 * 1000;i++)
			strings.add(new StringClass("Hello world ("+i+")"));
	}
	
	public static void main(String[] args) {
		EasyJNI.inizalisize();
		System.load(EasyJNI.EasyJNI_LibaryPath+"/libEasyJNISpeedTest.so");
		
		System.out.println("Setup tests!");
		setupTest();
		
		System.out.println("Run tests!");
		System.out.println();
		runTimedTest("Native", () -> runNativeTests());
		printStrings();
		
		strings.clear();
		runTimedTest("Java", () -> doJava());
		printStrings();
	}
	
	private static void printStrings(){
		System.out.println("Having "+strings.size()+" strings");
		if(strings.size() < 100)
			strings.forEach(e -> System.out.println(" - '"+e.getValue()+"'"));
	}
}
