//
// Created by wolverindev on 17.04.17.
//

#pragma once

#define CREATE_ACTION_LIMIT(name, delayCount, maxPerDelay) \
    uint64_t lastAction_##name = 0; \
    uint64_t actionCount_##name = 0; \
    int maxActions_##name = maxPerDelay; \
    int delay_##name = delayCount;

#define TRIGGER_ACTION(name, onFail) \
    auto currentTime_##name = TimeUtils::getCurrentTimeMillis();\
    if(lastAction_##name == 0 || currentTime_##name < lastAction_##name) \
        lastAction_##name = currentTime_##name; \
    if(currentTime_##name - lastAction_##name > delay_##name) { \
        actionCount_##name = 0; \
        lastAction_##name = currentTime_##name; \
    } \
    actionCount_##name += 1; \
    if(actionCount_##name > maxActions_##name){ \
        onFail \
    }

#define RESET_ACTION(name) \
    lastAction_##name = TimeUtils::getCurrentTimeMillis(); \
    actionCount_##name = 0;