//
// Created by os on 5/18/22.
//
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.h"
#include "../h/scheduler.h"
#include "../h/__Console.h"
#include "../h/riscv.h"
#include "../h/TrapHandle.h"
//#include "../h/__Semaphore.h"
class Wrap{
public:
    static void ThreadWrap(void* thr){

        ((Thread*)thr)->run();
    }
};


void* operator new(size_t size){
    return mem_alloc(size);
}

void operator delete (void* addr){
    mem_free(addr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create_noStart(&myHandle,body,arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t tm) {
    return time_sleep(tm);
}

Thread::Thread() {
    thread_create_noStart(&myHandle, &Wrap::ThreadWrap, this);
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    startThread(myHandle);
    return 0;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char ch) {
    ::putc(ch);
}
class data{
public:
    data(PeriodicThread* ptr,time_t t){
        thr=ptr;
        tm=t;
    }
    PeriodicThread* thr;
    time_t tm;
};

class PerWrap{
public:
    static void PerWrapper(void* arg){
        data* dat=(data*)arg;
        while(true){
            dat->thr->periodicActivation();
            Thread::sleep(dat->tm);

        }
    }
};

PeriodicThread::PeriodicThread(time_t period): Thread(&PerWrap::PerWrapper,(void*)new data(this,period)) {
}




