#pragma once

#include "JavaClassInfo.h"
#include "JavaField.h"

#define JFT_PRIMATIVE(type, mapping) \
JFT(type,\
EasyJNI::Utils::getJNIEnvAttach()->SetStatic ##mapping ##Field(field->getClassInfo()->getJavaClass(), field->getFieldId(), val); \
return true; \
,\
return EasyJNI::Utils::getJNIEnvAttach()->GetStatic ##mapping ##Field(field->getClassInfo()->getJavaClass(), field->getFieldId()); \
,\
EasyJNI::Utils::getJNIEnvAttach()->Set ##mapping ##Field(field->getClassInstance()->getJavaInstance(), field->getFieldInfo()->getFieldId(), val); \
return true; \
,\
return EasyJNI::Utils::getJNIEnvAttach()->Get ##mapping ##Field(field->getClassInstance()->getJavaInstance(), field->getFieldInfo()->getFieldId()); \
)

JFT_PRIMATIVE(jboolean, Boolean)

JFT_PRIMATIVE(jbyte , Byte)
JFT_PRIMATIVE(jshort , Short)
JFT_PRIMATIVE(jchar , Char)
JFT_PRIMATIVE(jint , Int)
JFT_PRIMATIVE(jlong , Long)

JFT_PRIMATIVE(jdouble , Double)
JFT_PRIMATIVE(jfloat , Float)