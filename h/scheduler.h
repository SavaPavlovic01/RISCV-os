//
// Created by os on 5/18/22.
//

#ifndef PROJECT_BASE_V1_0_SCHEDULER_H
#define PROJECT_BASE_V1_0_SCHEDULER_H

#include "../h/PCB.h"

class Scheduler{

    static thread_t listHead;
    static thread_t last;

    static thread_t sleepHead;
public:

    static thread_t runningThread;

    static void put(thread_t thread);

    static thread_t get();

    static void putSleep();

    static void getSleep();

};




#endif //PROJECT_BASE_V1_0_SCHEDULER_H
