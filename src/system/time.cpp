#include <chrono>
#include <wtypes.h>

#include "system/time.h"
#include <iostream>
#include <Detouring.hpp>

DWORD __stdcall timeGetTime()
{
    using namespace std::chrono;
    static auto start = steady_clock::now();
    auto now = steady_clock::now();
    auto elapsed = duration_cast<milliseconds>(now - start);

    // std::cout << "[HOOK] timeGetTime() called — returning " << elapsed.count() << " ms" << std::endl;

    return static_cast<DWORD>(elapsed.count());
}

void InitializeFrameTimer(void* timerStruct, uint32_t interval)
{
    if (*(int*)((int)timerStruct + 0x0C) == 0) {
        *(uint32_t*)((int)timerStruct + 0x04) = interval;
        *(int*)((int)timerStruct + 0x0C) = 1;
        *(int*)((int)timerStruct + 0x10) = 1;
        *(DWORD*)((int)timerStruct + 0x14) = timeGetTime();
        //*(DWORD*)((int)timerStruct + 0x14) = CALL_ORIGINAL(timeGetTime, 0x004A738C);
    }
}
