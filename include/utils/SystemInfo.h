//
// Created by wolverindev on 25.12.16.
//

#pragma once

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"

namespace Systeminfo {
    extern double getCPUUsage();
    extern double getMaxCPU();

    extern int getUsedMemory();
}