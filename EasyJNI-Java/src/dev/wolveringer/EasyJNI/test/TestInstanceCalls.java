package dev.wolveringer.EasyJNI.test;

public class TestInstanceCalls {
	public TestInstanceCalls() {
		System.out.println("Constructor");
	}
	
	public boolean callMe(){
		System.out.println("instance -> callMe() called");
		
		return true;
	}
	
	@Override
	protected void finalize() throws Throwable {
		System.out.println("Deconstructor");
	}
}
