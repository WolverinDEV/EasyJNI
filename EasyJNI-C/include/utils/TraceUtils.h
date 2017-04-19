//
// Created by wolverindev on 25.10.16.
//

#pragma once

#include <string>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../log/LogUtils.h"

#define BT_BUF_SIZE 100

using namespace std;

struct StackTraceElement {
    int index;
    string path;
};

struct StackTrace {
    StackTraceElement* elements;
    int stackSize;

    void printStackTrace(){
        printStackTrace([](StackTraceElement e){
           return  "Element ["+to_string(e.index)+"] = "+e.path;
        });
    }

    void printStackTrace(std::function<string(StackTraceElement)> format) {
        for (int i = 0; i < stackSize; i++)
            if (Terminal::isActive())
                Terminal::getInstance()->writeMessage(format(elements[i]));
            else
                printf(format(elements[i]).c_str());
    }

    ~StackTrace(){
        delete[] elements;
    }
};

class TraceUtils {
    public:
        static StackTrace backTrace(int size){
            int j, nptrs;
            void *buffer[BT_BUF_SIZE];
            char **strings;

            nptrs = backtrace(buffer, BT_BUF_SIZE);

            strings = backtrace_symbols(buffer, nptrs);
            if (strings == NULL) {
                perror("backtrace_symbols");
                exit(EXIT_FAILURE);
            }

            StackTrace out;
            out.elements = new StackTraceElement[nptrs];
            out.stackSize = nptrs;

            for (j = 0; j < nptrs; j++)
                out.elements[j] = StackTraceElement{j, std::string(strings[j])};
            free(strings);

            return out;
        }
};
