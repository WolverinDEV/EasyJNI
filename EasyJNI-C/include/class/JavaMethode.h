//
// Created by wolverindev on 18.04.17.
//

#pragma once

#include <type_traits>
#include <jni.h>
#include "JavaClassInfo.h"
#include "../Converter.h"
#include "../Utils.h"

#define JMIC(type, _static, _instance)                                                                       \
template <typename... Args>                                                                                 \
struct JNIMethodeInvoker<type, Args...> {                                                                   \
    inline static type callStatic(JavaMethodeInfo* methode, Args... args){                                         \
       _static                                                                                              \
    }                                                                                                       \
                                                                                                            \
    static type callInstance(JavaMethode* methode, Args... args) {         \
        _instance                                                                                           \
    }                                                                                                       \
};                                                                                                          \

class JavaMethodeInfo;

template <typename ReturnType, typename ...Args>
class JavaMethodeImpl;
class JavaMethode;

template <typename T, typename... Args>
struct JNIMethodeInvoker {
    inline static T callStatic(JavaMethodeInfo* methode, Args... args);
    static T callInstance(JavaMethode* methode, jobject instance, Args... args);
};

class JavaMethodeInfo {
    public:
        JavaClassInfo* klass;
        bool _static;

        JavaClassInfo* getJavaClass(){
            if(klass) return klass;
            throw EasyJNI::JNIException("Cant ind root class for the methode "+name+" with signature "+getSignature());
        }

        std::string name;
        jmethodID id = nullptr;

        virtual std::string getSignature() = 0;

        jmethodID getMethodeId(){
            if(id) return id;
            auto env = EasyJNI::Utils::getJNIEnvAttach();
            if(_static){
                printf("Static: %s - %s\n", name.c_str(), getSignature().c_str());
                id = env->GetStaticMethodID(klass->getJavaClass(), name.c_str(), getSignature().c_str());
            } else {
                id = env->GetMethodID(klass->getJavaClass(), name.c_str(), getSignature().c_str());
            }
            if(id) return id;
            throw EasyJNI::JNIException("Cant find "+string(_static ? "static" : "instance")+" methode "+name+" with signature "+getSignature()+" for class "+klass->package+"."+klass->name);
        }
};

template <typename ReturnType, typename... Args>
class JavaMethodeInfoImpl : public JavaMethodeInfo {
    public:
        JavaMethodeInfoImpl(JavaClassInfo* klass, std::string name, bool _static){
            this->klass = klass;
            this->name = name;
            this->_static = _static;
        }

        std::string getSignature(){
            return Concatenate<CompileTimeString<'('>, //left parenthesis
                    typename CPPToJNIConversor<Args>::JNITypeName..., //params signature
                    CompileTimeString<')'>, //right parenthesis
                    typename CPPToJNIConversor<ReturnType>::JNITypeName ,
                    CompileTimeString<'\0'>> //return type signature
            ::Result::value();
        }

        ReturnType callStatic(Args...args){
            static constexpr uint8_t nargs = sizeof...(Args);
            auto env = EasyJNI::Utils::getJNIEnvAttach();
            JNIParamDestructor<nargs> paramDestructor(env);

            //return JNIToCPPConversor<ReturnType>::convert(JNIMethodeInvoker<decltype(CPPToJNIConversor<ReturnType>::convert((ReturnType) nullptr)), decltype(CPPToJNIConversor<Args>::convert(args))...>::callStatic(this, JNIParamConversor<Args>(args, paramDestructor)...));
            return JNIMethodeInvoker<ReturnType, decltype(CPPToJNIConversor<Args>::convert(args))...>::callStatic(this, JNIParamConversor<Args>(args, paramDestructor)...); //TODO warp back return type
        }

        ReturnType operator()(Args...args){
            return callStatic(args...);
        }
};

/*
template <typename T = void, typename... Args>
struct JavaMethodeInfoImpl {
    public:
        void callStatic(Args...args){
            static constexpr uint8_t nargs = sizeof...(Args);
            auto env = EasyJNI::Utils::getJNIEnvAttach();
            JNIParamDestructor<nargs> paramDestructor(env);
            JNIMethodeInvoker<void, decltype(CPPToJNIConversor<Args>::convert(args))...>::callStatic(this, JNIParamConversor<Args>(args, paramDestructor)...);
        }

};
*/

struct JavaMethode {
    public:
        virtual JavaClass* getJavaClassInstance() = 0;

        //template <typename ReturnType, typename ...Args>
        //virtual JavaMethodeInfoImpl<ReturnType, Args...>* getMethodeInfo() = 0;

        virtual JavaMethodeInfo* getMethodeInfo() = 0;
};

template <typename ReturnType, typename ...Args>
struct JavaMethodeImpl : public JavaMethode {
    public:
        JavaClass* klassInstance;

        JavaClass* getJavaClassInstance(){
            return klassInstance;
        }

        JavaMethodeInfoImpl<ReturnType, Args...>* info;

        JavaMethodeInfoImpl<ReturnType, Args...>* getMethodeInfo(){
            return info;
        };

        JavaMethodeImpl(JavaClass* klassInstance, JavaMethodeInfoImpl<ReturnType, Args...>* mInfo){
            this->info = mInfo;
            this->klassInstance = klassInstance;
        }


        ReturnType call(Args... args){
            static constexpr uint8_t nargs = sizeof...(Args);
            auto env = EasyJNI::Utils::getJNIEnvAttach();
            JNIParamDestructor<nargs> paramDestructor(env);
            //return JNIToCPPConversor<ReturnType>::convert(JNIMethodeInvoker<decltype(CPPToJNIConversor<ReturnType>::convert((ReturnType) nullptr)), decltype(CPPToJNIConversor<Args>::convert(args))...>::callInstance(this, JNIParamConversor<Args>(args, paramDestructor)...));
            return JNIMethodeInvoker<ReturnType, decltype(CPPToJNIConversor<Args>::convert(args))...>::callInstance(this, JNIParamConversor<Args>(args, paramDestructor)...); //TODO warp back return type
        }

        ReturnType operator()(Args...args){
            return call(args...);
        }
};
