#include <jni.h>
#include "../include/Utils.h"
#include "../include/class/JavaMethodeImpl.h"
#include "../include/class/JavaFieldImpl.h"
#include "TestJavaClasses.h"

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestStaticCalls,
          JNI_STATIC_METHODE(callMe, static_CallMe, void)
          JNI_STATIC_METHODE(callMe, static_CallMe2, void, int, int)
          JNI_STATIC_METHODE(callMe, static_CallMeSqr, int, int)

          //JNI_STATIC_FIELD(bool, changeMe, field_changeMe)
)

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestStaticFields,
          JNI_STATIC_FIELD(bool, ChangeMe, static_changeMe);
          JNI_STATIC_FIELD(int, ChangeMeTo22, static_changeMeTo22);

)

JNI_CLASS(dev_wolveringer_EasyJNI_test, TestInstanceCalls,
          JNI_CLASS_METHODE(callMe, callMe, jboolean);
)

void testMethode(JNIEnv *env, jclass klass){
    printf("Methode called!\n");

    /*
        dev_wolveringer_EasyJNI_test_TestStaticCalls::static_CallMe()->callStatic();
        dev_wolveringer_EasyJNI_test_TestStaticCalls::static_CallMe2()->callStatic(2, 3);
        printf("SQR of 2 %d", dev_wolveringer_EasyJNI_test_TestStaticCalls::static_CallMeSqr()->callStatic(2));
    */

    dev_wolveringer_EasyJNI_test_TestStaticFields::static_changeMe()->set(!dev_wolveringer_EasyJNI_test_TestStaticFields::static_changeMe()->get());
    dev_wolveringer_EasyJNI_test_TestStaticFields::static_changeMeTo22()->set(22);

    auto clInstance = env->AllocObject(dev_wolveringer_EasyJNI_test_TestInstanceCalls::ClassInfo->getJavaClass());
    clInstance = env->NewWeakGlobalRef(clInstance);

    dev_wolveringer_EasyJNI_test_TestInstanceCalls instance(dev_wolveringer_EasyJNI_test_TestInstanceCalls::ClassInfo, clInstance);
    printf("Name: %s", instance.callMe.info->name.c_str());
    instance.callMe();
}

extern "C" {
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    printf("Inizalissizing EasyJNITest libary!\n");
    auto env = EasyJNI::Utils::getJNIEnvAttach();
    if(!env){
        printf("Failed to load EasyJNI env attachment!\n");
    }

    jclass TestClass = env->FindClass("dev/wolveringer/EasyJNI/Main");

    JNINativeMethod methods[1] = {"runNativeTests", "()V", &testMethode};

    env->RegisterNatives(TestClass, (const JNINativeMethod*) methods, 1);

    printf("inizalisized EasyJNITest libary successfully!\n");
    return JNI_VERSION_1_6;
}
}