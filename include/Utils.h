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
namespace EasyJNI {
    class JNIException: public std::exception
    {
        public:
            std::string message;
            JNIException(const std::string & message);
            virtual const char* what() const throw();
    };

    class Utils {
        private:
            static JNIEnv * env;
            static JavaVM * javaVM;
        public:
            static void init(JavaVM * vm, JNIEnv * env);
            static inline JNIEnv * getJNIEnv() { return env;}
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

            //static SPJNIMethodInfo getStaticMethodInfo(const std::string& className, const std::string& methodName, const char * signature);
            //static SPJNIMethodInfo getMethodInfo(const std::string& className, const std::string& methodName, const char * signature);
            static void checkException();
    };

    namespace Helpers {
        extern jobject getJClassInstance(JavaClass*);
        extern jclass getJClass(JavaClassInfo*);
    }
}

#define JNI_EXCEPTION_CHECK EasyJNI::Utils::checkException();