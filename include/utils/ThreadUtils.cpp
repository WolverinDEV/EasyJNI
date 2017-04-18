//
// Created by wolverindev on 29.03.17.
//

#include "ThreadUtils.h"
#include "../log/LogUtils.h"

void fnInvoker(std::function<void(void)>* fn){
    (*fn)();
    //std::cref(*fn);
}

typedef void* (*ThreadFn)(void*);
namespace ThreadUtils {
    void runAsync(std::function<void(void)> fn){
        pthread_t handle;
        pthread_create(&handle, nullptr, fnInvoker, &fn);
        pthread_detach(handle);
    }
}