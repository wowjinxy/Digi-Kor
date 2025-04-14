#pragma once

#include <windows.h>
#include <atomic>
#include <iterator>

#ifdef __cplusplus
extern "C" {
#endif

    void* __cdecl GetGameContext();

#ifdef __cplusplus
}
#endif


// Initialization and cleanup
bool InitOpenGL(HWND hWnd);
void CleanupOpenGL();
void RenderFrame();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int RunOpenGLLoop();

#ifdef STANDALONE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow);
#else 
int __stdcall DigiMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow);
#endif

void SetCodePage();

// Declare the function pointer type
using GameLoopFunc = void(__fastcall*)(void*);
using MessageLoopFunc = void(__fastcall*)(void*);

// Declare the external variable
extern GameLoopFunc GameLoopTick;

// Hook replacements
//extern "C" __declspec(dllexport) bool __stdcall MyCWndCreateHook(HWND hWnd);
//extern "C" __declspec(dllexport) bool __stdcall CWndCreateThunk(HWND hWnd);
int __stdcall DigiMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow);

#pragma comment(lib, "opengl32.lib")

#define ASSERT(expr) assert(expr)

extern HWND g_hWnd;
extern HDC g_hDC;
extern HGLRC g_hGLRC;
extern std::atomic<bool> g_OpenGLInitialized;

// Global MFC object replica
struct AppGlobalStruct {
    void** vtable;
    uint8_t pad[0x1C];
    void* gameContext; // 0x20
};

