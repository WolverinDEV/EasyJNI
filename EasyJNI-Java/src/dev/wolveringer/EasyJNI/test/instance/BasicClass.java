package dev.wolveringer.EasyJNI.test.instance;

public class BasicClass {
	public InformationHolder handle;
	public String name;
	public String value;
	
	BasicClass(InformationHolder handle, String name, String value){
		System.out.println("CONS -> "+handle+" -> "+name+" -> "+value);
		this.name = name;
		this.value = value;
		this.handle = handle;
	}
	
	@Override
	protected void finalize() throws Throwable {
		System.out.println("Finalize basic class ("+this+" -> ["+handle+", "+name+", "+value+"])");
	}
}
