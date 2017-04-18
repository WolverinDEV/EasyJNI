//
// Created by wolverindev on 24.10.16.
//

#ifndef MCJOINBOT_CHATUTILS_H
#define MCJOINBOT_CHATUTILS_H

#include <string>
#include "../json/json.hpp"

using json = nlohmann::json;
using namespace std;

class ChatUtils {
    public:
        static string getRawMessage(string in){
            json j = json::parse(in);
            return getRawMessage((json) j);
        }

        static string getRawMessage(json in){
            string out = "";
            if(in.is_array()){
                for(auto element : in){
                    out += getRawMessage(in);
                }
            } else if(in.is_string()){
                out += in;
            } else {
                if(in.count("text") > 0){
                    json text = in["text"];
                    if(text.is_string())
                        out += text;
                    else if(text.is_array())
                        for(auto element : in){
                            out += element;
                        }
                }
                if(in.count("extra") > 0){
                    for(auto element : in["extra"]){
                        out += getRawMessage(element);
                    }
                }
            }
            return out;
        }
};

#endif //MCJOINBOT_CHATUTILS_H
