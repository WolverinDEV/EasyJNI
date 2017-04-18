//
// Created by wolverindev on 24.08.16.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <algorithm>

using namespace std;

class StringUtils {
    public:
        static void split(const string &s, string delim, vector<string> &elems) {
            auto start = 0U;
            auto end = s.find(delim);
            while (end != std::string::npos)
            {
                elems.push_back(s.substr(start, end - start));
                start = end + delim.length();
                end = s.find(delim, start);
            }
            elems.push_back(s.substr(start, end - start));
        }

        static string replaceAll(std::string str, const std::string& from, const std::string& to) {
            size_t start_pos = 0;
            while((start_pos = str.find(from, start_pos)) != std::string::npos) {
                str.replace(start_pos, from.length(), to);
                start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
            return str;
        }

        static string copy(string old){
            const char* _new = new char(old.size()+1);
            memcpy((char*) _new,old.c_str(),old.size()+1);
            return _new;
        }

        static vector<string> split(const string &s, string delim) {
            vector<string> elems = vector<string>();
            split(s, delim, elems);
            return elems;
        }

        static bool contains(string& a, string b){
            return a.find(b) != string::npos;
        }

        /**
         *
         * @param in
         * @param fill
         * @param size
         * @param bounded -1|0|1
         * @return
         */
        static string fill(string in,string fill, int size, int bounded = 1){
            string out = in;
            while (out.size() < size){
                switch (bounded){
                    case -1:
                        out = fill + out;
                        break;
                    case 0:
                        out = fill + out + fill;
                        break;
                    case 1:
                        out += fill;
                        break;
                }
            }
            return out;
        }

        static bool equalsIgnoreCase(string a,string b){
            string ca(a);
            string cb(b);
            std::transform( ca.begin(), ca.end(), ca.begin(), ::tolower );
            std::transform( cb.begin(), cb.end(), cb.begin(), ::tolower );
            return ca == cb;
        }
};
