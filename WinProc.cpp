#include <iostream>
#include <main.h>
#include <system/InputSystem.hpp>
#include <fstream>
#include <gl/GL.h>

int g_ViewportWidth = 0;
int g_ViewportHeight = 0;

LRESULT CALLBACK FullWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
		//RegisterForRawInput(hWnd);
		break;
	case WM_COPYDATA:
		std::cout << "[WM_COPYDATA] Data received." << std::endl;
		break;
	case WM_DESTROY:
		std::cout << "[WM_DESTROY] Cleaning up OpenGL context." << std::endl;
		if (g_hGLRC) { wglDeleteContext(g_hGLRC); g_hGLRC = nullptr; }
		if (g_hDC) { ReleaseDC(hWnd, g_hDC); g_hDC = nullptr; }
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		g_ViewportWidth = LOWORD(lParam);
		g_ViewportHeight = HIWORD(lParam);
		//std::cout << "[WndProc] WM_SIZE: " << g_ViewportWidth << "x" << g_ViewportHeight << "\n";
		glViewport(0, 0, g_ViewportWidth, g_ViewportHeight);
		break;
	}
	case WM_SETFOCUS:
		std::cout << "[WM_SETFOCUS] Window gained focus." << std::endl;
		break;
	case WM_ENABLE:
		std::cout << "[WndProc] WM_ENABLE: " << ((BOOL)wParam ? "TRUE" : "FALSE") << "\n";
		break;
	case WM_HELP:
		std::cout << "[WndProc] WM_HELP\n";
		break;
	case WM_PAINT:
	{
		std::cout << "PAINT\n";
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		RenderFrame();
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_SHOWWINDOW:
	{
		BOOL bShow = (BOOL)wParam;
		int nStatus = (int)lParam;

		//std::cout << "[WM_SHOWWINDOW] show=" << (bShow ? "TRUE" : "FALSE")
			//<< ", status=0x" << std::hex << nStatus << std::dec << "\n";

		if (bShow)
		{
			// You could trigger an initial draw here if needed
			InvalidateRect(hWnd, nullptr, TRUE); // Force a WM_PAINT
		}

		return 0;
	}
	case WM_CLOSE:
		std::cout << "[WndProc] WM_CLOSE\n";
		PostQuitMessage(0);
		break;
	case WM_QUERYENDSESSION:
		std::cout << "[WndProc] WM_QUERYENDSESSION\n";
		return TRUE;
	case WM_QUERYOPEN:
		std::cout << "[WndProc] WM_QUERYOPEN\n";
		return TRUE;
	case WM_ERASEBKGND:
		return 1; // Prevent flickering
	case WM_SYSCOLORCHANGE:
		std::cout << "[WndProc] WM_SYSCOLORCHANGE\n";
		break;
	case WM_ENDSESSION:
		std::cout << "[WndProc] WM_ENDSESSION: " << ((BOOL)wParam ? "TRUE" : "FALSE") << "\n";
		break;
		//case WM_CTLCOLOR:
	case 0x0019:
		std::cout << "[WndProc] WM_CTLCOLOR\n";
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	case WM_WININICHANGE:
		std::cout << "[WndProc] WM_WININICHANGE: section=" << (LPCTSTR)lParam << "\n";
		break;
	case WM_DEVMODECHANGE:
		std::cout << "[WndProc] WM_DEVMODECHANGE: device=" << (LPTSTR)lParam << "\n";
		break;
	case WM_FONTCHANGE:
		std::cout << "[WndProc] WM_FONTCHANGE\n";
		break;
	case WM_TIMECHANGE:
		std::cout << "[WndProc] WM_TIMECHANGE\n";
		break;
	case WM_CANCELMODE:
		std::cout << "[WndProc] WM_CANCELMODE\n";
		break;
	case WM_MOUSEACTIVATE:
		std::cout << "[WndProc] WM_MOUSEACTIVATE\n";
		return MA_ACTIVATE;
	case WM_CHILDACTIVATE:
		std::cout << "[WndProc] WM_CHILDACTIVATE\n";
		break;
	case WM_GETMINMAXINFO:
	{
		auto* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
		mmi->ptMinTrackSize.x = 640;
		mmi->ptMinTrackSize.y = 480;

		// Optional: cap max size
		// mmi->ptMaxTrackSize.x = 1280;
		// mmi->ptMaxTrackSize.y = 720;
		return 0;
	}
	case WM_ICONERASEBKGND:
		std::cout << "[WndProc] WM_ICONERASEBKGND\n";
		return 1;
	case WM_SPOOLERSTATUS:
		std::cout << "[WndProc] WM_SPOOLERSTATUS: JobStatus = " << wParam << ", JobsLeft = " << lParam << "\n";
		break;
	case WM_DRAWITEM:
		std::cout << "[WndProc] WM_DRAWITEM\n";
		break;
		//case WM_DRAWITEM + WM_REFLECT_BASE:
	case WM_DRAWITEM + 0xBC00:
		std::cout << "[WndProc] WM_DRAWITEM_REFLECT\n";
		break;
	case WM_MEASUREITEM:
		std::cout << "[WndProc] WM_MEASUREITEM\n";
		break;
		//case WM_MEASUREITEM + WM_REFLECT_BASE:
	case WM_MEASUREITEM + 0xBC00:
		std::cout << "[WndProc] WM_MEASUREITEM_REFLECT\n";
		break;
	case WM_DELETEITEM:
		std::cout << "[WndProc] WM_DELETEITEM\n";
		break;
		//case WM_DELETEITEM + WM_REFLECT_BASE:
	case WM_DELETEITEM + 0xBC00:
	{
		auto* dis = reinterpret_cast<LPDELETEITEMSTRUCT>(lParam);
		std::cout << "[WM_DELETEITEM_REFLECT] Deleting item ID: " << dis->CtlID << "\n";
		break;
	}
	case WM_CHARTOITEM:
	{
		UINT nChar = static_cast<UINT>(wParam);
		UINT nIndex = static_cast<UINT>(wParam);
		std::cout << "[WM_CHARTOITEM] Char: " << nChar << ", Index: " << nIndex << "\n";
		break;
	}
	//case WM_CHARTOITEM + WM_REFLECT_BASE:
	case WM_CHARTOITEM + 0xBC00:
	{
		UINT nChar = static_cast<UINT>(wParam);
		UINT nIndex = static_cast<UINT>(lParam);
		std::cout << "[WM_CHARTOITEM_REFLECT] Char: " << nChar << ", Index: " << nIndex << "\n";
		break;
	}
	case WM_VKEYTOITEM:
	{
		UINT nKey = static_cast<UINT>(wParam);
		UINT nIndex = static_cast<UINT>(wParam);
		std::cout << "[WM_VKEYTOITEM] Key: " << nKey << ", Index: " << nIndex << "\n";
		break;
	}
	//case WM_VKEYTOITEM + WM_REFLECT_BASE:
	case WM_VKEYTOITEM + 0xBC00:
	{
		UINT nKey = static_cast<UINT>(wParam);
		UINT nIndex = static_cast<UINT>(lParam);
		std::cout << "[WM_VKEYTOITEM_REFLECT] Key: " << nKey << ", Index: " << nIndex << "\n";
		break;
	}
	case WM_QUERYDRAGICON:
		return reinterpret_cast<LRESULT>(LoadCursor(NULL, IDC_ARROW));
	case WM_COMPAREITEM:
	{
		int id = static_cast<int>(wParam);
		LPCOMPAREITEMSTRUCT compare = reinterpret_cast<LPCOMPAREITEMSTRUCT>(lParam);
		std::cout << "[WM_COMPAREITEM] Comparing item ID: " << id << "\n";
		break;
	}
	//case WM_COMPAREITEM + WM_REFLECT_BASE:
	case WM_COMPAREITEM + 0xBC00:
	{
		LPCOMPAREITEMSTRUCT compare = reinterpret_cast<LPCOMPAREITEMSTRUCT>(lParam);
		std::cout << "[WM_COMPAREITEM_REFLECT] Reflected item comparison\n";
		break;
	}
	case WM_COMPACTING:
		std::cout << "[WM_COMPACTING] System is compacting memory: wParam=" << wParam << "\n";
		break;
	case WM_NCDESTROY:
		std::cout << "[WndProc] WM_NCDESTROY" << std::endl;
		// Cleanup OpenGL or custom resources if needed
		break;
	case WM_NCHITTEST:
	{
		LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
		//std::cout << "[WndProc] WM_NCHITTEST result: " << hit << "\n";
		return hit; // Don't override unless you want to customize hit zones, this allows users to drag the window.
	}
	case WM_INPUT:
		//HandleRawInput(lParam);
		break;
	case WM_GETDLGCODE:
		std::cout << "[WndProc] WM_GETDLGCODE" << std::endl;
		return DLGC_WANTALLKEYS;  // Let us process all input keys
	case WM_NCLBUTTONUP:
		std::cout << "[WndProc] WM_NCLBUTTONUP" << std::endl;
		break;
	case WM_NCLBUTTONDBLCLK:
		std::cout << "[WndProc] WM_NCLBUTTONDBLCLK" << std::endl;
		break;
	case WM_GETICON:
	{
		return reinterpret_cast<LRESULT>(LoadIcon(nullptr, IDI_APPLICATION));
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
	case WM_TCARD:
	case WM_HSCROLL:
		//case WM_HSCROLL + WM_REFLECT_BASE:
	case WM_HSCROLL + 0xBC00:
	case WM_VSCROLL:
		//case WM_VSCROLL + WM_REFLECT_BASE:
	case WM_VSCROLL + 0xBC00:
	case WM_INITMENU:
	case WM_INITMENUPOPUP:
	case WM_MENUSELECT:
	case WM_MENUCHAR:
	case WM_ENTERIDLE:
	case WM_PARENTNOTIFY:
		//case WM_PARENTNOTIFY + WM_REFLECT_BASE:
	case WM_PARENTNOTIFY + 0xBC00:
	case WM_MDIACTIVATE:
	case WM_RENDERFORMAT:
	case WM_RENDERALLFORMATS:
	case WM_DESTROYCLIPBOARD:
	case WM_DRAWCLIPBOARD:
	case WM_PAINTCLIPBOARD:
	case WM_VSCROLLCLIPBOARD:
	case WM_CONTEXTMENU:
	case WM_SIZECLIPBOARD:
	case WM_ASKCBFORMATNAME:
	case WM_CHANGECBCHAIN:
	case WM_HSCROLLCLIPBOARD:
	case WM_QUERYNEWPALETTE:
	case WM_PALETTECHANGED:
	case WM_PALETTEISCHANGING:
	case WM_DROPFILES:
	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED:
	case WM_STYLECHANGED:
	case WM_STYLECHANGING:
	case WM_SIZING:
	case WM_MOVING:
	case WM_CAPTURECHANGED:
	case WM_DEVICECHANGE:
	default:
		std::cout << "[WndProc] msg=" << msg << " (0x" << std::hex << msg << ")\n";
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}