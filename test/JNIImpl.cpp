//
// Created by wolverindev on 18.04.17.
//

#include <jni.h>
#include "../include/Utils.h"
#include "../include/class/JavaMethodeImpl.h"
#include "../include/class/JavaFieldImpl.h"
#include "TestJavaClasses.h"

JNI_CLASS(dev_wolveringer_easyjni, TestClass,
          JNI_STATIC_METHODE(callMe, static_CallMe, void)
          JNI_STATIC_METHODE(callMe, static_CallMeII, void, int, int)

          JNI_STATIC_FIELD(bool, changeMe, field_changeMe)
)

void testMethode(JNIEnv *env, jclass klass){
    printf("Methode called!\n");

    jclass tklass = dev_wolveringer_easyjni_TestClass::ClassInfo->getJavaClass();
    printf("Methode %s java id %d\n", dev_wolveringer_easyjni_TestClass::static_CallMe()->name.c_str(), dev_wolveringer_easyjni_TestClass::static_CallMe()->getMethodeId());

    dev_wolveringer_easyjni_TestClass::static_CallMe()->callStatic();
    printf("Flag: %d\n", dev_wolveringer_easyjni_TestClass::field_field_changeMe()->get());
    dev_wolveringer_easyjni_TestClass::field_field_changeMe()->set(!dev_wolveringer_easyjni_TestClass::field_field_changeMe()->get());

    dev_wolveringer_easyjni_TestClass::static_CallMeII()->operator()(2, 3);

}

extern "C" {
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    printf("Inizalissizing EasyJNITest libary!\n");
    auto env = EasyJNI::Utils::getJNIEnvAttach();
    if(!env){
        printf("Failed to load EasyJNI env attachment!\n");
    }

    jclass TestClass = env->FindClass("dev/wolveringer/easyjni/Main");

    JNINativeMethod methods[1] = {"callTests", "()V", &testMethode};

    env->RegisterNatives(TestClass, (const JNINativeMethod*) methods, 1);

    printf("inizalisized EasyJNITest libary successfully!\n");
    return JNI_VERSION_1_6;
}
}