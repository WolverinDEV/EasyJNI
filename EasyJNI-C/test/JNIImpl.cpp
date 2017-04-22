#include <jni.h>
#include <iostream>
#include "../include/EasyJNI.h"

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
    JNI_EXCEPTION_CHECK;

    auto belm = dev_wolveringer_EasyJNI_test_instance_BasicClass::AllocateNewInstance();
    belm->constructor("hello", "world");

    iholder->basicInfo.set(belm);

    auto out = iholder->basicInfo.get();
    printf("Info: %s, %s for object: %s", out->name.get().c_str(), out->value.get().c_str(), out->toString().c_str());
}

JNI_CLASS(dev_wolveringer_EasyJNI, Main,
    JNI_STATIC_METHODE("runNativeTests", runNativeTests, void);
)

void runner(void*){
    EasyJNI::Utils::getJNIEnvAttach();
}

extern "C" {
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    printf("Inizalissizing EasyJNITest libary!\n");
    auto env = EasyJNI::Utils::getJNIEnvAttach();
    if(!env){
        printf("Failed to load EasyJNI env attachment!\n");
        return 0;
    }

    dev_wolveringer_EasyJNI_Main::runNativeTests()->bindNative((uintptr_t) &testMethode);

    printf("inizalisized EasyJNITest libary successfully!\n");

    pthread_t handle;
    pthread_create(&handle, nullptr, runner, nullptr);
    pthread_join(handle, nullptr);

    return JNI_VERSION_1_6;
}
}