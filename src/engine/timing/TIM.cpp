#include "include/TIM.h"
#include <system/Shutdown.hpp>

void __fastcall InitializeTimSystem() {
    // g_TimStateByte = *(BYTE*)(ESP + 3);  <-- fastcall uses ECX and EDX, so we assume dummy
    uint8_t g_TimStateByte = 0; // If you control caller, you can pass this in

    // Simulated memory-mapped addresses
    volatile uint8_t* pTimStateByte = reinterpret_cast<volatile uint8_t*>(0x004c94e0);
    volatile uint32_t* pTimCounter = reinterpret_cast<volatile uint32_t*>(0x004c94e4);
    volatile uint32_t* pTimListHead = reinterpret_cast<volatile uint32_t*>(0x004c94e8);
    volatile uint32_t* pTimSomeOtherValue = reinterpret_cast<volatile uint32_t*>(0x004c94ec);

    *pTimStateByte = g_TimStateByte;
    *pTimCounter = 0;
    *pTimListHead = 0;
    *pTimSomeOtherValue = 0;
}

void ShutdownTimSystem(void)

{   //FIX ME
    //FreeFileBuffer(g_TimCounter, g_TimCounter);
    //g_TimCounter = 0;
    //g_TimListHead = 0;
    //_g_TimSomeOtherValue = 0;
    return;
}

void RegisterTimShutdownCallback(void)

{
    SafeRegisterOnExitCallback(ShutdownTimSystem);
    return;
}
