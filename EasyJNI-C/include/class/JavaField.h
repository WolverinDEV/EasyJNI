//
// Created by wolverindev on 18.04.17.
//

#pragma once

#include <iostream>
#include "jni.h"
#include "../TypeConverter.h"
#include "../Utils.h"

class JavaClassInfo;
class JavaClass;

struct JavaFieldInfo {
    public:
        JavaClassInfo* klass;
        JavaClassInfo* getClassInfo(){
            return klass;
        }

        std::string name;
        bool _static;

        jfieldID fieldId = nullptr;

        jfieldID getFieldId(){
            if(fieldId) return fieldId;
            if(_static){
                fieldId = EasyJNI::Utils::getJNIEnvAttach()->GetStaticFieldID(EasyJNI::Helpers::getJClass(klass), name.c_str(), getSignature().c_str());
            } else {
                fieldId = EasyJNI::Utils::getJNIEnvAttach()->GetFieldID(EasyJNI::Helpers::getJClass(klass), name.c_str(), getSignature().c_str());
            }
            if(fieldId) return fieldId;
            throw EasyJNI::JNIException("Cant find field "+name+" with signature "+getSignature());
        }

        virtual std::string getSignature() = 0;
};

template <typename T>
class JavaFieldImpl;

class JavaField;

template <typename T>
class JavaFieldInfoImpl;

template <typename T>
struct JavaFieldActor {
    static T getStatic(JavaFieldInfo* field);
    static bool setStatic(JavaFieldInfo* field,T val);

    static T getInstance(JavaField* field);
    static bool setInstance(JavaField* field,T val);
};

template <typename T>
struct JavaFieldInfoImpl : public JavaFieldInfo {
    JavaFieldInfoImpl(JavaClassInfo* klass, const std::string &name, bool _static) {
        this->klass = klass;
        this->name = name;
        this->_static = _static;
    }

    std::string getSignature(){
        return Concatenate<typename CPPToJNIConversor<T>::JNITypeName , CompileTimeString<'\0'>>::Result::value();
    }

    T get(){
        auto out = JavaFieldActor<decltype(CPPToJNIConversor<T>::convert(nullptr))>
        ::getStatic((JavaFieldInfoImpl<decltype(CPPToJNIConversor<T>::convert(nullptr))>*) this);

        auto des = JNIParamDestructor<1>(EasyJNI::Utils::getJNIEnvAttach()); //Delete lcoal ref outside of this fn
        des.add(out);

        return JNIToCPPConversor<T>::convert(out);
    }

    bool set(T obj){
        decltype(CPPToJNIConversor<T>::convert(obj)) nobj = CPPToJNIConversor<T>::convert(obj);
        return JavaFieldActor<decltype(nobj)>::setStatic((JavaFieldInfo*) this, nobj);
    }

    bool operator=(T obj){
        return set(obj);
    }
};

class JavaField {
    public:
        //template <typename T>
        //virtual JavaFieldInfoImpl<T>* getFieldInfo() = 0;

        virtual JavaFieldInfo* getFieldInfo() = 0;

        virtual JavaClass* getClassInstance() = 0;
};

template<typename T>
class JavaFieldImpl : public JavaField {
    public:
        JavaFieldInfoImpl<T> *info;
        JavaFieldInfoImpl<T>* getFieldInfo(){
            return info;
        }

        JavaClass *klassInstance;

        JavaClass* getClassInstance(){ //TODO check non null?
            return klassInstance;
        }
    public:
        JavaFieldImpl(JavaClass *klassInstance, JavaFieldInfoImpl<T>* finfo) {
            this->klassInstance = klassInstance;
            this->info = finfo;
        }

        T get() {
            auto res = JavaFieldActor<decltype(CPPToJNIConversor<T>::convert(nullptr))>::getInstance(this);

            auto des = JNIParamDestructor<1>(EasyJNI::Utils::getJNIEnvAttach()); //Delete lcoal ref outside of this fn
            des.add(res);

            return JNIToCPPConversor<T>::convert(res);
        }

        bool set(T obj){
            decltype(CPPToJNIConversor<T>::convert(obj)) nobj = CPPToJNIConversor<T>::convert(obj);
            return JavaFieldActor<decltype(nobj)>::setInstance(this, nobj); //TODO fix cast
        }

        bool operator=(T obj){
            return set(obj);
        }
};

#define JFT(type, setStaticFn, getStaticFn, setInstanceFn, getInstanceFn) \
template <> \
struct JavaFieldActor<type> { \
    static type getStatic(JavaFieldInfo* field){ \
        getStaticFn \
    } \
    static bool setStatic(JavaFieldInfo* field,type val){ \
        setStaticFn \
    } \
 \
    static type getInstance(JavaField* field){ \
        getInstanceFn \
    } \
    static bool setInstance(JavaField* field,type val){ \
        setInstanceFn \
    } \
};