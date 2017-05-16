package dev.wolveringer.EasyJNI.test;

public class TestInstanceFields {
	public TestInstanceFields() {
		System.out.println("Allocated test field instance");
	}
	
	public boolean changeMe = false;

	@Override
	protected void finalize() throws Throwable {
		System.out.println("Deallocated test field instance! Field: "+changeMe);
	}
}
