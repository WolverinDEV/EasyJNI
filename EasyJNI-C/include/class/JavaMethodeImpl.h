//
// Created by wolverindev on 18.04.17.
//

#pragma once

#include "JavaMethode.h"
#include "JavaClassInfo.h"

#define JMC_PRIMATIVE(type, ptype)                                                                                                                                                       \
JMIC(type,                                                                                                                                                                               \
    return EasyJNI::Utils::getJNIEnvAttach()->CallStatic##ptype ##Method(methode->getJavaClass()->getJavaClass(), methode->getMethodeId(), args...);                                     \
,                                                                                                                                                                                        \
    return EasyJNI::Utils::getJNIEnvAttach()->Call##ptype ##Method(methode->getJavaClassInstance()->getJavaInstance(), methode->getMethodeInfo()->getMethodeId(), args...);              \
);                                                                                                                                                                                       \


JMC_PRIMATIVE(jbyte, Byte);
JMC_PRIMATIVE(jshort, Short);
JMC_PRIMATIVE(jchar, Char);
JMC_PRIMATIVE(jint, Int);
JMC_PRIMATIVE(jlong, Long);

JMC_PRIMATIVE(jfloat, Float);
JMC_PRIMATIVE(jdouble, Double);

JMC_PRIMATIVE(jboolean, Boolean);

JMC_PRIMATIVE(void, Void);

JMC_PRIMATIVE(jobject, Object);
JMC_PRIMATIVE(jstring, Object);