// MFC_Imports.h
#pragma once

#include <windows.h>

// Forward declarations of MFC base classes
class CObject;
class CCmdTarget;
class CWnd;
class CDocument;
class CView;
class CDocTemplate;
class CFrameWnd;
class CWinApp;
class CWinThread;
class CDaoRecordset;
class CDC;
class CFile;
class CFrameworkWnd;
class CGdiObject;
class CMyControl;
class CPaintDC;
class CRect;
class CString;

// ------------------------------------
// Runtime class information
// ------------------------------------
struct CRuntimeClass {
    LPCSTR m_lpszClassName;
    int m_nObjectSize;
    UINT m_wSchema;
    CObject* (PASCAL* m_pfnCreateObject)();
    CRuntimeClass* m_pBaseClass;

    CObject* CreateObject();
    BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const;

    static CRuntimeClass* PASCAL FromName(LPCSTR lpszClassName);
    static CRuntimeClass* PASCAL FromName(LPCWSTR lpszClassName);
    static CObject* PASCAL CreateObject(LPCSTR lpszClassName);
    static CObject* PASCAL CreateObject(LPCWSTR lpszClassName);
};

// ------------------------------------
// Message maps
// ------------------------------------
struct AFX_MSGMAP_ENTRY;
struct AFX_MSGMAP {
    const AFX_MSGMAP* pBaseMap;
    const AFX_MSGMAP_ENTRY* lpEntries;
};

// ------------------------------------
// Minimal base classes
// ------------------------------------
class CObject {
    // Base for all MFC objects
};

class CCmdTarget : public CObject {
    // Command handling interface
};

class CWnd : public CCmdTarget {
    // Window base class
};

class CDocument : public CCmdTarget {
    // Document base class
};

class CView : public CWnd {
public:
    CDocument* GetDocument() const { return m_pDocument; }

protected:
    CDocument* m_pDocument;
};

class CDocTemplate : public CCmdTarget {
public:
    virtual CDocument* CreateNewDocument() = 0;
    virtual CFrameWnd* CreateNewFrame(CDocument*, CFrameWnd*) = 0;
};

// ------------------------------------
// Creation context structure
// ------------------------------------
struct CCreateContext {
    CRuntimeClass* m_pNewViewClass;
    CDocument* m_pCurrentDoc;
    CDocTemplate* m_pNewDocTemplate;
    CView* m_pLastView;
    CFrameWnd* m_pCurrentFrame;
};

// ------------------------------------
// Global MFC function pointers
// ------------------------------------
inline CCmdTarget* (__cdecl* AfxGetModuleState)() = reinterpret_cast<CCmdTarget * (__cdecl*)()>(0x004a1b3a);
inline CWinThread* (__cdecl* AfxGetThread)() = reinterpret_cast<CWinThread * (__cdecl*)()>(0x004a1b2e);
inline LPCSTR(__cdecl* AfxRegisterWndClass)(UINT, HCURSOR, HBRUSH, HICON) = reinterpret_cast<LPCSTR(__cdecl*)(UINT, HCURSOR, HBRUSH, HICON)>(0x004a1c48);

// ------------------------------------
// Memory management
// ------------------------------------
inline void* (__cdecl* Alloc)(size_t) = reinterpret_cast<void* (__cdecl*)(size_t)>(0x004a1a3e);
inline void (*FreeFileBuffer)(void*) = reinterpret_cast<void(*)(void*)>(0x004a1a26);

// ------------------------------------
// CCmdTarget methods
// ------------------------------------
using CCmdTarget_GetCommandMap_t = const AFX_MSGMAP* (__thiscall*)(CCmdTarget*);
inline CCmdTarget_GetCommandMap_t CCmdTarget_GetCommandMap =
reinterpret_cast<CCmdTarget_GetCommandMap_t>(0x004a1ada);

using CCmdTarget_GetConnectionHook_t = HRESULT(__thiscall*)(CCmdTarget*, const IID&, void**);
inline CCmdTarget_GetConnectionHook_t CCmdTarget_GetConnectionHook =
reinterpret_cast<CCmdTarget_GetConnectionHook_t>(0x004a1aaa);

