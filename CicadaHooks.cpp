#include "Cicada/Cicada.hpp"
#include "HookEntryPoints.hpp"

namespace Cicada {
    Hook hooks[] = {
        //{ "CDWWndOnCreate",       0x0040EA90, reinterpret_cast<void*>(&CDWWnd::OnCreate),     HookType::CODE, true  },
        { "InitializeFrameTimer", 0x00410d40, reinterpret_cast<void*>(&InitializeFrameTimer), HookType::CODE, false },
        { "origReadEntireFile",   0x0041a0c0, reinterpret_cast<void*>(&ReadEntireFile),       HookType::CODE, true  },
        { "MFC43WinMain",         0x004a21f4, reinterpret_cast<void*>(&DigiMain),             HookType::CODE, true  },
        { "CWnd_WindowProc",      0x004a1bca, reinterpret_cast<void*>(&WndProc),              HookType::CODE, false },
        { "_ftol",                0x004a1e9c, reinterpret_cast<void*>(&_ftol),                HookType::CODE, false },
        { "_EH_prolog",           0x004a20e8, reinterpret_cast<void*>(&_EH_prolog),           HookType::CODE, false },
        { "timeGetTime",          0x004a738c, reinterpret_cast<void*>(&timeGetTime),          HookType::AUTO, true  }
    };

    const size_t hookCount = sizeof(hooks) / sizeof(hooks[0]);
}