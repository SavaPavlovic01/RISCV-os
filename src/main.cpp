//
// Created by os on 5/16/22.
//

#include "../h/__Console.h"
#include "../h/scheduler.h"
#include "../h/riscv.h"
#include "../h/TrapHandle.h"
#include "../h/syscall_c.h"
#include "../h/userMain.h"

void funMain (void*){
    //__asm__ volatile("csrr a0,sstatus");
    userMain();
}
void main(){
    __asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap));
    thread_t mejn;
    thread_t test;
    thread_t t1;
    thread_t t2;
    thread_create(&mejn, nullptr, nullptr);
    thread_create(&test,&funMain, nullptr);
    thread_create(&t2, &(__Console::sendToConsole), nullptr);
    thread_create(&t1, &(__Console::readFromConsole), nullptr);
    Scheduler::runningThread=mejn;
    __Console::initConsole(16);

    ms_sstatus(SSTATUS_SIE);

    while(!test->isFinished()){thread_dispatch();}
}
