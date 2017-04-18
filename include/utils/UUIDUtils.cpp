//
// Created by wolverindev on 29.03.17.
//

#include "UUIDUtils.h"
#include "cstring"

#define UUID_BUFFER_LENGTH 37 //Length 36+String sep

uuid_t uuid_empty = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

namespace UUIDUtils {
    string toString(uuid_t& uuid){
        char uuid_str[UUID_BUFFER_LENGTH];
        uuid_unparse(uuid, uuid_str);
        return string(uuid_str);
    }

    bool uuidFromString(uuid_t& out, string suuid, bool braces = true){
        if(braces){
            uuid_parse(suuid.c_str(),out);
            return true;
        }else {
            //8-4-4-4-12
            return uuidFromString(out, getUUIDString36(suuid));
        }
    }

    extern bool offlineUUID(uuid_t& out, string name){
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, (unsigned char*) name.c_str(), name.size());

        memset(out, 0, 16);

        MD5_Final((u_char*) out, &ctx);

        out[6]  &= 0x0f;  /* clear version */
        out[6]  |= 0x30;  /* set to version 3 */
        out[8]  = out[8] & 0x3f;  /* clear variant */
        out[8]  = out[8] | 0x80;  /* set to IETF variant JAVA :P*/
        return true;
    }

    string getUUIDString36(string str32){
        std::stringstream ss;
        ss << str32.substr(0,8) << "-" << str32.substr(8,4) << "-" << str32.substr(12, 4) << "-" << str32.substr(16, 4) << "-" << str32.substr(20, 12);
        return ss.str();
    }
}