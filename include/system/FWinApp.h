#pragma once
#include <windows.h>

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
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return static_cast<int>(msg.wParam);
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

extern "C" __declspec(dllexport) FWinThread* AfxGetThread();
extern "C" __declspec(dllexport) FWinApp* AfxGetApp();
