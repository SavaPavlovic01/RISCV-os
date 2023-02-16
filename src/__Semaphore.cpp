//
// Created by os on 5/22/22.
//
#include "../h/__Semaphore.h"
#include "../h/scheduler.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_cpp.hpp"

int __Semaphore::wait() {
    if(this->closed) return -1;
    count--;
    if(count<0){
        //wait at semaphore
        if(!listHead){
            listHead=Scheduler::runningThread;
            listHead->setSuspended(true);
            listHead->nextSem= nullptr;
            last=listHead;
            PCB::yield();
            if(this->closed) return -1;
            return 0;
        }
        last->nextSem=Scheduler::runningThread;
        Scheduler::runningThread->setSuspended(true);
        last->nextSem->nextSem= nullptr;
        last=last->nextSem;
        PCB::yield();
        if(this->closed) return -1;
        return 0;
    }
    return 0;
}

int __Semaphore::signal() {
    if(this->closed) return -1;

    if(count++<0){
        //unblock a thread
        thread_t toUnblock =listHead;
        toUnblock->setSuspended(false);
        listHead=listHead->nextSem;
        Scheduler::put(toUnblock);
        return 0;
    }

    return 0;

}

int __Semaphore::sem_open(__Semaphore **handle, unsigned int init) {
    int size= sizeof(__Semaphore) / MEM_BLOCK_SIZE;
    if(sizeof(__Semaphore) % MEM_BLOCK_SIZE != 0) size++;
    __Semaphore* newSem=(__Semaphore*)MemoryAllocator::mem_alloc(size);
    if(!newSem) return -1;
    newSem->count=init;
    newSem->listHead= nullptr;
    newSem->last= nullptr;
    newSem->closed=false;
    *handle=newSem;
    return 0;

}

int __Semaphore::sem_close(__Semaphore *handle) {
    handle->closed=true;
    for(;handle->listHead;handle->listHead=handle->listHead->next){
        handle->listHead->setSuspended(false);


        Scheduler::put(handle->listHead);
    }
    return 0;

}
