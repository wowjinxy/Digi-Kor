#include <wtypes.h>
#include <cstdint>

DWORD __stdcall timeGetTime();

void InitializeFrameTimer(void* timerStruct, uint32_t interval);

struct FrameTimer {
    uint32_t interval;
    bool initialized;
    bool active;
    DWORD startTime;
};

// Somewhere accessible (e.g. time.h or time.cpp)
typedef void(__cdecl* InitializeFrameTimerFunc)(void*, uint32_t);
extern InitializeFrameTimerFunc originalInitializeFrameTimer;
