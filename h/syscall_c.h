//
// Created by os on 5/17/22.
//

#ifndef PROJECT_BASE_V1_0_SYSCALL_C_H
#define PROJECT_BASE_V1_0_SYSCALL_C_H

#include "../h/TrapHandle.h"
#include "PCB.h"
#include "__Semaphore.h"

void* mem_alloc(size_t size);

int mem_free(void* addr);

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_create_noStart (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);

int thread_exit ();

void thread_dispatch ();

int sem_open (
        sem_t* handle,
        unsigned init
);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

int time_sleep(uint64 time);

char getc();

void putc(char);

void startThread(thread_t);

#endif //PROJECT_BASE_V1_0_SYSCALL_C_H