//inline void InitCStringSubsystem() { reinterpret_cast<void(__stdcall*)()>(0x004057c0)(); }
//inline void InitAppAndRegisterCleanup() { reinterpret_cast<void(__stdcall*)()>(0x004065c0)(); }
//inline void InitTimSubsystem() { reinterpret_cast<void(__fastcall*)(uint32_t)>(0x00406780)(0); }
//inline void STUBBED_SYSTEM() { reinterpret_cast<void(__stdcall*)()>(0x00408f00)(); }
inline void FUN_00410890() { reinterpret_cast<void(__cdecl*)()>(0x00410890)(); }
inline void FUN_00410910() { reinterpret_cast<void(__cdecl*)()>(0x00410910)(); }
inline void FUN_00411150() { reinterpret_cast<void(__cdecl*)()>(0x00411150)(); }
inline void _InitializeTimObjectExSystem() { reinterpret_cast<void(__stdcall*)()>(0x00418130)(); }
inline void STUBBED_THUNK() { reinterpret_cast<void(__stdcall*)()>(0x004199a0)(); }
inline void thunk_FUN_004199d0() { reinterpret_cast<void(__stdcall*)()>(0x004199d0)(); }
inline void FUN_0041bc40() { reinterpret_cast<void(__stdcall*)()>(0x0041bc40)(); }
inline void FUN_0041bc80() { reinterpret_cast<void(__stdcall*)()>(0x0041bc80)(); }
inline void FUN_0041c0f0() { reinterpret_cast<void(__stdcall*)()>(0x0041c0f0)(); }
inline void thunk_FUN_0041c140() { reinterpret_cast<void(__stdcall*)()>(0x0041c140)(); }
inline void FUN_0041c160() { reinterpret_cast<void(__stdcall*)()>(0x0041c160)(); }
inline void thunk_FUN_0041c1b0() { reinterpret_cast<void(__stdcall*)()>(0x0041c1b0)(); }
inline void thunk_FUN_0041e6d0() { reinterpret_cast<void(__stdcall*)()>(0x0041e6d0)(); }
inline void FUN_0041eb40() { reinterpret_cast<void(__stdcall*)()>(0x0041eb40)(); }
inline void FUN_00420980() { reinterpret_cast<void(__stdcall*)()>(0x00420980)(); }
inline void FUN_00420b10() { reinterpret_cast<void(__stdcall*)()>(0x00420b10)(); }
inline void FUN_00426470() { reinterpret_cast<void(__stdcall*)()>(0x00426470)(); }
inline void InitializeFontSystem() { reinterpret_cast<void(__stdcall*)()>(0x004289d0)(); }
inline void FUN_00428a40() { reinterpret_cast<void(__stdcall*)()>(0x00428a40)(); }
inline void FUN_00428a90() { reinterpret_cast<void(__stdcall*)()>(0x00428a90)(); }
inline void FUN_00428ba0() { reinterpret_cast<void(__stdcall*)()>(0x00428ba0)(); }
inline void FUN_0042c1a0() { reinterpret_cast<void(__stdcall*)()>(0x0042c1a0)(); }
inline void FUN_0042c1e0() { reinterpret_cast<void(__stdcall*)()>(0x0042c1e0)(); }
inline void FUN_0042c220() { reinterpret_cast<void(__stdcall*)()>(0x0042c220)(); }
inline void thunk_FUN_0042c2e0() { reinterpret_cast<void(__stdcall*)()>(0x0042c2e0)(); }
inline void FUN_0042c440() { reinterpret_cast<void(__stdcall*)()>(0x0042c440)(); }
inline void thunk_FUN_0042c500() { reinterpret_cast<void(__stdcall*)()>(0x0042c500)(); }
inline void thunk_FUN_0042c6e0() { reinterpret_cast<void(__stdcall*)()>(0x0042c6e0)(); }
inline void FUN_0042c960() { reinterpret_cast<void(__stdcall*)()>(0x0042c960)(); }
inline void thunk_FUN_0042c9b0() { reinterpret_cast<void(__stdcall*)()>(0x0042c9b0)(); }
inline void FUN_0042ccc0() { reinterpret_cast<void(__stdcall*)()>(0x0042ccc0)(); }
inline void FUN_0042cee0() { reinterpret_cast<void(__stdcall*)()>(0x0042cee0)(); }
inline void FUN_0042d230() { reinterpret_cast<void(__stdcall*)()>(0x0042d230)(); }
inline void FUN_0042f060() { reinterpret_cast<void(__stdcall*)()>(0x0042f060)(); }
inline void thunk_FUN_0042f0b0() { reinterpret_cast<void(__stdcall*)()>(0x0042f0b0)(); }
inline void FUN_00433560() { reinterpret_cast<void(__stdcall*)()>(0x00433560)(); }
inline void FUN_00434440() { reinterpret_cast<void(__stdcall*)()>(0x00434440)(); }
inline void thunk_FUN_00434490() { reinterpret_cast<void(__stdcall*)()>(0x00434490)(); }
inline void thunk_FUN_00434c50() { reinterpret_cast<void(__stdcall*)()>(0x00434c50)(); }
inline void FUN_00434d00() { reinterpret_cast<void(__stdcall*)()>(0x00434d00)(); }
inline void FUN_004393e0() { reinterpret_cast<void(__stdcall*)()>(0x004393e0)(); }
inline void thunk_FUN_0043a110() { reinterpret_cast<void(__stdcall*)()>(0x0043a110)(); }
inline void thunk_FUN_00443520() { reinterpret_cast<void(__stdcall*)()>(0x00443520)(); }
inline void thunk_FUN_00443620() { reinterpret_cast<void(__stdcall*)()>(0x00443620)(); }
inline void thunk_FUN_00443690() { reinterpret_cast<void(__stdcall*)()>(0x00443690)(); }
inline void thunk_FUN_004436e0() { reinterpret_cast<void(__stdcall*)()>(0x004436e0)(); }
inline void thunk_FUN_00443750() { reinterpret_cast<void(__stdcall*)()>(0x00443750)(); }
inline void thunk_FUN_004437c0() { reinterpret_cast<void(__stdcall*)()>(0x004437c0)(); }
inline void thunk_FUN_00443830() { reinterpret_cast<void(__stdcall*)()>(0x00443830)(); }
inline void FUN_004442f0() { reinterpret_cast<void(__stdcall*)()>(0x004442f0)(); }
inline void FUN_00444350() { reinterpret_cast<void(__stdcall*)()>(0x00444350)(); }
inline void FUN_00446d80() { reinterpret_cast<void(__stdcall*)()>(0x00446d80)(); }
inline void FUN_00446de0() { reinterpret_cast<void(__stdcall*)()>(0x00446de0)(); }
inline void FUN_00449450() { reinterpret_cast<void(__stdcall*)()>(0x00449450)(); }
inline void FUN_004522c0() { reinterpret_cast<void(__stdcall*)()>(0x004522c0)(); }
inline void FUN_00452330() { reinterpret_cast<void(__stdcall*)()>(0x00452330)(); }
inline void FUN_00453540() { reinterpret_cast<void(__stdcall*)()>(0x00453540)(); }
inline void FUN_00453580() { reinterpret_cast<void(__stdcall*)()>(0x00453580)(); }
inline void FUN_00454270() { reinterpret_cast<void(__stdcall*)()>(0x00454270)(); }
inline void FUN_004543d0() { reinterpret_cast<void(__stdcall*)()>(0x004543d0)(); }
inline void FUN_00454620() { reinterpret_cast<void(__stdcall*)()>(0x00454620)(); }
inline void FUN_00454e00() { reinterpret_cast<void(__stdcall*)()>(0x00454e00)(); }
inline void FUN_00454e40() { reinterpret_cast<void(__stdcall*)()>(0x00454e40)(); }
inline void FUN_00454ff0() { reinterpret_cast<void(__stdcall*)()>(0x00454ff0)(); }
inline void FUN_00455ea0() { reinterpret_cast<void(__stdcall*)()>(0x00455ea0)(); }
inline void thunk_FUN_00456850() { reinterpret_cast<void(__stdcall*)()>(0x00456850)(); }
inline void thunk_FUN_00456890() { reinterpret_cast<void(__stdcall*)()>(0x00456890)(); }
inline void thunk_FUN_004568d0() { reinterpret_cast<void(__stdcall*)()>(0x004568d0)(); }
inline void thunk_FUN_00456910() { reinterpret_cast<void(__stdcall*)()>(0x00456910)(); }
inline void thunk_FUN_0045b030() { reinterpret_cast<void(__stdcall*)()>(0x0045b030)(); }
inline void thunk_FUN_0045b800() { reinterpret_cast<void(__stdcall*)()>(0x0045b800)(); }
inline void thunk_FUN_0045b8c0() { reinterpret_cast<void(__stdcall*)()>(0x0045b8c0)(); }
inline void FUN_0045ca70() { reinterpret_cast<void(__stdcall*)()>(0x0045ca70)(); }
inline void LAB_0045cad0() { reinterpret_cast<void(__stdcall*)()>(0x0045cad0)(); }
inline void LAB_0045cbc0() { reinterpret_cast<void(__stdcall*)()>(0x0045cbc0)(); }
inline void LAB_0045de70() { reinterpret_cast<void(__stdcall*)()>(0x0045de70)(); }
inline void LAB_0045ded0() { reinterpret_cast<void(__stdcall*)()>(0x0045ded0)(); }
inline void LAB_0045df10() { reinterpret_cast<void(__stdcall*)()>(0x0045df10)(); }
inline void LAB_0045df50() { reinterpret_cast<void(__stdcall*)()>(0x0045df50)(); }
inline void LAB_0045df90() { reinterpret_cast<void(__stdcall*)()>(0x0045df90)(); }
inline void LAB_0045dfd0() { reinterpret_cast<void(__stdcall*)()>(0x0045dfd0)(); }
inline void LAB_0045e030() { reinterpret_cast<void(__stdcall*)()>(0x0045e030)(); }
inline void LAB_0045e160() { reinterpret_cast<void(__stdcall*)()>(0x0045e160)(); }
inline void LAB_00461760() { reinterpret_cast<void(__stdcall*)()>(0x00461760)(); }
inline void LAB_004617a0() { reinterpret_cast<void(__stdcall*)()>(0x004617a0)(); }
inline void LAB_00461df0() { reinterpret_cast<void(__stdcall*)()>(0x00461df0)(); }
inline void LAB_004620b0() { reinterpret_cast<void(__stdcall*)()>(0x004620b0)(); }
inline void LAB_004620f0() { reinterpret_cast<void(__stdcall*)()>(0x004620f0)(); }
inline void LAB_00462870() { reinterpret_cast<void(__stdcall*)()>(0x00462870)(); }
inline void LAB_00465150() { reinterpret_cast<void(__stdcall*)()>(0x00465150)(); }
inline void LAB_004696e0() { reinterpret_cast<void(__stdcall*)()>(0x004696e0)(); }
inline void LAB_0046bcc0() { reinterpret_cast<void(__stdcall*)()>(0x0046bcc0)(); }
inline void LAB_0046d070() { reinterpret_cast<void(__stdcall*)()>(0x0046d070)(); }
inline void LAB_0046d0b0() { reinterpret_cast<void(__stdcall*)()>(0x0046d0b0)(); }
inline void LAB_0046d0f0() { reinterpret_cast<void(__stdcall*)()>(0x0046d0f0)(); }
inline void LAB_0046d140() { reinterpret_cast<void(__stdcall*)()>(0x0046d140)(); }
inline void LAB_0046d1a0() { reinterpret_cast<void(__stdcall*)()>(0x0046d1a0)(); }
inline void LAB_00473420() { reinterpret_cast<void(__stdcall*)()>(0x00473420)(); }
inline void LAB_00479a10() { reinterpret_cast<void(__stdcall*)()>(0x00479a10)(); }
inline void FUN_00479f60() { reinterpret_cast<void(__stdcall*)()>(0x00479f60)(); }
inline void FUN_0047a3b0() { reinterpret_cast<void(__stdcall*)()>(0x0047a3b0)(); }
inline void FUN_0047a3f0() { reinterpret_cast<void(__stdcall*)()>(0x0047a3f0)(); }
inline void FUN_0047a430() { reinterpret_cast<void(__stdcall*)()>(0x0047a430)(); }
inline void FUN_0047a490() { reinterpret_cast<void(__stdcall*)()>(0x0047a490)(); }
inline void thunk_FUN_0047b3e0() { reinterpret_cast<void(__stdcall*)()>(0x0047b3e0)(); }
inline void FUN_0047c6f0() { reinterpret_cast<void(__stdcall*)()>(0x0047c6f0)(); }
inline void FUN_0047d250() { reinterpret_cast<void(__stdcall*)()>(0x0047d250)(); }
inline void thunk_FUN_0047d710() { reinterpret_cast<void(__stdcall*)()>(0x0047d710)(); }
inline void thunk_FUN_0047da10() { reinterpret_cast<void(__stdcall*)()>(0x0047da10)(); }
inline void thunk_FUN_00481df0() { reinterpret_cast<void(__stdcall*)()>(0x00481df0)(); }
inline void thunk_FUN_00481e90() { reinterpret_cast<void(__stdcall*)()>(0x00481e90)(); }
inline void FUN_004824a0() { reinterpret_cast<void(__stdcall*)()>(0x004824a0)(); }
inline void FUN_00485690() { reinterpret_cast<void(__stdcall*)()>(0x00485690)(); }
inline void FUN_004a1960() { reinterpret_cast<void(__stdcall*)()>(0x004a1960)(); }

using InitFunc = void(*)();

inline void psudo_initterm(InitFunc* begin, InitFunc* end) {
    while (begin < end) {
        if (*begin != nullptr) {
            (*begin)();
        }
        ++begin;
    }
}

extern InitFunc _InitTable[];
extern const size_t _InitTableCount;
