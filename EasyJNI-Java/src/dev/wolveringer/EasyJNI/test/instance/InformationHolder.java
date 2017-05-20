package dev.wolveringer.EasyJNI.test.instance;

import java.util.ArrayList;

public class InformationHolder {
	private BasicClass basicInfo;
	private ArrayList<BasicClass> infos;

	public InformationHolder() {
		infos = new ArrayList<>();
		basicInfo = new BasicClass(this, "undefined", "infos");
	}

	public void print(){
		try {
			System.out.println("Print me from natiove:");
			finalize();
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	@Override
	protected void finalize() throws Throwable {
		System.out.println("Infos: " + (basicInfo == null ? "unknown" : basicInfo.name + " -> " + basicInfo.value+" @"+basicInfo.handle+"/"+this));
		infos.forEach(e -> System.out.println(e.name + " -> " + e.value));
	}
}
