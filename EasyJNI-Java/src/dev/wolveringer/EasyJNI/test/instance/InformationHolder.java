package dev.wolveringer.EasyJNI.test.instance;

import java.util.ArrayList;

public class InformationHolder {
	private BasicClass basicInfo;
	private ArrayList<BasicClass> infos;

	public InformationHolder() {
		infos = new ArrayList<>();
		basicInfo = new BasicClass("undefined", "infos");
	}

	@Override
	protected void finalize() throws Throwable {
		System.out.println("Infos: " + (basicInfo == null ? "unknown" : basicInfo.name + " " + basicInfo.value));
		infos.forEach(e -> System.out.println(e.name + " -> " + e.value));
	}
}
