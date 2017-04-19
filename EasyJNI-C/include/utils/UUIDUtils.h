//
// Created by wolverindev on 24.08.16.
//
#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <uuid/uuid.h>

#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/md5.h>

using namespace std;
extern uuid_t uuid_empty;

namespace UUIDUtils {
    extern string toString(uuid_t& uuid);
    extern bool uuidFromString(uuid_t& out, string suuid, bool braces = true);

    extern bool offlineUUID(uuid_t& out, string name);

    extern string getUUIDString36(string str32);
}