#include "system/ExceptionHandler.h"
#include <windows.h>
#include <DbgHelp.h>
#include <cstdio>

// Internal function to write a minidump on crash
void WriteMinidump(EXCEPTION_POINTERS* pException)
{
    HANDLE hFile = CreateFileA("crash.dmp", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = GetCurrentThreadId();
        mdei.ExceptionPointers = pException;
        mdei.ClientPointers = FALSE;

        MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hFile,
            MiniDumpWithDataSegs,
            &mdei,
            nullptr,
            nullptr
        );

        CloseHandle(hFile);
    }
}

// The function Windows calls when a crash happens
LONG WINAPI TopLevelExceptionHandler(EXCEPTION_POINTERS* pException)
{
    OutputDebugStringA("[CRASH] Top-level structured exception handler triggered\n");
    WriteMinidump(pException);
    return EXCEPTION_EXECUTE_HANDLER;
}

// Call this early in your application (main, WinMain, DllMain, etc)
void InitCrashHandler()
{
    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
    OutputDebugStringA("[INIT] Crash handler initialized\n");
}

// You can call this manually or let HANDLE_EXCEPTION() macro do it
void HandleStructuredExceptionWithContext(
    const char* file,
    const char* function,
    int line,
    const char* optionalMessage)
{
    char buffer[1024];
    sprintf_s(buffer, sizeof(buffer),
        "\n[CRASH] Structured exception caught!\n"
        "  File:     %s\n"
        "  Function: %s\n"
        "  Line:     %d\n"
        "  Message:  %s\n",
        file,
        function,
        line,
        optionalMessage ? optionalMessage : "(none)");

    // Print to debugger
    OutputDebugStringA(buffer);

    // Also log to file
    FILE* logFile;
    if (fopen_s(&logFile, "crash_log.txt", "a") == 0 && logFile)
    {
        fprintf(logFile, "%s\n", buffer);
        fclose(logFile);
    }

    // Raise a software exception to trigger SEH (which may trigger minidump)
    RaiseException(0xE0000002, EXCEPTION_NONCONTINUABLE, 0, nullptr);
}
