#include "InitTable.h"
#include "include/TIM.h"
#include <cstdint>

void __fastcall InitTimSubsystem(void)

{
    InitializeTimSystem();
    RegisterTimShutdownCallback();
    return;
}
