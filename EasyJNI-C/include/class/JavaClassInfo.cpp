//
// Created by wolverindev on 18.04.17.
//

#include "JavaClassInfo.h"
#include "JavaField.h"
#include "../utils/StringUtil.h"

JavaClassInfo::JavaClassInfo(const char *package, const char *name) : package(StringUtils::replaceAll(package, "_", ".")), name(name) {
    //thisInstance = this;
}

std::vector<JavaFieldInfo*> JavaClassInfo::getFields(){
    return fields;
}