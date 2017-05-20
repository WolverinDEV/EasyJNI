//
// Created by wolverindev on 12.04.17.
//

#include "TypeConverter.h"
#include "utils/StringUtil.h"
#include "class/JavaClassInfo.h"
#include "class/JavaField.h"
#include "class/JavaFieldImpl.h"
#include "class/JavaMethode.h"
#include "class/JavaMethodeImpl.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)

#define JNI_CLASS_PTR(clsName) shared_ptr<clsName>


#define JNI_CLASS(package, name, contains)                                                                                                                             \
class package ##_## name : public JavaClass {                                                                                                                          \
    public:                                                                                                                                                            \
        static JavaClassInfo* ClassInfo;                                                                                                                               \
    private:                                                                                                                                                           \
        static JavaClassInfo* info(){                                                                                                                                  \
            if(ClassInfo) return ClassInfo;                                                                                                                            \
            ClassInfo = new JavaClassInfo(STRINGIZE(package), STRINGIZE(name));                                                                                        \
            return ClassInfo;                                                                                                                                          \
        }                                                                                                                                                              \
    public:                                                                                                                                                            \
        JNI_JNI_CONSTRUCTOR(package ##_## name)                                                                                                                        \
        JNI_CLASS_METHODE("toString", toString, std::string);                                                                                                          \
        contains                                                                                                                                                       \
};                                                                                                                                                                     \
JavaClassInfo* package##_ ##name ::ClassInfo;                                                                                                                          \
template <>                                                                                                                                                            \
struct JNIToCPPConversor<JNI_CLASS_PTR(package ##_## name)> {                                                                                                          \
    inline static JNI_CLASS_PTR(package ##_## name) convert(jobject obj){                                                                                              \
        package ##_## name * ptrInstance = new package ##_## name(package##_ ##name ::ClassInfo, EasyJNI::Utils::getJNIEnvAttach()->NewWeakGlobalRef(obj));            \
        return JNI_CLASS_PTR(package ##_## name)(std::move(ptrInstance));                                                                                              \
    }                                                                                                                                                                  \
};                                                                                                                                                                     \
template<>                                                                                                                                                             \
struct CPPToJNIConversor<JNI_CLASS_PTR(package ##_## name)> {                                                                                                          \
    using JNITypeName = CompileTimeString<'L', CLASS_STRING_TO_CHARS_DEFAULT(STRINGIZE(package ##_## name))>;                                                          \
    using JNIType = jobject;                                                                                                                                           \
    static jobject convert(JNI_CLASS_PTR(package ##_## name)& obj) {                                                                                                   \
        return obj->getJavaInstance();                                                                                                                                 \
    };                                                                                                                                                                 \
                                                                                                                                                                       \
    static jobject convert(nullptr_t obj) { return nullptr; }                                                                                                          \
}; \
//CLASS_PCONV(package ##_## name)



#define JNI_STATIC_FIELD(fieldType, fieldName, fnName)                                                                                                                 \
public:                                                                                                                                                                \
    static JavaFieldInfoImpl<fieldType>* fnName(){                                                                                                                     \
         return info()->getFieldInfo<fieldType>(STRINGIZE(fieldName));                                                                                                 \
    }                                                                                                                                                                  \
                                                                                                                                                                       \
private:                                                                                                                                                               \
    __attribute__((constructor)) static void registerField_##fnName() {                                                                                                \
         info()->registerField<fieldType>(STRINGIZE(fieldName), true);                                                                                                 \
    }


#define JNI_STATIC_METHODE(name, fnName, retType, arguments...)                                                                                                        \
    public:                                                                                                                                                            \
        static JavaMethodeInfoImpl<retType, ##arguments>*  fnName(){                                                                                                   \
            return info()->getMethode<retType, ##arguments>(name);                                                                                                     \
        };                                                                                                                                                             \
    private:                                                                                                                                                           \
        __attribute__((constructor)) static void registerMethode_##fnName() {                                                                                          \
            info()->registerMethode<retType, ##arguments>(name, true);                                                                                                 \
        }



#define JNI_JNI_CONSTRUCTOR(clsName)                                                                                                                                   \
    public:                                                                                                                                                            \
        clsName(JavaClassInfo* iKlass, jobject obj) : JavaClass(iKlass, obj) {                                                                                         \
        }                                                                                                                                                              \
                                                                                                                                                                       \
        JNI_JNI_INSTANCE_ALLOCATOR(clsName)                                                                                                                            \
        JNI_JNI_INSTANCE_CONSTRUCTOR(clsName)                                                                                                                          \
        JNI_CLASS_CONSTRUCTOR(constructor_default)



#define JNI_JNI_INSTANCE_ALLOCATOR(clsName)                                                                                                                            \
        static JNI_CLASS_PTR(clsName) AllocateNewInstance() {                                                                                                          \
            auto env = EasyJNI::Utils::getJNIEnvAttach();                                                                                                              \
            auto clInstance = env->AllocObject(clsName::ClassInfo->getJavaClass());                                                                                    \
            clInstance = env->NewGlobalRef(clInstance);                                                                                                            \
            clsName* ptrInstance = new clsName(clsName::ClassInfo, clInstance);                                                                                        \
            return JNI_CLASS_PTR(clsName)(std::move(ptrInstance), [](clsName* ptr){ delete ptr; });                                                                    \
        }

#define JNI_JNI_INSTANCE_CONSTRUCTOR(clsName)                                                                                                                          \
        static JNI_CLASS_PTR(clsName) NewInstance(jobject clInstance, bool makeGlobal = true) {                                                                          \
            if(makeGlobal) clInstance = EasyJNI::Utils::getJNIEnvAttach()->NewGlobalRef(clInstance);                                                                 \
            clsName* ptrInstance = new clsName(clsName::ClassInfo, clInstance);                                                                                        \
            return JNI_CLASS_PTR(clsName)(std::move(ptrInstance), [](clsName* ptr){ delete ptr; });                                                                    \
        }

#define JNI_CLASS_METHODE(name, fnName, retType, arguments...)                                                                                                         \
    public:                                                                                                                                                            \
        JavaMethodeImpl<retType, ##arguments> fnName = JavaMethodeImpl<retType, ##arguments>(this, info()->getMethode<retType, ##arguments>(name));                    \
    private:                                                                                                                                                           \
        __attribute__((constructor)) static void registerClassMethode_##fnName() {                                                                                     \
            info()->registerMethode<retType, ##arguments>(name, false);                                                                                                \
        }


#define JNI_CLASS_CONSTRUCTOR(conName, parameter...)                                                                                                                   \
    public:                                                                                                                                                            \
        JNI_CLASS_METHODE("<init>", conName, void, ##parameter);                                                                                                       \



#define JNI_CLASS_FIELD(fieldType, fieldName)                                                                                                                          \
public:                                                                                                                                                                \
    JavaFieldImpl<fieldType> fieldName = JavaFieldImpl<fieldType>(this, info()->getFieldInfo<fieldType>(STRINGIZE(fieldName)));                                        \
private:                                                                                                                                                               \
    __attribute__((constructor)) static void registerField_##fieldName() {                                                                                             \
         info()->registerField<fieldType>(STRINGIZE(fieldName), false);                                                                                                \
    }                                                                                                                                                                  \
