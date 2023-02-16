//
// Created by os on 5/18/22.
//

#ifndef PROJECT_BASE_V1_0_TRAPHANDLE_H
#define PROJECT_BASE_V1_0_TRAPHANDLE_H

#include "../lib/hw.h"

extern "C" void handleSupervisorTrap();

extern "C" void supervisorTrap();

void sysCall();

#endif //PROJECT_BASE_V1_0_TRAPHANDLE_H
