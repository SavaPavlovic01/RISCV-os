//
// Created by os on 6/1/22.
//

#ifndef PROJECT_BASE_V1_0_CONSOLE_H
#define PROJECT_BASE_V1_0_CONSOLE_H
#include "../h/__Semaphore.h"
class __Console{

    static int size;
    static char *inBuff;// ovo je ono sto je stiglo od konzole, getc uzima odavde
    static char *outBuff;// ovaj se salje konzoli, putc stavlja ovde
    static sem_t inBuffSpaceAv,inBuffItemAv;
    static sem_t outBuffSpaceAv,outBuffItemAv;

    static sem_t blockedPut;
    static int headOut;
    static int tailOut;
    static int headIn;
    static int tailIn;



public:

    static sem_t interruptSend;
    static sem_t interruptReceive;



    static int initConsole(int sz);

    static int putC(char);

    static char getC();

    static int interruptSendSignal(){return interruptSend->signal();}

    static int interruptReceiveSignal(){return interruptReceive->signal();}

    static void readFromConsole(void*);

    static void sendToConsole(void*);
};

#endif //PROJECT_BASE_V1_0_CONSOLE_H
