#pragma once
#include <windows.h>
#include <wingdi.h>
#include <tchar.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif

// Internal placeholder for HDC simulation
struct FakeHDC {
    SDL_Surface* surface;
    TTF_Font* font;
    SDL_Color textColor;
};

static std::unordered_map<std::string, TTF_Font*> fontCache;

HDC WINAPI SDL2CreateCompatibleDC(HDC hdc);
HBITMAP WINAPI SDL2CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT usage, VOID** ppvBits, HANDLE hSection, DWORD offset);
BOOL WINAPI SDL2DeleteDC(HDC hdc);
HGDIOBJ WINAPI SDL2DeleteObject(HGDIOBJ hObject);
HFONT WINAPI SDL2CreateFontA(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight,
	DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
	DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
	DWORD fdwPitchAndFamily, LPCSTR lpszFace);
HFONT WINAPI SDL2CreateFontIndirectA(const LOGFONTA* lplf);
HGDIOBJ WINAPI SDL2SelectObject(HDC hdc, HGDIOBJ h);
COLORREF WINAPI SDL2SetBkColor(HDC hdc, COLORREF color);
int WINAPI SDL2SetBkMode(HDC hdc, int mode);
COLORREF WINAPI SDL2SetTextColor(HDC hdc, COLORREF color);
BOOL WINAPI SDL2GetTextExtentPoint32A(HDC hdc, LPCSTR lpString, int c, LPSIZE psizl);
BOOL WINAPI SDL2GetTextMetricsA(HDC hdc, LPTEXTMETRICA lptm);
BOOL WINAPI SDL2GetObjectA(HGDIOBJ h, int c, LPVOID pv);

#ifdef __cplusplus
}
#endif
