//
// Created by os on 5/18/22.
//
#include "../h/scheduler.h"
#include "../lib/console.h"
thread_t Scheduler::runningThread= nullptr;
thread_t Scheduler::listHead= nullptr;
thread_t Scheduler::last= nullptr;
thread_t Scheduler::sleepHead= nullptr;

void Scheduler::put(thread_t thread) {

    if(!listHead){
        listHead=thread;
        thread->next= nullptr;
        last=listHead;
        return;
    }
    last->next=thread;
    last->next->next= nullptr;
    last=last->next;
}

thread_t Scheduler::get() {// sta ako je listHead null??????????????????????????????????

    thread_t temp=listHead;
    listHead=listHead->next;
    temp->next= nullptr;

    return temp;
}

void Scheduler::putSleep() {
    thread_t toSleep=Scheduler::runningThread;

    thread_t cur=Scheduler::sleepHead;
    thread_t prev= nullptr;
    int sum=0;

    if(!cur){
        sleepHead=toSleep;
        return;
    }

    for(;cur;cur=cur->nextSleep){
        sum+=cur->sleepTime;
        if(toSleep->sleepTime<=sum){
            if(!prev){
                Scheduler::sleepHead=toSleep;
                toSleep->nextSleep=cur;
                cur->sleepTime=cur->sleepTime-toSleep->sleepTime;
                return;
            }
            else{
                thread_t temp=prev->nextSleep;
                prev->nextSleep=toSleep;
                toSleep->nextSleep=temp;

                sum-=cur->sleepTime;
                toSleep->sleepTime-=sum;
                temp->sleepTime-=toSleep->sleepTime;
                return;
            }

        }
        prev=cur;
    }

    prev->nextSleep=toSleep;
    toSleep->sleepTime-=sum;

}

void Scheduler::getSleep() {
    if(sleepHead){
        sleepHead->sleepTime--;
        while(sleepHead->sleepTime<=0){
            sleepHead->setSuspended(false);
            thread_t temp=sleepHead;
            sleepHead=sleepHead->nextSleep;
            temp->nextSleep= nullptr;
            put(temp);
            if(sleepHead== nullptr) break;
        }
    }
}




