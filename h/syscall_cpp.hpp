//
// Created by os on 5/18/22.
//

#ifndef PROJECT_BASE_V1_0_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_0_SYSCALL_CPP_HPP

#include "syscall_c.h"
void* operator new (size_t);
void operator delete (void*);

class Thread {
public:
    friend class Wrap;
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console{
public:
    static char getc ();
    static void putc (char);
};


class PeriodicThread : public Thread {
protected:

    friend class PerWrap;
    PeriodicThread (time_t period);

    virtual void periodicActivation () {}
};



#endif //PROJECT_BASE_V1_0_SYSCALL_CPP_HPP
