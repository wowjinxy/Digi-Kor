#pragma once

#include <windows.h>

// Initialize global exception handling (call once in main or early init)

void InitCrashHandler();


void WriteMinidump(EXCEPTION_POINTERS* pException);

// Core crash handler used internally or manually
void HandleStructuredExceptionWithContext(
    const char* file,
    const char* function,
    int line,
    const char* optionalMessage = nullptr
);

// Macro to auto-fill call-site info
#define HANDLE_EXCEPTION(msg)                           \
    do {                                                \
        HandleStructuredExceptionWithContext(           \
            __FILE__, __FUNCTION__, __LINE__, msg);     \
        TriggerSEH();                                   \
    } while(0)
