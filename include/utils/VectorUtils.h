//
// Created by wolverindev on 26.12.16.
//

#pragma once

#include <vector>
#include <algorithm>

class VectorUtils {
    public:
        template <typename T>
        static void remove(std::vector<T>& vec, T& obj){
            if(vec.empty()) return;
            auto res = std::find(vec.begin(), vec.end(), obj);
            //if(res == vec.end())
            //    return;
            vec.erase(res);
        }
};