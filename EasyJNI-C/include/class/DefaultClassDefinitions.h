//
// Created by wolverindev on 22.04.17.
//
#include "../JavaClassDefine.h"

JNI_CLASS(java_lang, Throwable,
    JNI_CLASS_METHODE("getMessage", getMessage, std::string);
)