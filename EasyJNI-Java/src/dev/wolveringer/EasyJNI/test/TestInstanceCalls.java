package dev.wolveringer.EasyJNI.test;

public class TestInstanceCalls {
	public TestInstanceCalls() {
		System.out.println("Constructor");
	}
	
	@Override
	protected void finalize() throws Throwable {
		System.out.println("Deconstructor");
	}
}
