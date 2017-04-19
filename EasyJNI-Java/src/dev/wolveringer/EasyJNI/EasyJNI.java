package dev.wolveringer.EasyJNI;

public class EasyJNI {
	private static final String EasyJNI_LibaryName = "libEasyJNI.so";
	private static final String EasyJNI_LibaryPath = "/home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug/";
	
	private static int inizalisizeState = -1;
	
	public static void inizalisize(){
		try {
			System.load(EasyJNI_LibaryPath + EasyJNI_LibaryName);
			inizalisizeState = 1;
		}catch (Exception e) {
			inizalisizeState = 0;
			e.printStackTrace();
		}
	}
	
	public static boolean isLoaded(){
		return inizalisizeState == 1;
	}
	
	public static boolean isAvariable(){
		return inizalisizeState > 0;
	}
}
