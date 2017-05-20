#include "../include/EasyJNI.h"
#include "jni.h"

//Class define

JNI_CLASS(java_util, List,
    JNI_CLASS_METHODE("add", addElement, bool, jobject)
    JNI_CLASS_METHODE("remove", removeElement, bool, jobject)
)

JNI_CLASS(dev_wolveringer_EasyJNI_test_speed, Main,
    JNI_STATIC_METHODE("setupTest", setupTestFn, void)
    JNI_STATIC_METHODE("runNativeTests", runNativeTestsFn, void)

    JNI_STATIC_METHODE("printStrings", printStrings, void)

    JNI_STATIC_FIELD(JNI_CLASS_PTR(java_util_List), strings, strings)
)

JNI_CLASS(dev_wolveringer_EasyJNI_test_speed, StringClass,
    JNI_CLASS_CONSTRUCTOR(valConstructor, string)

          JNI_CLASS_FIELD(jstring, value)
          JNI_CLASS_METHODE("setValue", setValue, void, string)
          JNI_CLASS_METHODE("getValue", getValue, string)
)

using MainClass = dev_wolveringer_EasyJNI_test_speed_Main;
using StringClass = dev_wolveringer_EasyJNI_test_speed_StringClass;

void setupTestFn(JNIEnv*, jclass){

}

void runNativeTestsFn(JNIEnv*, jclass){
    cout << "Test runnings!" << endl;
    auto strings = MainClass::strings()->get();

    for(int i = 0;i<1000*150;i++){
        auto strInstance = StringClass::AllocateNewInstance();
        strInstance->valConstructor("Hello world ("+to_string(i)+")");


        strings->addElement(strInstance->getJavaInstance());
    }

    //MainClass::printStrings()->callStatic();
    cout << "Test runned!" << endl;
}

extern "C" {
    jint JNI_OnLoad(JavaVM *vm, void *reserved) {
        cout << "Loading jni speedtest!" << endl;

        auto env = EasyJNI::Utils::getJNIEnvAttach();
        if(!env){
            printf("Failed to load EasyJNI env attachment!\n");
            return 0;
        }

        MainClass::setupTestFn()->bindNative(&setupTestFn);
        MainClass::runNativeTestsFn()->bindNative(&runNativeTestsFn);

        cout << "Loaded jni speedtest successfull!" << endl;

        return JNI_VERSION_1_6;
    }
}