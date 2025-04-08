// GDI-to-SDL2 replacement prototypes
// This file provides stubbed function signatures for replacing GDI functions with SDL2 equivalents.
// Begin implementing one by one using SDL2 (and SDL_ttf where appropriate).

#pragma once

#include "SDL2.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    // CreateCompatibleDC implementation using SDL_Surface as a placeholder
    HDC WINAPI SDL2CreateCompatibleDC(HDC hdc) {
        // We're simulating a memory DC using an SDL surface. You can expand this with real surface logic.
        FakeHDC* fakeDC = new FakeHDC();
        fakeDC->surface = nullptr; // No surface until a bitmap is selected
        return reinterpret_cast<HDC>(fakeDC);
    }

    // CreateDIBSection implementation using SDL_CreateRGBSurfaceWithFormat
    HBITMAP WINAPI SDL2CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT usage, VOID** ppvBits, HANDLE hSection, DWORD offset) {
        if (!pbmi || !ppvBits) return nullptr;

        int width = pbmi->bmiHeader.biWidth;
        int height = abs(pbmi->bmiHeader.biHeight); // Account for top-down vs bottom-up
        int bpp = pbmi->bmiHeader.biBitCount;

        Uint32 pixelFormat = SDL_PIXELFORMAT_ARGB8888; // Default
        if (bpp == 24) pixelFormat = SDL_PIXELFORMAT_RGB24;
        else if (bpp == 32) pixelFormat = SDL_PIXELFORMAT_ARGB8888;
        else if (bpp == 16) pixelFormat = SDL_PIXELFORMAT_RGB565; // fallback

        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, bpp, pixelFormat);
        if (!surface) return nullptr;

        *ppvBits = surface->pixels;

        // Store surface in FakeHDC if passed
        if (hdc) {
            FakeHDC* fakeDC = reinterpret_cast<FakeHDC*>(hdc);
            fakeDC->surface = surface;
        }

        return reinterpret_cast<HBITMAP>(surface);
    }

    // DeleteDC implementation for freeing FakeHDC
    BOOL WINAPI SDL2DeleteDC(HDC hdc) {
        if (!hdc) return FALSE;
        FakeHDC* fakeDC = reinterpret_cast<FakeHDC*>(hdc);
        if (fakeDC->surface) {
            SDL_FreeSurface(fakeDC->surface);
            fakeDC->surface = nullptr;
        }
        delete fakeDC;
        return TRUE;
    }

    // DeleteObject implementation for cleaning up SDL_Surface-backed HBITMAPs
    HGDIOBJ WINAPI SDL2DeleteObject(HGDIOBJ hObject) {
        if (!hObject) return nullptr;
        SDL_Surface* surface = reinterpret_cast<SDL_Surface*>(hObject);
        SDL_FreeSurface(surface);
        return nullptr;
    }

    const char* MapLogicalFontName(const char* name) {
        if (!name) return "arial.ttf";

        if (strcmp(name, "굴림체") == 0 || strcmp(name, "GulimChe") == 0) return "gulim.ttf";
        if (strcmp(name, "바탕체") == 0 || strcmp(name, "BatangChe") == 0) return "batang.ttf";
        if (strcmp(name, "돋움체") == 0 || strcmp(name, "DotumChe") == 0) return "dotum.ttf";
        return name;
    }

    // Font creation and selection using SDL_ttf
    HFONT WINAPI SDL2CreateFontA(INT cHeight, INT cWidth, INT cEscapement, INT cOrientation, INT cWeight,
        DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
        DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
        DWORD fdwPitchAndFamily, LPCSTR lpszFace) {

        int pointSize = abs(cHeight); // Define this first
        const char* mappedName = MapLogicalFontName(lpszFace);

        std::string key = std::string(mappedName) + ":" + std::to_string(pointSize);

        if (fontCache.find(key) == fontCache.end()) {
            TTF_Font* font = TTF_OpenFont(mappedName, pointSize);
            if (!font) return nullptr;
            fontCache[key] = font;
        }

        return reinterpret_cast<HFONT>(fontCache[key]);
    }


    HFONT WINAPI SDL2CreateFontIndirectA(const LOGFONTA* lplf) {
        if (!lplf || !lplf->lfFaceName[0]) return nullptr;

        int pointSize = abs(lplf->lfHeight);
        std::string key = std::string(lplf->lfFaceName) + ":" + std::to_string(pointSize);

        if (fontCache.find(key) == fontCache.end()) {
            TTF_Font* font = TTF_OpenFont(lplf->lfFaceName, pointSize);
            if (!font) return nullptr;
            fontCache[key] = font;
        }

        return reinterpret_cast<HFONT>(fontCache[key]);
    }

    HGDIOBJ WINAPI SDL2SelectObject(HDC hdc, HGDIOBJ h) {
        if (!hdc || !h) return nullptr;

        FakeHDC* fakeDC = reinterpret_cast<FakeHDC*>(hdc);

        // Try casting to SDL_Surface first (for bitmaps)
        SDL_Surface* surf = reinterpret_cast<SDL_Surface*>(h);
        if (surf->format != nullptr && surf->pixels != nullptr) {
            SDL_Surface* oldSurface = fakeDC->surface;
            fakeDC->surface = surf;
            return reinterpret_cast<HGDIOBJ>(oldSurface);
        }

        // Otherwise, treat it as a font
        TTF_Font* newFont = reinterpret_cast<TTF_Font*>(h);
        TTF_Font* oldFont = fakeDC->font;
        fakeDC->font = newFont;
        return reinterpret_cast<HGDIOBJ>(oldFont);
    }

    // Text rendering configuration
    COLORREF WINAPI SDL2SetBkColor(HDC hdc, COLORREF color) {
        // Currently unused but stubbed to match GDI behavior
        // In SDL, background color is usually handled explicitly during rendering
        return color;
    }

    int WINAPI SDL2SetBkMode(HDC hdc, int mode) {
        // Stub implementation: SDL_ttf always renders with transparent background unless rendered to a new surface.
        // GDI uses OPAQUE or TRANSPARENT; here we just store mode for potential use.
        // In future, you could apply a solid background fill to the target surface manually.
        return mode;
    }

    COLORREF WINAPI SDL2SetTextColor(HDC hdc, COLORREF color) {
        if (!hdc) return 0;
        FakeHDC* fakeDC = reinterpret_cast<FakeHDC*>(hdc);
        fakeDC->textColor.r = GetRValue(color);
        fakeDC->textColor.g = GetGValue(color);
        fakeDC->textColor.b = GetBValue(color);
        fakeDC->textColor.a = 255;
        return color;
    }

    // Text measurement
    BOOL WINAPI SDL2GetTextExtentPoint32A(HDC hdc, LPCSTR lpString, int c, LPSIZE psizl) {
        if (!hdc || !lpString || !psizl) return FALSE;
        FakeHDC* fakeDC = reinterpret_cast<FakeHDC*>(hdc);
        if (!fakeDC->font) return FALSE;

        int w = 0, h = 0;
        std::string str(lpString, c);
        if (TTF_SizeText(fakeDC->font, str.c_str(), &w, &h) != 0) return FALSE;

        psizl->cx = w;
        psizl->cy = h;
        return TRUE;
    }

    BOOL WINAPI SDL2GetTextMetricsA(HDC hdc, LPTEXTMETRICA lptm) {
        if (!hdc || !lptm) return FALSE;

        FakeHDC* fakeDC = reinterpret_cast<FakeHDC*>(hdc);
        if (!fakeDC) {
            OutputDebugStringA("[SDL2GetTextMetricsA] fakeDC is null!\n");
            return FALSE;
        }

        if (!fakeDC->font) {
            OutputDebugStringA("[SDL2GetTextMetricsA] font not set in HDC!\n");
            return FALSE;
        }

        int ascent = TTF_FontAscent(fakeDC->font);
        int descent = TTF_FontDescent(fakeDC->font);
        int height = TTF_FontHeight(fakeDC->font);

        memset(lptm, 0, sizeof(TEXTMETRICA));
        lptm->tmAscent = ascent;
        lptm->tmDescent = -descent;
        lptm->tmHeight = height;
        lptm->tmAveCharWidth = height / 2;

        return TRUE;
    }


    BOOL WINAPI SDL2GetObjectA(HGDIOBJ h, int c, LPVOID pv) {
        if (!h || !pv || c <= 0)
            return FALSE;

        TTF_Font* font = reinterpret_cast<TTF_Font*>(h);

        // We don't really have a LOGFONTA from TTF_Font, but you can fake/partial-fill it:
        LOGFONTA* lf = reinterpret_cast<LOGFONTA*>(pv);
        memset(lf, 0, c);

        // Estimate fields from TTF_Font
        lf->lfHeight = TTF_FontHeight(font) * -1; // negative = character height, not cell
        lf->lfWeight = FW_NORMAL; // Could customize if you're storing weights
        lf->lfCharSet = ANSI_CHARSET;
        lf->lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;

        // You can optionally store the face name if you kept it
        // strcpy_s(lf->lfFaceName, LF_FACESIZE, "Arial");

        return TRUE;
    }

#ifdef __cplusplus
}
#endif
