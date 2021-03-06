//
// Created by wolverindev on 16.04.17.
//

#pragma once

#include <istream>
#include "jni.h"
#include "Utils.h"

#pragma mark Compile Time Strings

//Compile Time String
template<char... Cs>
struct CompileTimeString {
    static const char *value() {
        static const char a[sizeof...(Cs)] = {Cs...};
        return a;
    }
};

//Concatenate 2 Strings
template<class L, class R>
struct Concatenate2;

template<char... LC, char... RC>
struct Concatenate2<CompileTimeString<LC...>, CompileTimeString<RC...>> {
    using Result = CompileTimeString<LC..., RC...>;
};

//Concatenate N strings
template<typename ...C>
struct Concatenate;

template<typename C1>
struct Concatenate<C1> {
    using Result = C1;
};

template<typename C1, typename C2>
struct Concatenate<C1, C2> {
    using Result = typename Concatenate2<C1, C2>::Result;
};

template<typename C1, typename ...C>
struct Concatenate<C1, C...> {
    using Result = typename Concatenate2<C1, typename Concatenate<C...>::Result>::Result;
};


#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

template<unsigned int N>
constexpr char get_ch(char const (&s)[N], unsigned int i) {
    return i >= N ? '\0' : (s[i] == '_' ? '/' : (s[i] == '\0' ? ';' : s[i]));
}

#define DEFAULT_CLASS_STR_LENGTH 100

#define CLASS_STRING_TO_CHARS_EXTRACT(z, n, data) \
        BOOST_PP_COMMA_IF(n) get_ch(data, n)

#define CLASS_STRING_TO_CHARS(STRLEN, STR)  \
        BOOST_PP_REPEAT(STRLEN, CLASS_STRING_TO_CHARS_EXTRACT, STR)

#define CLASS_STRING_TO_CHARS_DEFAULT(STR)  \
        CLASS_STRING_TO_CHARS(DEFAULT_CLASS_STR_LENGTH, STR)

#pragma mark C++ To JNI conversion templates

//default template
template<typename T>
struct CPPToJNIConversor {
    using JNIType = T;

    inline static T convert(T obj);
};

/*
//generic pointer implementation (using jlong type)
template<typename T>
struct CPPToJNIConversor<T *> {
    using JNIType = CompileTimeString<'J'>;

    inline static jlong convert(T *obj) { return reinterpret_cast<jlong>(obj); }
};

//object implementations
template<>
struct CPPToJNIConversor<std::string> {
    using JNIType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;

    inline static jstring convert(const std::string &obj) { return Utils::toJString(obj); }
};

template<>
struct CPPToJNIConversor<const char *> {
    using JNIType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;

    inline static jstring convert(const char *obj) { return Utils::toJString(obj); }
};

template<>
struct CPPToJNIConversor<std::vector < std::string>> {
using JNIType = CompileTimeString<'[', 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;

inline static jobjectArray convert(const std::vector <std::string> &obj) { return Utils::toJObjectArray(obj); }

};

template<>
struct CPPToJNIConversor<std::vector < uint8_t>> {
using JNIType = CompileTimeString<'[', 'B'>;

inline static jbyteArray convert(const std::vector <uint8_t> &obj) { return Utils::toJObjectArray(obj); }

};

template<>
struct CPPToJNIConversor<jobject> {
    using JNIType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', ';'>;

    inline static jobject convert(jobject obj) { return obj; }
};

template<>
struct CPPToJNIConversor<JNIObjectPtr> {
    using JNIType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', ';'>;

    inline static jobject convert(const JNIObjectPtr &obj) { return obj->instance; }
};

template<>
struct CPPToJNIConversor<std::vector < jobject>> {
using JNIType = CompileTimeString<'[', 'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', ';'>;

inline static jbyteArray convert(const std::vector <uint8_t> &obj) { return Utils::toJObjectArray(obj); }

};

template<>
struct CPPToJNIConversor<std::map < std::string, std::string>> {
using JNIType = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'u', 't', 'i', 'l', '/', 'H', 'a', 's', 'h', 'M', 'a', 'p', ';'>;

inline static jobject convert(const std::map <std::string, std::string> &obj) { return Utils::toHashMap(obj); }

};
*/

//Add more types here

