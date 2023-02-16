//
// Created by os on 5/18/22.
//
#include "../h/syscall_c.h"
#include "../h/scheduler.h"
#include "../h/__Console.h"

#include "../h/riscv.h"
void* mem_alloc(size_t size){
    uint64 id=1;
    size_t sizeInBlock=size/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE!=0) sizeInBlock++;
    __asm__ volatile ("mv a0, %[vector]" : : [vector] "r" (id));
    __asm__ volatile ("mv a1, %[sz]" : : [sz] "r" (sizeInBlock));
    sysCall();
    void* res;
    __asm__ volatile ("mv %[ime], a0" : [ime] "=r" (res));
    return res;
}

int mem_free(void* addr){
    uint64 id=2;

    __asm__ volatile ("mv a1, a0");
    __asm__ volatile ("mv a0, %[sz]" : : [sz] "r" (id));
    sysCall();
    int res;
    __asm__ volatile ("mv %[ime], a0" : [ime] "=r" (res));
    return res;
}

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg){

    void* stack= mem_alloc(2*DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv s11,%[stack]" : : [stack] "r" (stack));
    __asm__ volatile ("mv a3,%[args]" : : [args] "r" (arg));
    __asm__ volatile ("mv a2,%[addr]" : : [addr] "r" (start_routine));
    __asm__ volatile ("mv a1,%[h]" : : [h] "r" (handle));
    uint64 id=11;

    __asm__ volatile ("mv a0,%[id]" : : [id] "r" (id));

    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;

}

int thread_create_noStart(thread_t* handle,void(*start_routine)(void*),void* arg){

    void* stack= mem_alloc(2*DEFAULT_STACK_SIZE);
    __asm__ volatile ("mv s11,%[stack]" : : [stack] "r" (stack));
    __asm__ volatile ("mv a3,%[args]" : : [args] "r" (arg));
    __asm__ volatile ("mv a2,%[addr]" : : [addr] "r" (start_routine));
    __asm__ volatile ("mv a1,%[h]" : : [h] "r" (handle));
    uint64 id=12;

    __asm__ volatile ("mv a0,%[id]" : : [id] "r" (id));

    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;
}

void startThread(thread_t thr){
    __asm__ volatile ("mv a1,a0");
    uint64 id=13;

    __asm__ volatile ("mv a0,%[id]" : : [id] "r" (id));
    sysCall();
}

void thread_dispatch(){
    uint64 id=0x13;
    __asm__ volatile ("mv a0, %[id]" : : [id] "r" (id));
    sysCall();


}

int thread_exit(){
    uint64 id=0x12;
    __asm__ volatile ("mv a0, %[id]" : : [id] "r" (id));
    sysCall();
    int ret;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (ret));
    return ret;

}

int sem_open(sem_t* handle,unsigned init){
    //uint64 id=0x21;
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1,a0");
    __asm__ volatile("li a0,0x21");
    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;

}

int sem_close(sem_t handle){

    __asm__ volatile ("mv a1,a0");
    __asm__ volatile ("li a0,0x22");
    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;
}

int sem_wait(sem_t id){
    __asm__ volatile ("mv a1,a0");
    __asm__ volatile ("li a0,0x23");
    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;
}

int sem_signal(sem_t id){
    __asm__ volatile ("mv a1,a0");
    __asm__ volatile ("li a0,0x24");
    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;
}

int time_sleep(uint64 time){
    __asm__ volatile ("mv a1,a0");
    __asm__ volatile ("li a0,0x31");
    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;
}

char getc(){
    __asm__ volatile ("li a0,0x41");
    sysCall();
    int res;
    __asm__ volatile("mv %[res],a0" : [res] "=r" (res));
    return res;
}

void putc(char c){
    __asm__ volatile ("mv a1,a0");
    __asm__ volatile ("li a0,0x42");
    sysCall();
}




