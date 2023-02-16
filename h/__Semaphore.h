//
// Created by os on 5/22/22.
//

#ifndef PROJECT_BASE_V1_0_SEMAPHORE_H
#define PROJECT_BASE_V1_0_SEMAPHORE_H

#include "../lib/hw.h"
#include "PCB.h"
class __Semaphore{

    int count;
    bool closed;
    thread_t listHead;
    thread_t last;

public:

    int wait();

    int signal();

    static int sem_open(__Semaphore** handle, unsigned init);

    static int sem_close(__Semaphore* handle);
};

typedef __Semaphore* sem_t;
#endif //PROJECT_BASE_V1_0_SEMAPHORE_H
