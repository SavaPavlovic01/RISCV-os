//
// Created by os on 6/1/22.
//
#include "../h/__Console.h"
#include "../h/MemoryAllocator.h"
#include "../h/PCB.h"
int __Console::size=0;
int __Console::headIn=0;
int __Console::headOut=0;
int __Console::tailOut=0;
int __Console::tailIn=0;
sem_t __Console::inBuffItemAv= nullptr;
sem_t __Console::inBuffSpaceAv= nullptr;
sem_t __Console::outBuffItemAv= nullptr;
sem_t __Console::outBuffSpaceAv= nullptr;
sem_t __Console::interruptSend= nullptr;
sem_t __Console::interruptReceive= nullptr;
sem_t __Console::blockedPut= nullptr;
char* __Console::inBuff= nullptr;
char* __Console::outBuff= nullptr;


int __Console::putC(char item) {

    outBuffSpaceAv->wait();
    outBuff[tailOut]=item;
    tailOut=(tailOut+1)%size;
    outBuffItemAv->signal();
    // blockedPut->wait();

    return 0;
}

char __Console::getC() {
    inBuffItemAv->wait();
    char ret=inBuff[headIn];
    headIn=(headIn+1)%size;
    inBuffSpaceAv->signal();
    return ret;
}

void __Console::readFromConsole(void*) {
    while(true){
        interruptReceive->wait();
        int cnt=0;
        while((*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT)){
            cnt++;
            inBuffSpaceAv->wait();
            inBuff[tailIn]=(*(char*)CONSOLE_RX_DATA);
            tailIn=(tailIn+1)%size;
            inBuffItemAv->signal();
            if(cnt==10) break;
            }
        //PCB::dispatch();

    }
}

void __Console::sendToConsole(void*) {
    while(true){
        //vrv bolje ovde da sacekas
        interruptSend->wait();
        while((*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT)){
            outBuffItemAv->wait();
            *(char*)CONSOLE_TX_DATA=outBuff[headOut];
            headOut=(headOut+1)%size;
            outBuffSpaceAv->signal();
            blockedPut->signal();
        }
        //PCB::dispatch();
    }
}

int __Console::initConsole(int sz) {
    size=sz*MEM_BLOCK_SIZE;
    __Semaphore::sem_open(&inBuffItemAv, 0);
    __Semaphore::sem_open(&outBuffItemAv, 0);
    __Semaphore::sem_open(&inBuffSpaceAv, size );
    __Semaphore::sem_open(&outBuffSpaceAv, size );
    __Semaphore::sem_open(&interruptReceive, 0);
    __Semaphore::sem_open(&interruptSend, 0);
    __Semaphore::sem_open(&blockedPut, 0);
    inBuff=(char*)MemoryAllocator::mem_alloc(sz);
    outBuff=(char*)MemoryAllocator::mem_alloc(sz);
    return 0;
}


/*
int sendToConsole(){
    while(true){
        //sacekaj na semaforu koji kaze da li je stigao prekid
        outBuffItemAv->wait();// blokiraj ovu nit ako nemamo nista da saljemo
        // sve dok imas sta da saljes salji, ili dok konzola ne moze vise da primi
        // nisam siguran sta ako nemamo sta vise da saljemo, da li onda da blokiram na outBuff semaforu?
        //vrv da
        //svaki put kad posalje radis outBuffAv->wait
        //tu negde se blokiraj na ovom prvom semaforu
    }
}

int getFromConsole(){
    while(true){
        //interupt->wait();
        //inBuffSpaceAv->wait();
        //pisi u inBuff dok ga ne napunis ili dok nema sta da salje
    }
}
*/