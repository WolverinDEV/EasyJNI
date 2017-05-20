#include "../JavaClassDefine.h"

JNI_CLASS(java_lang, Throwable,
    JNI_CLASS_METHODE("getMessage", getMessage, std::string);
)