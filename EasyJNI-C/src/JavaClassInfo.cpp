//
// Created by wolverindev on 18.04.17.
//

#include "../include/class/JavaClassInfo.h"
#include "../include/class/JavaField.h"
#include "../include/utils/StringUtil.h"

vector<JavaClassInfo*> JavaClassInfo::loadedClasses;

JavaClassInfo::JavaClassInfo(const char *package, const char *name) : package(StringUtils::replaceAll(package, "_", ".")), name(name) {
    JavaClassInfo::loadedClasses.push_back(this);
}