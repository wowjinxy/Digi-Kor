﻿#include "Cicada/Cicada.hpp"
#include "HookEntryPoints.hpp"

inline const char* EUCKR(const wchar_t* utf16str) {
    static char buffer[256]; // or allocate
    int len = WideCharToMultiByte(949, 0, utf16str, -1, buffer, sizeof(buffer), nullptr, nullptr); // 949 = EUC-KR
    return len ? buffer : "??";
}

namespace Cicada {

    Hook hooks[] = {
        // Hook table (used by ApplyAllHooks)

        //{ "CDWWndOnCreate",       0x0040EA90, reinterpret_cast<void*>(&CDWWnd::OnCreate),     HookType::CODE, true  },
        { "UpdateInputState",       0x0041fc90, reinterpret_cast<void*>(&UpdateInputState_SDL),     HookType::CODE, true  },
        { "InitializeFrameTimer", 0x00410d40, reinterpret_cast<void*>(&InitializeFrameTimer), HookType::CODE, false },
        { "ConvertJKParser",   0x0043d990, reinterpret_cast<void*>(&ConvertJKParser),       HookType::CODE, true  },
        { "CWnd_WindowProc",      0x004a1bca, reinterpret_cast<void*>(&WndProc),              HookType::CODE, false },
        { "_ftol",                0x004a1e9c, reinterpret_cast<void*>(&_ftol),                HookType::CODE, false },
        { "_EH_prolog",           0x004a20e8, reinterpret_cast<void*>(&_EH_prolog),           HookType::CODE, false },
        //{ "initterm",           0x004a217a, reinterpret_cast<void*>(&StubbedInitializer),           HookType::CODE, true }, Both called too soon to patch this way.
        //{ "WinMain",           0x004a219c, reinterpret_cast<void*>(&DigiMain),           HookType::CODE, true },
        { "SetModuleStateAndCodePage", 0x004a21b4, reinterpret_cast<void*>(&SetCodePage), HookType::CODE, false },
        //{ "ProcessData", 0x0043deb0, reinterpret_cast<void*>(&ProcessData), HookType::CODE, true },
        { "timeGetTime",          0x004a738c, reinterpret_cast<void*>(&timeGetTime),          HookType::AUTO, false  }
    };

    const size_t hookCount = sizeof(hooks) / sizeof(hooks[0]); 

    // === Call-site hook declarations ===
    //CICADA_DECLARE_HOOK(HDC, WINAPI, SDL2CreateCompatibleDC, (HDC hdc));
    //CICADA_DECLARE_HOOK(HBITMAP, WINAPI, SDL2CreateDIBSection, (HDC hdc, const BITMAPINFO* pbmi, UINT usage, VOID** ppvBits, HANDLE hSection, DWORD offset));
    //CICADA_DECLARE_HOOK(HGDIOBJ, WINAPI, SDL2DeleteObject, (HGDIOBJ hObject));
    //CICADA_DECLARE_HOOK(HFONT, WINAPI, SDL2CreateFontA, (INT, INT, INT, INT, INT, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR));
    //CICADA_DECLARE_HOOK(HFONT, WINAPI, SDL2CreateFontIndirectA, (const LOGFONTA*));
    //CICADA_DECLARE_HOOK(BOOL, WINAPI, SDL2DeleteDC, (HDC hdc));
    //CICADA_DECLARE_HOOK(HGDIOBJ, WINAPI, SDL2SelectObject, (HDC, HGDIOBJ));
    //CICADA_DECLARE_HOOK(COLORREF, WINAPI, SDL2SetBkColor, (HDC, COLORREF));
    //CICADA_DECLARE_HOOK(int, WINAPI, SDL2SetBkMode, (HDC, int));
    //CICADA_DECLARE_HOOK(int, WINAPI, SDL2GetObjectA, (HGDIOBJ, int, LPVOID));
    //CICADA_DECLARE_HOOK(COLORREF, WINAPI, SDL2SetTextColor, (HDC, COLORREF));
    //CICADA_DECLARE_HOOK(BOOL, WINAPI, SDL2GetTextExtentPoint32A, (HDC, LPCSTR, int, LPSIZE));
    //CICADA_DECLARE_HOOK(BOOL, WINAPI, SDL2GetTextMetricsA, (HDC, LPTEXTMETRICA));
    //CICADA_DECLARE_HOOK(void, __fastcall, ConvertJKParser, (void* thisPtr, void* dummy, const char* filename));

