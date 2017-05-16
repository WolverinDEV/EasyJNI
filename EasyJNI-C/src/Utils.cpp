//
// Created by wolverindev on 18.04.17.
//

#include "../include/Utils.h"
#include "../include/class/JavaClassInfo.h"
#include "../include/JavaClassDefine.h"
#include "../include/class/DefaultClassDefinitions.h"

using std::string;
using std::vector;

namespace EasyJNI {

    //JNIEnv* Utils::env = 0;
    JavaVM* Utils::javaVM = 0;
    pthread_key_t Utils::JNIEnvKey = 0;

    void init(JavaVM * vm, JNIEnv * env) {
        Utils::init(vm, env);
    }

    /**
     * JNIException
     */
    JNIException::JNIException(const std::string& message): message(message)
    {
        EasyJNI_debug("JNI Exception: %s", message.c_str());
    }

    JNIException::JNIException(JNIException &&handle) : message(string(handle.message)) { }

    const char* JNIException::what() const throw()
    {
        return message.c_str();
    }



    void Utils::KeyDetachCurrentThread(void *keyValuePtr) {
        if(!Utils::javaVM) return; //JVM destroyed

        Utils::javaVM->DetachCurrentThread();
    }

    /**
     * General env stuff
     */
    void Utils::init(JavaVM * vm, JNIEnv * currentEnv)
    {
        Utils::javaVM = vm;
        //Utils::env = jniEnv;
        pthread_key_create(&Utils::JNIEnvKey, Utils::KeyDetachCurrentThread);
        pthread_setspecific(Utils::JNIEnvKey, currentEnv);

        EasyJNI_debug("EasyJNI successfully inizalisized.\n");
        EasyJNI_debug("Currently registered classes (%d):\n", JavaClassInfo::getClassCount());
        auto loadedCls = JavaClassInfo::getLoadedClasses();
        for(auto it = loadedCls.begin(); it != loadedCls.end(); it++){
            EasyJNI_debug("   - %s\n", (*it)->getFullName().c_str());
        }
    }

    void Utils::uinit(JavaVM *vm) {
        Utils::javaVM = nullptr;
    }

    JNIEnv * Utils::getJNIEnvAttach()
    {
        if (javaVM) {
            JNIEnv* env = nullptr;
            if((env = (JNIEnv*) pthread_getspecific(Utils::JNIEnvKey)) == nullptr){
                int status = javaVM->AttachCurrentThread((void**) &env, NULL);
                if (status < 0) {
                    throw JNIException("Could not attach the JNI environment to the current thread.");
                }
                //printf("Attaching JVM\n");
                if(pthread_setspecific(Utils::JNIEnvKey, env) < 0) throw JNIException("Could not set the thread specific JNIEnv ptr.");
            }
            return env;
        }

        throw JNIException("Invalid JVM ptr.");
    }



    jstring Utils::toJString(const char * str)
    {
        return getJNIEnvAttach()->NewStringUTF(str);
    }

    jobjectArray Utils::toJObjectArray(const std::vector<std::string> & data)
    {
        auto env = getJNIEnvAttach();
        jclass classId = env->FindClass("java/lang/String");
        jint size = data.size();
        jobjectArray joa = env->NewObjectArray(size, classId, 0);

        for (int i = 0; i < size; i++)
        {
            jstring jstr = toJString(data[i]);
            env->SetObjectArrayElement(joa, i, jstr);

        }
        env->DeleteLocalRef(classId);

        JNI_EXCEPTION_CHECK
        return joa;
    }

    jbyteArray Utils::toJObjectArray(const std::vector<uint8_t> & data)
    {
        auto env = getJNIEnvAttach();
        jbyteArray jba = env->NewByteArray(data.size());
        env->SetByteArrayRegion(jba, 0, data.size(), (const jbyte*)&data[0]);
        JNI_EXCEPTION_CHECK
        return jba;
    }

