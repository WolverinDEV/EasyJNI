//
// Created by wolverindev on 29.03.17.
//
#pragma once

#include <functional>

namespace ThreadUtils {
    extern void runAsync(std::function<void(void)>);
}