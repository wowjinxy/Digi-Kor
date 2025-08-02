#pragma once
#include <windows.h>
#include <iostream>
#include <iomanip> // for std::hex and std::dec

class FWinThread {
public:
    HWND m_pMainWnd = nullptr;

    virtual BOOL InitInstance() {
        return TRUE;
    }

    virtual int ExitInstance() {
        return 0;
    }

    virtual int Run() {
        MSG msg = {};
        std::cout << "[Run] Entering message + game loop.\n";

        void* ctx = GetGameContext();

        while (true) {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    std::cout << "[Run] Exiting with code: " << msg.wParam << "\n";
                    return static_cast<int>(msg.wParam);
                }

                if (msg.message != 0x00FF && msg.message != 0x00A0) {
                    std::cout << "[Run] Msg: " << msg.message
                        << " (0x" << std::hex << msg.message << std::dec << ")\n";
                }

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            // Run game logic even when idle
            if (ctx) {
                GameLoopTick(ctx);
            }

            // RenderFrame(); // Rendering handled by D3D8 shim
        }
    }

    virtual ~FWinThread() = default;
};

class FWinApp : public FWinThread {
public:
    HINSTANCE m_hInstance = nullptr;
    HINSTANCE m_hPrevInstance = nullptr;
    LPTSTR m_lpCmdLine = nullptr;
    int m_nCmdShow = 0;
    LPCTSTR m_pszAppName = nullptr;
    LPCTSTR m_pszRegistryKey = nullptr;
    LPCTSTR m_pszExeName = nullptr;
    LPCTSTR m_pszHelpFilePath = nullptr;
    LPCTSTR m_pszProfileName = nullptr;

    FWinApp(LPCTSTR lpszAppName = nullptr) {
        m_pszAppName = lpszAppName;
    }

    virtual BOOL InitApplication() {
        return TRUE;
    }

    virtual BOOL InitInstance() override {
        return FWinThread::InitInstance();
    }

    virtual int ExitInstance() override {
        return FWinThread::ExitInstance();
    }

    virtual int Run() override {
        return FWinThread::Run();
    }

    virtual ~FWinApp() = default;
};

//extern "C" __declspec(dllexport) FWinThread* AfxGetThread();
//extern "C" __declspec(dllexport) FWinApp* AfxGetApp();
