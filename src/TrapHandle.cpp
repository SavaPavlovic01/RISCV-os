//
// Created by os on 5/17/22.
//
#include "../h/TrapHandle.h"
#include "../h/MemoryAllocator.h"
#include "../h/PCB.h"
#include "../h/__Semaphore.h"
#include "../h/scheduler.h"
#include "../h/riscv.h"
#include "../h/__Console.h"
extern "C" void handleSupervisorTrap(){
    uint64 id;
    __asm__ volatile ("mv %[var], a0" : [var] "=r" (id));
    volatile uint64 scause;

    __asm__ volatile("csrr %[cause],scause" : [cause] "=r" (scause));


    if(scause==0x8000000000000001UL){
        mc_sip(SSTATUS_SIE);
        PCB::timeSliceCount++;
        Scheduler::getSleep();
        if(PCB::timeSliceCount>=Scheduler::runningThread->getTimeSlice()){

            PCB::timeSliceCount=0;
            PCB::dispatch();
        }


    }else if (scause == 0x8000000000000009UL){
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)

        int plic=plic_claim();
        if(plic==10){

            if((*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT)){

                __Console::interruptReceive->signal();
            }

            if((*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT)){
                __Console::interruptSend->signal();
            }

        }
        plic_complete(plic);

    } else if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){

        uint64 pc;

        __asm__ volatile ("csrr %[ime], sepc" : [ime] "=r" (pc));
        pc+=4;
        __asm__ volatile ("csrw sepc, %[vector]" : : [vector] "r" (pc));


        if(id==1){
            size_t size;
            __asm__ volatile ("mv %[sz], a1" : [sz] "=r" (size));

            MemoryAllocator::mem_alloc(size);
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }

        if(id==2){
            void* addr;
            __asm__ volatile ("mv %[sz], a1" : [sz] "=r" (addr));

            MemoryAllocator::mem_free(addr);
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }

        if(id==18){
            PCB::thread_exit();
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }

        if(id==19){
           // uint64 sepc;
            uint64 sstatus;
            //__asm__ volatile("csrr %[sepc],sepc" : [sepc] "=r" (sepc));
            __asm__ volatile("csrr %[status],sstatus" : [status] "=r" (sstatus));
            PCB::timeSliceCount=0;
            PCB::dispatch();
            //__asm__ volatile("csrw sepc,%[pc]" : : [pc] "r" (sepc));
            __asm__ volatile("csrw sstatus,%[status]" : : [status] "r" (sstatus));
        }

        if(id==20){
            //ovo ili 19 treba da se brise(msm da je ovo ali nisam siguran
            PCB::dispatch();
            return;
        }
        if(id==11){
            // sys call thread_create
            thread_t *handle;
            void (*start_routine)(void *);
            void *arg;
            void *stack_space;

            __asm__ volatile("mv %[hand],a1" : [hand] "=r" (handle));
            __asm__ volatile("mv %[rout],a2" : [rout] "=r" (start_routine));
            __asm__ volatile("mv %[args],a3" : [args] "=r" (arg));
            __asm__ volatile("mv %[stack],s11" : [stack] "=r" (stack_space));
            PCB::thread_create(handle,start_routine,arg,stack_space);
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }
        if(id==12){
            thread_t *handle;
            void (*start_routine)(void *);
            void *arg;
            void *stack_space;

            __asm__ volatile("mv %[hand],a1" : [hand] "=r" (handle));
            __asm__ volatile("mv %[rout],a2" : [rout] "=r" (start_routine));
            __asm__ volatile("mv %[args],a3" : [args] "=r" (arg));
            __asm__ volatile("mv %[stack],s11" : [stack] "=r" (stack_space));
            PCB::thread_createNoStart(handle,start_routine,arg,stack_space);
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }
        if(id==13){
            thread_t thr;
            __asm__ volatile("mv %[hand],a1" : [hand] "=r" (thr));
            Scheduler::put(thr);
            return;
        }
        if(id==33){
            // sys call sem_open
            sem_t* handle;
            unsigned init;

            __asm__ volatile("mv %[hand],a1" : [hand] "=r" (handle));
            __asm__ volatile("mv %[num],a2" : [num] "=r" (init));
            __Semaphore::sem_open(handle, init);
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }

        if(id==34){
            // sys call sem_close
            sem_t handle;

            __asm__ volatile("mv %[hand],a1" : [hand] "=r" (handle));
            __Semaphore::sem_close(handle);
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }
        if(id==35){
            // sys call sem_wait
            sem_t sem;
            __asm__ volatile("mv %[sem],a1" : [sem] "=r" (sem));
            sem->wait();
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }
        if(id==36){
            // sys call sem_signal
            sem_t sem;
            __asm__ volatile("mv %[sem],a1" : [sem] "=r" (sem));
            sem->signal();
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }

        if(id==49){
            // thread_sleep
            uint64 time;
            __asm__ volatile("mv %[tm],a1" : [tm] "=r" (time));
            PCB::sleep(time);
            return;
        }
        if(id==0x41){

            __Console::getC();
            __asm__ volatile ("sd a0, 0x50(fp)");
            return;
        }
        if(id==0x42){
            char c;

            __asm__ volatile("mv %[item],a1" : [item] "=r" (c));
            __Console::putC(c);
            return;
        }
    }else{

        __Console::putC('w');
    }

}

extern "C" void supervisorTrap();

void sysCall(){
    //__asm__ volatile ("csrw stvec, %[vector]" : : [vector] "r" (&supervisorTrap));
    __asm__ volatile ("ecall");
}