//void conversor

#define CPP_JNI(type, jnitype, cname, convertFn) \
template<> \
struct CPPToJNIConversor<type> { \
    using JNITypeName = cname; \
    using JNIType = jnitype; \
    inline static jnitype convert(type& obj) { \
        convertFn; \
    };\
    inline static jnitype convert(std::nullptr_t) { };\
};

CPP_JNI(jlong, jlong, CompileTimeString<'J'>, return obj);

CPP_JNI(jint, jint, CompileTimeString<'I'>, return obj);

CPP_JNI(jshort, jshort, CompileTimeString<'S'>, return obj);

CPP_JNI(jchar, jchar, CompileTimeString<'C'>, return obj);

CPP_JNI(jbyte, jbyte, CompileTimeString<'B'>, return obj);

CPP_JNI(jfloat, jfloat, CompileTimeString<'F'>, return obj);

CPP_JNI(jdouble, jdouble, CompileTimeString<'D'>, return obj);

CPP_JNI(jboolean, jboolean, CompileTimeString<'Z'>, return obj);

//CPP_JNI(string, jstring, (), return EasyJNI::Utils::toString(obj));

template<>
struct CPPToJNIConversor<jstring> {
    using JNITypeName = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    using JNIType = jstring;

    static JNIType convert(jstring obj) {
        return obj;
    };
};

template<>
struct CPPToJNIConversor<std::string> {
    using JNITypeName = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    using JNIType = jstring;

    static JNIType convert(std::string obj) {
        return EasyJNI::Utils::toJString(obj.c_str());
    };
};

template<>
struct CPPToJNIConversor<const char *> {
    using JNITypeName = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'S', 't', 'r', 'i', 'n', 'g', ';'>;
    using JNIType = jstring;

    static JNIType convert(const char *obj) {
        return EasyJNI::Utils::toJString(obj);
    };
};

template<>
struct CPPToJNIConversor<jobject> {
    using JNITypeName = CompileTimeString<'L', 'j', 'a', 'v', 'a', '/', 'l', 'a', 'n', 'g', '/', 'O', 'b', 'j', 'e', 'c', 't', ';'>;
    using JNIType = jobject;

    inline static jobject convert(jobject obj) { return obj; }
};

template<typename T>
struct CPPToJNIConversor<T *> {
    using JNITypeName = CPPToJNIConversor<jlong>::JNITypeName;
    using JNIType = CPPToJNIConversor<jlong>::JNIType;

    inline static jlong convert(T *&obj) {
        return (jlong) obj;
    };
};

#define CPP_JNI_NAME_FROM_JNI(cppType, jniType) CPP_JNI(cppType, jniType, CPPToJNIConversor<jniType>::JNITypeName , return static_cast<jniType>(obj))

CPP_JNI_NAME_FROM_JNI(bool, jboolean)

CPP_JNI_NAME_FROM_JNI(char, jbyte)

template<>
struct CPPToJNIConversor<void> {
    using JNITypeName = CompileTimeString<'V'>;
    using JNIType = void;
};


#pragma mark JNI To C++ conversion templates

template<typename T>
struct JNIToCPPConversor {
    inline static T convert(T obj) {
        return obj;
    }
};

template<>
struct JNIToCPPConversor<void> {
    inline static void convert(void) {}
};

#define JNI_CPP(jniType, cppType, actor) \
template<> \
struct JNIToCPPConversor<cppType> { \
    inline static cppType convert(jniType obj){ \
        actor; \
    } \
    using JNIType = jniType; \
};


JNI_CPP(jboolean, bool, return obj);

JNI_CPP(jbyte, char, return obj);

JNI_CPP(jchar, uint16_t, return obj);

JNI_CPP(jshort, int16_t, return obj);

JNI_CPP(jint, int32_t, return obj);

JNI_CPP(jlong, int64_t, return obj);

//Ptr cast
template<typename T>
struct JNIToCPPConversor<T *> {
    inline static T *convert(jlong obj) {
        return (T *) obj;
    }

    using JNIType = jlong;
};


template<>
struct JNIToCPPConversor<std::string> {
    using JNIType = jstring;

    inline static std::string convert(jstring obj) { return EasyJNI::Utils::toString(obj); }
};