	void RegisterCallsiteHooks() {
		// Register callsite hooks for GDI functions
        //CICADA_REGISTER_PTRPATCH("CreateCompatibleDC [Call 1]", 0x00428ee5, SDL2CreateCompatibleDC);
        //CICADA_REGISTER_PTRPATCH("CreateCompatibleDC [Indirect]", 0x004A55C8, SDL2CreateCompatibleDC);
        //CICADA_REGISTER_PTRPATCH("CreateCompatibleDC [Call 2]", 0x00495ee4, SDL2CreateCompatibleDC);
        //CICADA_REGISTER_PTRPATCH("CreateCompatibleDC [Indirect 2]", 0x004A7034, SDL2CreateCompatibleDC);
        //CICADA_REGISTER_PTRPATCH("CreateDIBSection [Call 1]", 0x00495f07, SDL2CreateDIBSection);
        //CICADA_REGISTER_PTRPATCH("CreateDIBSection [Call 2]", 0x00496172, SDL2CreateDIBSection);
        //CICADA_REGISTER_PTRPATCH("CreateFontA", 0x00485abc, SDL2CreateFontA);
        //CICADA_REGISTER_PTRPATCH("CreateFontA IndirectPtr", 0x004A7038, SDL2CreateFontA);
        //CICADA_REGISTER_PTRPATCH("DeleteObject [Call 1]", 0x0040ea57, SDL2DeleteObject);
        //CICADA_REGISTER_PTRPATCH("DeleteObject [Call 2]", 0x00492eea, SDL2DeleteObject);
        //CICADA_REGISTER_PTRPATCH("DeleteObject [Call 3]", 0x00495b9b, SDL2DeleteObject);
        //CICADA_REGISTER_PTRPATCH("DeleteObject [Call 4]", 0x0049619a, SDL2DeleteObject);
        //CICADA_REGISTER_PTRPATCH("CreateFontIndirectA", 0x00492f26, SDL2CreateFontIndirectA);
        //CICADA_REGISTER_PTRPATCH("DeleteDC", 0x00495ba9, SDL2DeleteDC);
        //CICADA_REGISTER_PTRPATCH("SelectObject [PTR 1]", 0x00495b63, SDL2SelectObject);
        //CICADA_REGISTER_PTRPATCH("SelectObject [PTR 2]", 0x00495e01, SDL2SelectObject);
        //CICADA_REGISTER_PTRPATCH("SelectObject [PTR 3]", 0x00495f46, SDL2SelectObject);
        //CICADA_REGISTER_PTRPATCH("SelectObject [PTR 4]", 0x0049601c, SDL2SelectObject);
        //CICADA_REGISTER_PTRPATCH("SelectObject [PTR 5]", 0x0049618c, SDL2SelectObject);
        //CICADA_REGISTER_PTRPATCH("SetBkColor", 0x00495f29, SDL2SetBkColor);
        //CICADA_REGISTER_PTRPATCH("SetBkMode", 0x00495f1f, SDL2SetBkMode);
        //CICADA_REGISTER_PTRPATCH("GetObjectA", 0x0048673f, SDL2GetObjectA);
        //CICADA_REGISTER_PTRPATCH("GetObjectA [indirect]", 0x004A7030, SDL2GetObjectA);
        //CICADA_REGISTER_PTRPATCH("GetTextExtentPoint32A", 0x004291b2, SDL2GetTextExtentPoint32A);
        //CICADA_REGISTER_PTRPATCH("GetTextMetricsA", 0x00428ffd, SDL2GetTextMetricsA);
        //CICADA_REGISTER_PTRPATCH("GetTextMetricsA [Indirect]", 0x004A703C, SDL2GetTextMetricsA);
        //CICADA_REGISTER_PTRPATCH("SetTextColor", 0x00495f37, SDL2SetTextColor);
        //CICADA_REGISTER_PTRPATCH("ConvertJKParser", 0x00429531, ConvertJKParser);
    }

#define EN

    void RegisterStringPatches() {
        // Example patch:
        // PATCH_STRING(0x004A9000, "Digital Adventure: Reborn");
#ifdef EN
        static const char* DummyOP = "movie\\dummy.wmv";
        static const char* QuitGameLabel = "Quit Game";
        static const char* ContinueGameLabel = "Continue Game";
        static const char* NewGameLabel = "New Game";
#else
        static const char* ContinueGameLabel = "이 어 서 하 기";
#endif // TEXT_FIXED

        PATCH_STRING(0x004070eb, DummyOP);
        PATCH_STRING(0x0040e72e, QuitGameLabel);
        PATCH_STRING(0x0040e71a, ContinueGameLabel);
        PATCH_STRING(0x0040e706, NewGameLabel);
    }
} // namespace Cicada
