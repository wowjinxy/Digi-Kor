#include <windows.h>   // For MessageBoxA and ExitProcess
#include <math.h>      // For sin, cos, sqrt

extern "C" {

    // Float to long conversion helper (legacy MSVC)
    long __cdecl _ftol(double d) {
        return static_cast<long>(d);
    }

    // x64 variant (if needed, often not used on x86)
    long long __cdecl _ftol2(double d) {
        return static_cast<long long>(d);
    }

    // Stack probing stub (for large stack allocations)
    void* __cdecl _chkstk() {
        return nullptr; // do nothing
    }

    // Optional SEH helpers (stubbed)
    void __cdecl _except_handler3() {}
    void __cdecl _except_handler4() {}

    // Pure virtual function call handler
    //void __cdecl _purecall() {
//#ifdef _DEBUG
        //MessageBoxA(nullptr, "Pure virtual function call!", "FakeCRT", MB_OK | MB_ICONERROR);
//#endif
        //ExitProcess(1);
    //}

    // Floating-point intrinsics (optional)
    double __cdecl _CIsin(double x) { return sin(x); }
    double __cdecl _CIcos(double x) { return cos(x); }
    double __cdecl _CIsqrt(double x) { return sqrt(x); }

    // Run-Time Check for ESP consistency
    //void __cdecl _RTC_CheckEsp() {
        // No-op for patched builds
    //}

    // C++ EH Prolog/Epilog (rarely necessary unless doing SEH manually)
    void __cdecl _EH_prolog() {}
    void __cdecl _EH_epilog() {}

}
