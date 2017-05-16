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
    private:
        static std::vector<JavaClassInfo*> loadedClasses;

    public:
        static int getClassCount(){
            return loadedClasses.size();
        }

        static std::vector<JavaClassInfo*> getLoadedClasses(){
            return loadedClasses;
        }

    public:
        JavaClassInfo(const char *package, const char *name);

        const std::string package;
        const std::string name;

        std::string getFullName(){
            return package + "." + name;
        }

        std::vector<JavaMethodeInfo*> getMethods(){
            return methods;
        }

        std::vector<JavaFieldInfo*> getFields(){
            return fields;
        }

        jclass javaClass = nullptr;

        jclass getJavaClass(){
            if(javaClass) return javaClass;
            auto env = EasyJNI::Utils::getJNIEnvAttach();
            javaClass = (jclass) env->NewGlobalRef(env->FindClass((StringUtils::replaceAll(package, ".", "/") + "/" + name).c_str()));
            if(javaClass) return javaClass;

            throw EasyJNI::JNIException("Cant find class "+ getFullName() +" ("+StringUtils::replaceAll(package, ".", "/") + "/" + name+")");
        }

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

            EasyJNI_debugClass(getFullName().c_str(), "Register %sfield %s with the signature %s\n", _static ? "static " : "", name.c_str(), out->getSignature().c_str());
            return out;
        }

        template <typename T, typename... Args>
        JavaMethodeInfoImpl<T, Args...>* registerMethode(std::string name, bool _static){
            JavaMethodeInfoImpl<T, Args...>* info = new JavaMethodeInfoImpl<T, Args...>(this, name, _static);
            methods.push_back(info);
            EasyJNI_debugClass(getFullName().c_str(), "Register %smethode %s with the siganture %s\n", _static ? "static " : "", name.c_str(), info->getSignature().c_str());
            return info;
        };

        template <typename T, typename... Args>
        JavaMethodeInfoImpl<T, Args...>* getMethode(std::string name){
            for(auto it = methods.begin(); it != methods.end(); it++)
                if(dynamic_cast<JavaMethodeInfoImpl<T, Args...>*>(*it) && EasyJNI::Helpers::getMethodeName(*it).compare(name) == 0)
                    return dynamic_cast<JavaMethodeInfoImpl<T, Args...>*>(*it);
            return nullptr;
        }

    private:
        std::vector<JavaFieldInfo*> fields;
        std::vector<JavaMethodeInfo*> methods;
};

class JavaClass {
    public:
        JavaClassInfo* javaInfo = nullptr;
        jobject javaInstance;

        jobject getJavaInstance() {
            if(!javaInstance) throw EasyJNI::JNIException("Cant get class instance");
            return javaInstance;
        }

        JavaClass(JavaClassInfo* jiInfo, jobject obj) {
            this->javaInfo= jiInfo;
            this->javaInstance = obj;
            //cout << "Loading cls instance!" << endl;
        }

        ~JavaClass(){
            if(javaInstance){
                //cout << "Deleting cls instance!" << endl;
                EasyJNI::Utils::getJNIEnvAttach()->DeleteWeakGlobalRef(javaInstance);
            }
        }
};