/*
template<>
struct JNIToCPPConversor<std::vector < std::string>> {
inline static std::vector <std::string> convert(jobject obj) { return Utils::toVectorString((jobjectArray) obj); }

};

template<>
struct JNIToCPPConversor<std::vector < uint8_t>> {
inline static std::vector <uint8_t> convert(jobject obj) { return Utils::toVectorByte((jbyteArray) obj); }

};

template<>
struct JNIToCPPConversor<std::vector < jobject>> {
inline static std::vector <jobject> convert(jobject obj) { return Utils::toVectorJObject((jobjectArray) obj); }

};
 */


#pragma mark JNI Signature Utilities

//deduces the signature of a JNI method according to the variadic params and the return type
template<typename T, typename... Args>
inline const char *getJNISignature(Args...) {
    return Concatenate<CompileTimeString<'('>, //left parenthesis
            typename CPPToJNIConversor<Args>::JNIType..., //params signature
            CompileTimeString<')'>, //right parenthesis
            typename CPPToJNIConversor<T>::JNIType,
            CompileTimeString<'\0'>> //return type signature
    ::Result::value();
}


#pragma mark JNI Param Destructor Templates

//Helper object to destroy parameters converter to JNI
template<uint8_t NUM_PARAMS>
struct JNIParamDestructor {
    JNIEnv *jniEnv;
    jobject jniParams[NUM_PARAMS] = {0};
    int currentIndex;

    JNIParamDestructor(JNIEnv *env) : jniEnv(env), currentIndex(0) {

    }

    jobject add(jobject jniObject) {
        jniParams[currentIndex++] = jniObject;
        return jniObject;
    }

    ~JNIParamDestructor() {
        //printf("Deleting refs!");
        for (int i = 0; i < NUM_PARAMS; ++i) {
            if (jniParams[i])
                if(jniEnv->GetObjectRefType(jniParams[i]) == jobjectRefType::JNILocalRefType)
                    jniEnv->DeleteLocalRef(jniParams[i]);
        }
        JNI_EXCEPTION_CHECK
    }
};

//optimized base case for the destructor
template<>
struct JNIParamDestructor<0> {
    JNIParamDestructor(JNIEnv *env) {}

    ~JNIParamDestructor() {
        JNI_EXCEPTION_CHECK
    }
};


//helper template to decide when a jni type must be destroyed (by adding the ref to the JNIParamDestructor struct);
template<typename T, typename D>
struct JNIDestructorDecider {
    //by default the template ignores destruction (for primivitve types)
    inline static void deleteReference(T jniParam, D &destructor) {}
};

template<typename D>
struct JNIDestructorDecider<jobject, D> {
    inline static void deleteReference(jobject obj, D &destructor) { destructor.add(obj); }
};

template<typename D>
struct JNIDestructorDecider<jbyteArray, D> {
    inline static void deleteReference(jbyteArray obj, D &destructor) { destructor.add((jobject) obj); }
};

template<typename D>
struct JNIDestructorDecider<jobjectArray, D> {
    inline static void deleteReference(jobjectArray obj, D &destructor) { destructor.add((jobject) obj); }
};

template<typename D>
struct JNIDestructorDecider<jstring, D> {
    inline static void deleteReference(jstring obj, D &destructor) { destructor.add((jobject) obj); }
};

#pragma mark JNI Param Conversor Utility Template

//JNI param conversor helper: Converts the parameter to JNI and adds it to the destructor if needed
template<typename T, typename D>
auto JNIParamConversor(T &arg, D &destructor) -> decltype(CPPToJNIConversor<T>::convert(arg)) {
    auto result = CPPToJNIConversor<T>::convert(arg);
    JNIDestructorDecider<decltype(result), D>::deleteReference(result, destructor);
    return result;
}

/*
#define CLASS_PCONV(name) \
template<typename D> \
auto JNIParamConversor(JNI_CLASS_PTR(name) &arg, D &destructor) -> decltype(CPPToJNIConversor<JNI_CLASS_PTR(name)>::convert(nullptr)) { \
    auto result = CPPToJNIConversor<JNI_CLASS_PTR(name)>::convert(arg); \
    JNIDestructorDecider<decltype(result), D>::deleteReference(result, destructor); \
    return result; \
}
 */