// Copyright (c) 2020 Rat431
// Copyright (c) 2024 Anonim17PL
#include "main_system.h"
#define WIN32_LEAN_AND_MEAN
//#include <cstdio>

int _stdcall DllMain(void* hModule, unsigned long  ul_reason_for_call, void* lpReserved)
{
    if (ul_reason_for_call == 1) {
        KernelF::InitWrapper(hModule);
        //AllocConsole();
        //freopen_s(&stream,"CONOUT$", "w",stdout);
    }
    return 1;
}