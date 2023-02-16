//
// Created by os on 5/19/22.
//

#ifndef PROJECT_BASE_V1_0_PCB_H
#define PROJECT_BASE_V1_0_PCB_H
#include "../lib/hw.h"
//#include "TrapHandle.h"

class __Semaphore;

class PCB{
    friend class Scheduler;
    friend class __Semaphore;
    struct Context{
        uint64 ra;
        uint64 sp;
        volatile uint64 sepc;
        volatile uint64 sstatus;
    };

    uint64* stack;
    void(*routine)(void*);
    void* args;
    PCB* next;
    PCB* nextSem;
    PCB* nextSleep;
    bool finished;
    bool suspended;
    Context context;
    uint64 time_slice;
    int sleepTime;



    static void ThreadWrapper();

public:
    uint64 getSEPC(){return context.sepc;}

    uint64 getStatus(){return context.sstatus;
    }
    static void saveSEPC();

    static void loadSEPC();

    static uint64 timeSliceCount;

    void operator delete (void*);

    bool isFinished() const {return finished;}

    bool isSuspended() const{return suspended;}

    void setFinished(bool status){finished=status;}

    void setSuspended(bool status){suspended=status;}

    void setTimeSlice(uint64 time){time_slice=time;}

    uint64 getTimeSlice(){return time_slice;}

    static void popSppSpie();
    static void yield();

    static void dispatch();

    static int thread_create(PCB** handle,void(*start_routine)(void*),void* arg,void* stack_space);

    static int thread_createNoStart(PCB** handle,void(*start_routine)(void*),void* arg,void* stack_space);

    static void contextSwitch(Context* oldContext,Context* newContext);

    static int thread_exit();

    static int sleep(uint64 time);


};

typedef PCB* thread_t;
#endif //PROJECT_BASE_V1_0_PCB_H