    jobject Utils::toHashMap(const std::map<std::string, std::string> & data)
    {
        auto env = getJNIEnvAttach();
        jclass classId = env->FindClass("java/util/HashMap");
        jmethodID methodId = env->GetMethodID(classId, "<init>", "()V");
        jobject hashmap = env->NewObject(classId, methodId);

        methodId = env->GetMethodID(classId, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        for (auto & item : data)
        {
            jstring key = env->NewStringUTF(item.first.c_str());
            jstring value = env->NewStringUTF(item.second.c_str());
            env->CallObjectMethod(hashmap, methodId, key, value);

            env->DeleteLocalRef(key);
            env->DeleteLocalRef(value);
        }
        env->DeleteLocalRef(classId);
        JNI_EXCEPTION_CHECK
        return hashmap;
    }

    std::string Utils::toString(jstring str)
    {
        if (!str) {
            return std::string();
        }

        jboolean isCopy;
        auto env = getJNIEnvAttach();
        const char* chars = env->GetStringUTFChars(str, &isCopy);
        std::string s;
        if (chars) {
            s = chars;
            env->ReleaseStringUTFChars(str, chars);
        }
        JNI_EXCEPTION_CHECK
        return s;
    }

    std::vector<std::string> Utils::toVectorString(jobjectArray array)
    {
        std::vector<std::string> result;
        if (array) {
            auto env = getJNIEnvAttach();
            jint length = env->GetArrayLength(array);

            for (int i = 0; i < length; i++) {
                jobject valueJObject = env->GetObjectArrayElement(array, i);
                result.push_back(toString((jstring)valueJObject));
                env->DeleteLocalRef(valueJObject);
            }
        }
        JNI_EXCEPTION_CHECK
        return result;
    }

    std::vector<uint8_t> Utils::toVectorByte(jbyteArray array)
    {
        if (!array) {
            return std::vector<uint8_t>();
        }

        auto env = getJNIEnvAttach();
        jsize size = env->GetArrayLength(array);
        std::vector<uint8_t> result(size);
        env->GetByteArrayRegion(array, 0, size, (jbyte*)&result[0]);
        JNI_EXCEPTION_CHECK
        return result;
    }

    std::vector<float> Utils::toVectorFloat(jfloatArray array)
    {
        if (!array) {
            return std::vector<float>();
        }

        auto env = getJNIEnvAttach();
        jsize size = env->GetArrayLength(array);
        std::vector<float> result(size);
        env->GetFloatArrayRegion(array, 0, size, (jfloat*)&result[0]);
        JNI_EXCEPTION_CHECK
        return result;
    }

    std::vector<jobject> Utils::toVectorJObject(jobjectArray array)
    {
        std::vector<jobject> result;
        if (array) {

            auto env = getJNIEnvAttach();
            jint length = env->GetArrayLength(array);

            for (int i = 0; i < length; i++) {
                jobject valueJObject = env->GetObjectArrayElement(array, i);
                result.push_back(valueJObject);
            }
        }
        return result;
    }

    void Utils::checkException()
    {
        auto env = getJNIEnvAttach();
        if (env->ExceptionCheck())
        {
            jthrowable jthrowable = env->ExceptionOccurred();
            env->ExceptionDescribe();
            env->ExceptionClear();

            java_lang_Throwable throwable = java_lang_Throwable(java_lang_Throwable::ClassInfo, jthrowable);
            std::string message = throwable.getMessage();
            env->DeleteLocalRef(jthrowable);

            throw JNIException(message);
        }
    }

    namespace Helpers {
        jobject getJClassInstance(JavaClass* cls){
            return cls->getJavaInstance();
        }
        jclass getJClass(JavaClassInfo* info){
            return info->getJavaClass();
        }
        std::string getMethodeName(JavaMethodeInfo* methodeInfo){
            return methodeInfo->name;
        }
    }
}

extern "C"
{
    jint JNI_OnLoad(JavaVM* vm, void* reserved) {
        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            return -1;
        }
        // Initialize JNIUtils
        EasyJNI::init(vm, env);

        return JNI_VERSION_1_6;
    }

    void JNI_OnUnload(JavaVM *vm, void *reserved){
        EasyJNI::Utils::uinit(vm);
    }
}