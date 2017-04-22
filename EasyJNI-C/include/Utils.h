//
// Created by wolverindev on 18.04.17.
//

#pragma once

#include <jni.h>
#include <string>
#include <vector>
#include <map>

class JavaClass;
class JavaClassInfo;
class JavaMethodeInfo;

namespace EasyJNI {
    class JNIException: public std::exception
    {
        public:
            std::string message;
            JNIException(const std::string & message);
            JNIException(JNIException&& handle);
            virtual const char* what() const throw();
    };

    class Utils {
        private:
            //static JNIEnv * env;
            static JavaVM * javaVM;
            static pthread_key_t JNIEnvKey;

            static void KeyDetachCurrentThread(void* keyValuePtr);
        public:
            static void init(JavaVM * vm, JNIEnv * env);
            static void uinit(JavaVM *vm);

            //static inline JNIEnv * getJNIEnv() { return env;}
            static JNIEnv* getJNIEnvAttach();

            static jstring toJString(const char * str);
            inline static jstring toJString(const std::string & str) {
                return toJString(str.c_str());
            }
            static std::string toString(jstring str);

            static jobjectArray toJObjectArray(const std::vector<std::string> & data);
            static jbyteArray toJObjectArray(const std::vector<uint8_t> & data);
            static jobject toHashMap(const std::map<std::string, std::string> & data);

            static std::vector<std::string> toVectorString(jobjectArray array);
            static std::vector<uint8_t> toVectorByte(jbyteArray);
            static std::vector<float> toVectorFloat(jfloatArray);
            static std::vector<jobject> toVectorJObject(jobjectArray);

            static void checkException();
    };

    namespace Helpers {
        extern jobject getJClassInstance(JavaClass*);
        extern jclass getJClass(JavaClassInfo*);
        extern std::string getMethodeName(JavaMethodeInfo*);
    }
}

#define JNI_EXCEPTION_CHECK EasyJNI::Utils::checkException();

#include <stdarg.h>

#ifdef EasyJNI_ENABLE_DEBUG
    #define EasyJNI_debug(message, ...) printf((string("[EasyJNI] ") + message).c_str(), ##__VA_ARGS__);
    #define EasyJNI_debugClass(klass, message, ...) printf(("[EasyJNI] [%s] " + string(message)).c_str(), klass, ##__VA_ARGS__);
#else
    #define EasyJNI_debug(message, ...)
    #define EasyJNI_debugClass(klass, message, ...)
#endif