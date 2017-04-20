//
// Created by wolverindev on 18.04.17.
//

#pragma once

#include <iostream>
#include "jni.h"
#include "../Converter.h"
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

class JavaClassInfo;
template <typename T>
class JavaField;
template <typename T>
class JavaFieldInfoImpl;

template <typename T>
struct JavaFieldActor {
    static T getStatic(JavaFieldInfoImpl<T>* field);
    static bool setStatic(JavaFieldInfoImpl<T>* field,T val);

    static T getInstance(JavaField<T>* field);
    static bool setInstance(JavaField<T>* field,T val);
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
        return JNIToCPPConversor<T>::convert(JavaFieldActor<decltype(CPPToJNIConversor<T>::convert(nullptr))>::getStatic(this));
    }

    bool set(T obj){
        decltype(CPPToJNIConversor<T>::convert(obj)) nobj = CPPToJNIConversor<T>::convert(obj);
        return JavaFieldActor<decltype(nobj)>::setStatic(this, nobj);
    }

    bool operator=(T obj){
        return set(obj);
    }
};

template<typename T>
class JavaField {
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
        JavaField(JavaClass *klassInstance, JavaFieldInfoImpl<T>* finfo) {
            this->klassInstance = klassInstance;
            this->info = finfo;
            std::cout << "Init field! " << info->name << std::endl;
        }

        T get() {
            return JNIToCPPConversor<T>::convert(JavaFieldActor<decltype(CPPToJNIConversor<T>::convert(nullptr))>::getInstance(this));
        }

        bool set(T obj){
            decltype(CPPToJNIConversor<T>::convert(obj)) nobj = CPPToJNIConversor<T>::convert(obj);
            return JavaFieldActor<decltype(nobj)>::setInstance(this, nobj);
        }

        bool operator=(T obj){
            return set(obj);
        }
};

#define JFT(type, setStaticFn, getStaticFn, setInstanceFn, getInstanceFn) \
template <> \
struct JavaFieldActor<type> { \
    static type getStatic(JavaFieldInfoImpl<type>* field){ \
        getStaticFn \
    } \
    static bool setStatic(JavaFieldInfoImpl<type>* field,type val){ \
        setStaticFn \
    } \
 \
    static type getInstance(JavaField<type>* field){ \
        getInstanceFn \
    } \
    static bool setInstance(JavaField<type>* field,type val){ \
        setInstanceFn \
    } \
};