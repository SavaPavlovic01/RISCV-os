//
// Created by os on 5/16/22.
//
#include "../h/MemoryAllocator.h"

//MemoryAllocator::MemBlock* MemoryAllocator::start=(MemBlock*)HEAP_START_ADDR;
//bool  MemoryAllocator::created=false;

MemoryAllocator::MemBlock* MemoryAllocator::taken= nullptr;
MemoryAllocator::MemBlock* MemoryAllocator::head=(MemBlock*)(HEAP_START_ADDR);
bool MemoryAllocator::first=true;
void *MemoryAllocator::mem_alloc(size_t size) {
    if(first){
        head=(MemBlock*) HEAP_START_ADDR;
        head->next= nullptr;
        head->size=(size_t)((char*)HEAP_END_ADDR-(char*)HEAP_START_ADDR-sizeof(MemBlock));
        first=false;
    }
    //size je u blokovima
    //1 blok je 64 bajta
    MemoryAllocator::MemBlock* blk=head;
    MemoryAllocator::MemBlock* prev= nullptr;
    for(;blk;blk=blk->next){
        if(blk->size>=size*MEM_BLOCK_SIZE) break;
        prev=blk;
    }
    if(blk){
        size_t offset=size*MEM_BLOCK_SIZE+ (size_t)(sizeof(MemBlock));
        MemBlock* newBlock=(MemBlock*)((char*)blk+offset);
        if(prev)prev->next=newBlock;
        else head=newBlock;
        newBlock->next=blk->next;
        newBlock->size=blk->size-size*MEM_BLOCK_SIZE-(size_t)sizeof(MemBlock);
        blk->size=size*MEM_BLOCK_SIZE;
        if(taken) {
            insertSorted(taken,blk);
        }
        else taken=blk;
    }else{
        return nullptr;
    }
    return (void*)((char*)blk+sizeof(MemBlock));

}

int MemoryAllocator::mem_free(void *addr) {
    MemBlock* start=taken;
    MemBlock* prev= nullptr;
    while(true){
        if(start>addr or start== nullptr) return -1;// nije alocirano
        if((char*)start+sizeof(MemBlock)==addr) {
            //dealociraj
            if(prev) prev->next=start->next;
            else taken= taken->next;
            MemBlock* cur=head;
            MemBlock* freePrev= nullptr;
            for(;cur;cur=cur->next){
                if(start<cur){
                    if(freePrev) freePrev->next=start;
                    else head=start;
                    start->next=cur;
                    tryToJoin(start);
                    tryToJoin(freePrev);
                    return 0;
                }
                freePrev=cur;
            }
            freePrev->next=start;
            start->next= nullptr;
            return 0;
        }
        prev=start;
        start=start->next;
    }

}

int MemoryAllocator::tryToJoin(MemoryAllocator::MemBlock *cur) {
    if (!cur) return 0;
    if (cur->next && (char*)cur+cur->size+sizeof(MemBlock)== (char*)(cur->next)) {

        cur->size += cur->next->size;
        cur->next = cur->next->next;

        return 1;
    } else
        return 0;
}

void MemoryAllocator::insertSorted(MemoryAllocator::MemBlock *listHead, MemoryAllocator::MemBlock *elem) {
    MemBlock* cur=listHead;
    MemBlock* prev= nullptr;
    for(;cur;cur=cur->next){
        if(elem<cur){
            if(prev) prev->next=elem;
            else listHead=elem;
            elem->next=cur;
            return;
        }

        prev=cur;
    }
    prev->next=elem;
    elem->next= nullptr;
}