// ------------------------------------
// CDC methods
// ------------------------------------
using CDC_Attach_t = BOOL(__thiscall*)(CDC*, HDC);
inline CDC_Attach_t CDC_Attach =
reinterpret_cast<CDC_Attach_t>(0x004a1cd8);

using CDC_DeleteDC_t = BOOL(__thiscall*)(CDC*);
inline CDC_DeleteDC_t CDC_DeleteDC =
reinterpret_cast<CDC_DeleteDC_t>(0x004a1ce4);

using CDC_SelectObject_t = HGDIOBJ(__thiscall*)(CDC*, HGDIOBJ);
inline CDC_SelectObject_t CDC_SelectObject =
reinterpret_cast<CDC_SelectObject_t>(0x004a1ccc);

// ------------------------------------
// CFile methods
// ------------------------------------
using CFile_Close_t = void(__thiscall*)(CFile*);
inline CFile_Close_t CFile_Close =
reinterpret_cast<CFile_Close_t>(0x004a1c90);

// ------------------------------------
// CFrameWnd methods
// ------------------------------------
using CFrameWnd_ActivateFrame_t = void(__thiscall*)(CFrameWnd*, int);
inline CFrameWnd_ActivateFrame_t CFrameWnd_ActivateFrame =
reinterpret_cast<CFrameWnd_ActivateFrame_t>(0x004a1b7c);

// ------------------------------------
// CString methods
// ------------------------------------
// CString(LPCSTR)
using CString_ctor_from_LPCSTR_t = CString * (__thiscall*)(CString*, LPCSTR);
inline CString_ctor_from_LPCSTR_t CString_ctor_from_LPCSTR =
reinterpret_cast<CString_ctor_from_LPCSTR_t>(0x004a1d08);  // Ordinal_535

// CString(const CString&)
using CString_ctor_copy_t = CString * (__thiscall*)(CString*, const CString&);
inline CString_ctor_copy_t CString_ctor_copy =
reinterpret_cast<CString_ctor_copy_t>(0x004a1b40);  // Ordinal_537

// CString()
using CString_ctor_default_t = CString * (__thiscall*)(CString*);
inline CString_ctor_default_t CString_ctor_default =
reinterpret_cast<CString_ctor_default_t>(0x004a1a38);  // Ordinal_540


using CString_operator_divide_t = CString & (__thiscall*)(CString*, LPCSTR);
inline CString_operator_divide_t CString_operator_divide =
reinterpret_cast<CString_operator_divide_t>(0x004a1b34);  // Ordinal_860

using CString_operator_assign_858_t = CString & (__thiscall*)(CString*, LPCSTR);
inline CString_operator_assign_858_t CString_operator_assign_858 =
reinterpret_cast<CString_operator_assign_858_t>(0x004a1a44);  // Ordinal_858

// ------------------------------------
// CWinApp methods
// ------------------------------------
using CWinApp_AddToRecentFileList_t = void(__thiscall*)(CWinApp*, LPCTSTR);
inline CWinApp_AddToRecentFileList_t CWinApp_AddToRecentFileList =
reinterpret_cast<CWinApp_AddToRecentFileList_t>(0x004a1a6e);

// ------------------------------------
// CWnd methods
// ------------------------------------
using CWnd_CalcWindowRect_t = void(__thiscall*)(CWnd*, LPRECT, UINT);
inline CWnd_CalcWindowRect_t CWnd_CalcWindowRect =
reinterpret_cast<CWnd_CalcWindowRect_t>(0x004a1c18);

using CWnd_Create_t = BOOL(__thiscall*)(CWnd*, LPCTSTR, LPCTSTR, DWORD, const RECT&, CWnd*, UINT, CCreateContext*);
inline CWnd_Create_t CWnd_Create =
reinterpret_cast<CWnd_Create_t>(0x004a1c24);

// ------------------------------------
// Application entry point
// ------------------------------------
inline int(__cdecl* MFC_WinMain)(HINSTANCE, HINSTANCE, LPSTR, int) = reinterpret_cast<int(__cdecl*)(HINSTANCE, HINSTANCE, LPSTR, int)>(0x004a21f4);
