//
// Created by wolverindev on 18.04.17.
//

#pragma once

#include "../Utils.h"
#include "../utils/StringUtil.h"
#include "JavaField.h"
#include <iostream>
#include <vector>
#include <jni.h>

class JavaFieldInfo;
template <typename T>
class JavaFieldInfoImpl;

class JavaMethodeInfo;

template <typename T, typename... Args>
class JavaMethodeInfoImpl;

class JavaClassInfo {
    public:
        JavaClassInfo(const char *package, const char *name);

        const std::string package;
        const std::string name;

        std::vector<JavaFieldInfo*> fields;
        std::vector<JavaMethodeInfo*> methods;

        jclass javaClass = nullptr;

        jclass getJavaClass(){
            if(javaClass) return javaClass;
            auto env = EasyJNI::Utils::getJNIEnvAttach();
            javaClass = env->FindClass((StringUtils::replaceAll(package, ".", "/") + "/" + name).c_str());
            if(javaClass) return javaClass;

            throw EasyJNI::JNIException("Cant find class "+package+"."+name+" ("+StringUtils::replaceAll(package, ".", "/") + "/" + name+")");
        }
    public:
        std::vector<JavaFieldInfo*> getFields();

        template <typename T>
        JavaFieldInfoImpl<T>* getFieldInfo(std::string name){
            for(auto it = fields.begin(); it != fields.end(); it++)
                if((*it)->name.compare(name) == 0)
                    if(!dynamic_cast<JavaFieldInfoImpl<T>*>(*it))
                        throw EasyJNI::JNIException("Invalid field type '"+string(Concatenate<typename CPPToJNIConversor<T>::JNITypeName , CompileTimeString<'\0'>>::Result::value())+"' cant be cast to '"+(*it)->getSignature()+"'");
                    else
                        return dynamic_cast<JavaFieldInfoImpl<T>*>(*it);
            return nullptr;
        }

        template <typename T>
        JavaFieldInfoImpl<T>* registerField(std::string name, bool _static){
            auto out = new JavaFieldInfoImpl<T>(this, name, _static);
            fields.push_back(out);
            printf("Registering static field %s %s", out->name.c_str(), out->getSignature().c_str());
            return out;
        }

        template <typename T, typename... Args>
        JavaMethodeInfoImpl<T, Args...>* registerMethode(std::string name, bool _static){
            JavaMethodeInfoImpl<T, Args...>* info = new JavaMethodeInfoImpl<T, Args...>(this, name, _static);
            methods.push_back(info);
            return info;
        };

        template <typename T, typename... Args>
        JavaMethodeInfoImpl<T, Args...>* getMethode(std::string name){
            for(auto it = methods.begin(); it != methods.end(); it++)
                if(dynamic_cast<JavaMethodeInfoImpl<T, Args...>*>(*it))
                    return dynamic_cast<JavaMethodeInfoImpl<T, Args...>*>(*it);
            return nullptr;
        }
};

class JavaClass {
    public:
        jobject javaInstance;

        jobject getJavaInstance(){
            return javaInstance;
        }
};