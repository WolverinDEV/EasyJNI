#include <jni.h>
#include <iostream>
#include "../include/Utils.h"
#include "../include/class/JavaMethodeImpl.h"
#include "../include/class/JavaFieldImpl.h"
#include "TestJavaClasses.h"

#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdio>

using namespace std;

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestStaticCalls,
          JNI_STATIC_METHODE("callMe", static_CallMe, void)
          JNI_STATIC_METHODE("callMe", static_CallMe2, void, int, int)
          JNI_STATIC_METHODE("callMe", static_CallMeSqr, int, int)
          //JNI_STATIC_FIELD(bool, changeMe, field_changeMe)
)

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestStaticFields,
          JNI_STATIC_FIELD(bool, ChangeMe, static_changeMe);
          JNI_STATIC_FIELD(int, ChangeMeTo22, static_changeMeTo22);
)

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestInstanceCalls,
          JNI_CLASS_METHODE("callMe", callMe, jboolean);
)

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestInstanceFields,
          JNI_CLASS_FIELD(bool, changeMe)
)

JNI_CLASS(dev_wolveringer_EasyJNI_test_instance, BasicClass,
    JNI_CLASS_CONSTRUCTOR(constructor, string, string);
          JNI_CLASS_FIELD(string, name);
          JNI_CLASS_FIELD(string, value);
)

JNI_CLASS(dev_wolveringer_EasyJNI_test_instance, InformationHolder,
    JNI_CLASS_FIELD(JNI_CLASS_PTR(dev_wolveringer_EasyJNI_test_instance_BasicClass), basicInfo)
)

void testMethode(JNIEnv *env, jclass klass){
    auto iholder = dev_wolveringer_EasyJNI_test_instance_InformationHolder::AllocateNewInstance();
    iholder->constructor_default();

    auto belm = dev_wolveringer_EasyJNI_test_instance_BasicClass::AllocateNewInstance();
    belm->constructor("hello", "world");

    iholder->basicInfo.set(belm);

    auto out = iholder->basicInfo.get();
    printf("Info: %s, %s", out->name.get().c_str(), out->value.get().c_str());
}

extern "C" {
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    printf("Inizalissizing EasyJNITest libary!\n");
    auto env = EasyJNI::Utils::getJNIEnvAttach();
    if(!env){
        printf("Failed to load EasyJNI env attachment!\n");
        return 0;
    }

    jclass TestClass = env->FindClass("dev/wolveringer/EasyJNI/Main");

    JNINativeMethod methods[1] = {"runNativeTests", "()V", &testMethode};

    env->RegisterNatives(TestClass, (const JNINativeMethod*) methods, 1);

    printf("inizalisized EasyJNITest libary successfully!\n");
    return JNI_VERSION_1_6;
}
}