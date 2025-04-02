#include "system/ExceptionHandler.h"
#include <windows.h>
#include <DbgHelp.h>
#include <cstdio>

#pragma comment(lib, "Dbghelp.lib")

// Dump CPU registers for diagnostics
static void LogRegisters(CONTEXT* ctx)
{
    char regs[512];
    sprintf_s(regs, sizeof(regs),
        "[REGISTERS]\n"
        "EAX = 0x%08X  EBX = 0x%08X  ECX = 0x%08X  EDX = 0x%08X\n"
        "ESI = 0x%08X  EDI = 0x%08X  ESP = 0x%08X  EBP = 0x%08X\n"
        "EIP = 0x%08X  EFLAGS = 0x%08X\n",
        ctx->Eax, ctx->Ebx, ctx->Ecx, ctx->Edx,
        ctx->Esi, ctx->Edi, ctx->Esp, ctx->Ebp,
        ctx->Eip, ctx->EFlags
    );

    OutputDebugStringA(regs);

    FILE* logFile;
    if (fopen_s(&logFile, "crash_log.txt", "a") == 0 && logFile)
    {
        fprintf(logFile, "%s\n", regs);
        fclose(logFile);
    }
}

// Try to resolve source file + line number for crash address
static void LogCrashSourceInfo(EXCEPTION_POINTERS* pException)
{
    DWORD64 addr = (DWORD64)pException->ExceptionRecord->ExceptionAddress;
    IMAGEHLP_LINE64 line = { sizeof(IMAGEHLP_LINE64) };
    DWORD displacement = 0;

    char output[512] = {};

    if (SymGetLineFromAddr64(GetCurrentProcess(), addr, &displacement, &line)) {
        sprintf_s(output, sizeof(output),
            "[CRASH] Resolved location:\n  File: %s\n  Line: %lu\n",
            line.FileName, line.LineNumber);
    }
    else {
        sprintf_s(output, sizeof(output),
            "[CRASH] Could not resolve source (GetLastError = %lu)\n",
            GetLastError());
    }

    OutputDebugStringA(output);

    FILE* logFile;
    if (fopen_s(&logFile, "crash_log.txt", "a") == 0 && logFile)
    {
        fprintf(logFile, "%s\n", output);
        fclose(logFile);
    }
}

// Write minidump to disk
static void WriteMinidump(EXCEPTION_POINTERS* pException)
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

// Main SEH entrypoint
LONG WINAPI TopLevelExceptionHandler(EXCEPTION_POINTERS* pException)
{
    OutputDebugStringA("[CRASH] Top-level SEH triggered!\n");

    char dbg[256];
    sprintf_s(dbg, sizeof(dbg), "[DEBUG] Exception at address: 0x%p\n", pException->ExceptionRecord->ExceptionAddress);
    OutputDebugStringA(dbg);

    LogRegisters(pException->ContextRecord);
    LogCrashSourceInfo(pException);
    WriteMinidump(pException);

    ExitProcess(1); // Don't try to continue, just shut down cleanly
}

// Optional structured logging for manual use
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

    OutputDebugStringA(buffer);

    FILE* logFile;
    if (fopen_s(&logFile, "crash_log.txt", "a") == 0 && logFile)
    {
        fprintf(logFile, "%s\n", buffer);
        fclose(logFile);
    }
}

// Separate function to deliberately raise a crash
void TriggerSEH()
{
    RaiseException(0xE0000002, 0, 0, nullptr); // Will be caught by TopLevelExceptionHandler
}

// Call once during startup
void InitCrashHandler()
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);

    SetUnhandledExceptionFilter(TopLevelExceptionHandler);
    AddVectoredExceptionHandler(1, TopLevelExceptionHandler);

    OutputDebugStringA("[INIT] Crash handler initialized\n");
}
