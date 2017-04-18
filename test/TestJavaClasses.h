//
// Created by wolverindev on 12.04.17.
//

#include "../include/Converter.h"
#include "../include/utils/StringUtil.h"
#include "../include/class/JavaClassInfo.h"
#include "../include/class/JavaField.h"
#include "../include/class/JavaMethode.h"
#include <cstring>
#include <string>
#include <vector>

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

/*
 * Turn A into a string literal after macro-expanding it.
 */
#define STRINGIZE(A) STRINGIZE_NX(A)


#define STD_COUT_INIZALISIZER std::ios_base::Init mInitializer;

#define JNI_CLASS(package, name, contains) \
class package ##_## name : public JavaClass { \
    public: \
        static JavaClassInfo* ClassInfo; \
    private: \
        static JavaClassInfo* info(){ \
            if(ClassInfo) return ClassInfo; \
            ClassInfo = new JavaClassInfo(STRINGIZE(package), STRINGIZE(name)); \
            return ClassInfo; \
        } \
    public: \
        contains \
}; \
JavaClassInfo* package##_ ##name ::ClassInfo;


#define JNI_FIELD_PROTECTION public

#define JNI_STATIC_FIELD(fieldType, fieldName, fnName)                                                                                                \
public:                                                                                                                        \
    static JavaFieldInfoImpl<fieldType>* field_ ##fnName(){ \
         return info()->getFieldInfo<fieldType>(STRINGIZE(fieldName));  \
    }                                               \
private:                                                                                                                                      \
    __attribute__((constructor)) static void registerField_##fnName() {                                                                        \
         info()->registerField<fieldType>(STRINGIZE(fieldName), true);                       \
    }

#define JNI_CLASS_FIELD(fieldType, fieldName)                                                                                                \
JNI_FIELD_PROTECTION:                                                                                                                        \
    JavaField<fieldType> fieldName = JavaField<fieldType>(this, info()->getFieldInfo<fieldType>(STRINGIZE(fieldName)));                                               \
private:                                                                                                                                      \
    __attribute__((constructor)) static void registerField_##fieldName() {                                                                        \
         info()->registerField<fieldType>(STRINGIZE(fieldName), false);                       \
    }


#define JNI_STATIC_METHODE(name, fnName, retType, arguments...) \
    public: \
        static JavaMethodeInfoImpl<retType, ##arguments>*  fnName(){\
            return info()->getMethode<retType, ##arguments>(STRINGIZE(name)); \
        }; \
        __attribute__((constructor)) static void registerMethode_##fnName() { \
            info()->registerMethode<retType, ##arguments>(STRINGIZE(name), true);\
}\

#define JNI_CLASS_METHODE(name, rtype, args) rtype call_##name(args){ return nullptr; };


#define STATIC_VOID() \
  public:\
static void registerField_XXXX() {\
   std::cout << "Register field " << "X" << std::endl; \
}


/*
JNI_BEGIN_CLASS(dev_wolveringer, test)
    JNI_CLASS_FIELD(bool, test)
        __attribute__((constructor)) static void registerField_() {
        }
            //JNI_CLASS_METHODE(test, bool,);
JNI_END_CLASS()
 */

JNI_CLASS(dev_wolveringer, test,
          JNI_STATIC_METHODE(callMe, static_CallMe, void, jboolean, jint)
          JNI_CLASS_FIELD(jboolean, test);
          JNI_CLASS_FIELD(jboolean, test1);
)