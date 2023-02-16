//
// Created by os on 5/25/22.
//

#ifndef PROJECT_BASE_V1_0_RISCV_H
#define PROJECT_BASE_V1_0_RISCV_H
#include "../lib/hw.h"

// read register scause
uint64 r_scause();

// write register scause
void w_scause(uint64 scause);

// read register sepc
uint64 r_sepc();

// write register sepc
void w_sepc(uint64 sepc);

// read register stvec
uint64 r_stvec();

// write register stvec
void w_stvec(uint64 stvec);

// read register stval
uint64 r_stval();

// write register stval
void w_stval(uint64 stval);

enum BitMaskSip
{
    SIP_SSIP = (1 << 1),
    SIP_STIP = (1 << 5),
    SIP_SEIP = (1 << 9),
};

// mask set register sip
void ms_sip(uint64 mask);

// mask clear register sip
void mc_sip(uint64 mask);

// read register sip
uint64 r_sip();

// write register sip
void w_sip(uint64 sip);

enum BitMaskSstatus
{
    SSTATUS_SIE = (1 << 1),
    SSTATUS_SPIE = (1 << 5),
    SSTATUS_SPP = (1 << 8),
};

// mask set register sstatus
void ms_sstatus(uint64 mask);

// mask clear register sstatus
void mc_sstatus(uint64 mask);

// read register sstatus
uint64 r_sstatus();

// write register sstatus
void w_sstatus(uint64 sstatus);

#endif //PROJECT_BASE_V1_0_RISCV_H
