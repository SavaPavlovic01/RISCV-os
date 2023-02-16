//
// Created by os on 5/16/22.
//

#ifndef PROJECT_BASE_V1_0_MEMORYALLOCATOR_H
#define PROJECT_BASE_V1_0_MEMORYALLOCATOR_H
#include "../lib/hw.h"
#include "../lib/console.h"
class MemoryAllocator{

    struct MemBlock{
        size_t size;
        MemBlock* next;
        MemBlock():size(0),next(nullptr){}
    };

    static MemBlock* head;
    static MemBlock* taken;
    static bool first;
    static int tryToJoin(MemBlock* cur);

    static void insertSorted(MemBlock* listHead,MemBlock* elem);

public:



    static void* mem_alloc(size_t size);

    static int mem_free(void*);
};

#endif //PROJECT_BASE_V1_0_MEMORYALLOCATOR_H
