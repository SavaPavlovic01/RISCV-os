//
// Created by os on 5/19/22.
//
#include "../h/PCB.h"
#include "../h/scheduler.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.h"
#include "../h/riscv.h"
extern "C" void pushRegs();

extern "C" void popRegs();

uint64 PCB::timeSliceCount=0;

void PCB::contextSwitch(Context* oldContext, Context* newContext){
    __asm__ volatile("sd ra, 0*8(a0)");
    __asm__ volatile("sd sp, 1*8(a0)");
    __asm__ volatile("ld ra, 0*8(a1)");
    __asm__ volatile("ld sp, 1*8(a1)");
}

void PCB::yield() {
   // __asm__ volatile("mv sp,%[thr]" : : [thr] "r" (oldContext->context.sp));
    /*pushRegs();
    contextSwitch(oldContext,newContext);

    popRegs();*/
    uint64 id=0x13;
    __asm__ volatile ("mv a0, %[id]" : : [id] "r" (id));
    __asm__ volatile("ecall");
}



void PCB::dispatch(){

    if(!(Scheduler::runningThread->isFinished() or Scheduler::runningThread->isSuspended()))
        Scheduler::put(Scheduler::runningThread);
    thread_t newThread=Scheduler::get();
    saveSEPC();
    //if(Scheduler::runningThread==newThread) return;
    thread_t oldThread=Scheduler::runningThread;
    Scheduler::runningThread=newThread;
    loadSEPC();
    //PCB::yield(&oldThread->context, &newThread->context);

    contextSwitch(&oldThread->context, &newThread->context);
}

int PCB::thread_create(PCB **handle, void (*start_routine)(void *), void *arg, void *stack_space) {
    uint64 sz=sizeof(PCB)/MEM_BLOCK_SIZE;
    if(sizeof(PCB)%MEM_BLOCK_SIZE!=0) sz++;
    PCB* newPCB=(PCB*)MemoryAllocator::mem_alloc(sz);
    if(!newPCB) return -1;

    *handle=newPCB;
    if(!start_routine){

        newPCB->stack= nullptr;
        newPCB->routine= nullptr;
        newPCB->args= nullptr;
        newPCB->finished=false;
        newPCB->suspended=false;
        newPCB->nextSem= nullptr;
        newPCB->nextSleep= nullptr;
        newPCB->context.sp=0;
        newPCB->context.ra=0;
        newPCB->sleepTime=0;
        newPCB->setTimeSlice(DEFAULT_TIME_SLICE);
        return 0;
    }
    newPCB->stack=(uint64*)stack_space;
    newPCB->routine=start_routine;
    newPCB->args=arg;
    newPCB->finished=false;
    newPCB->suspended=false;
    newPCB->context.sp=(uint64)((uint64*)stack_space+2*DEFAULT_STACK_SIZE);
    newPCB->context.ra=(uint64)&ThreadWrapper;
    newPCB->nextSem= nullptr;
    newPCB->nextSleep= nullptr;
    newPCB->setTimeSlice(DEFAULT_TIME_SLICE);
    newPCB->sleepTime=0;
    Scheduler::put(newPCB);
    return 0;

}

int PCB::thread_exit() {
    Scheduler::runningThread->setFinished(true);
    yield();
    return 0;
}

void PCB::popSppSpie()
{

    __asm__ volatile ("csrw sepc, ra");
    uint64 mask=0x100;
    __asm__ volatile("csrc sstatus,%[a]" : : [a] "r" (mask));
    //ms_sstatus(SSTATUS_SIE);
    __asm__ volatile ("sret");
}

void PCB::ThreadWrapper() {
    popSppSpie();

    Scheduler::runningThread->routine(Scheduler::runningThread->args);
    Scheduler::runningThread->setFinished(true);
    yield();
    //PCB::dispatch();

    //dispatch();
}

void PCB::operator delete(void * arg) {
    PCB* toDelete=(PCB*)arg;
    MemoryAllocator::mem_free(toDelete->stack);
    MemoryAllocator::mem_free(arg);
}

int PCB::sleep(uint64 time) {

    Scheduler::runningThread->sleepTime=time;
    Scheduler::runningThread->setSuspended(true);
    Scheduler::putSleep();
    yield();
    return 0;
}

void PCB::saveSEPC() {
    uint64 sepc=r_sepc();
    volatile uint64 sstatus=r_sstatus();
    Scheduler::runningThread->context.sepc=sepc;
    Scheduler::runningThread->context.sstatus=sstatus;
}

void PCB::loadSEPC(){
    w_sstatus(Scheduler::runningThread->getStatus());
    w_sepc(Scheduler::runningThread->getSEPC());
}

int PCB::thread_createNoStart(PCB **handle, void (*start_routine)(void *), void *arg, void *stack_space) {
    uint64 sz=sizeof(PCB)/MEM_BLOCK_SIZE;
    if(sizeof(PCB)%MEM_BLOCK_SIZE!=0) sz++;
    PCB* newPCB=(PCB*)MemoryAllocator::mem_alloc(sz);
    if(!newPCB) return -1;

    *handle=newPCB;
    if(!start_routine){

        newPCB->stack= nullptr;
        newPCB->routine= nullptr;
        newPCB->args= nullptr;
        newPCB->finished=false;
        newPCB->suspended=false;
        newPCB->nextSem= nullptr;
        newPCB->nextSleep= nullptr;
        newPCB->context.sp=0;
        newPCB->context.ra=0;
        newPCB->sleepTime=0;
        newPCB->setTimeSlice(DEFAULT_TIME_SLICE);
        return 0;
    }
    newPCB->stack=(uint64*)stack_space;
    newPCB->routine=start_routine;
    newPCB->args=arg;
    newPCB->finished=false;
    newPCB->suspended=false;
    newPCB->context.sp=(uint64)((uint64*)stack_space+2*DEFAULT_STACK_SIZE);
    newPCB->context.ra=(uint64)&ThreadWrapper;
    newPCB->nextSem= nullptr;
    newPCB->nextSleep= nullptr;
    newPCB->setTimeSlice(DEFAULT_TIME_SLICE);
    newPCB->sleepTime=0;
    //Scheduler::put(newPCB);
    return 0;
}

