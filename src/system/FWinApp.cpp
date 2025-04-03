#include "system/FWinApp.h"

// Define the global instances (they must not be duplicated)
static FWinThread g_fThreadInstance;
static FWinApp g_fAppInstance;

//extern "C" __declspec(dllexport)
//FWinThread* AfxGetThread() {
    //return &g_fThreadInstance;
//}

extern "C" __declspec(dllexport)
FWinApp* AfxGetApp() {
    return &g_fAppInstance;
}
