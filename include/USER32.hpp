#pragma once
#include "Cicada/Cicada.hpp"
#include <windows.h>

namespace USER32 {

    inline decltype(&::AdjustWindowRect) AdjustWindowRect =
        reinterpret_cast<decltype(&::AdjustWindowRect)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "AdjustWindowRect"));

    inline decltype(&::CloseWindow) CloseWindow =
        reinterpret_cast<decltype(&::CloseWindow)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "CloseWindow"));

    inline decltype(&::CopyRect) CopyRect =
        reinterpret_cast<decltype(&::CopyRect)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "CopyRect"));

    inline decltype(&::DispatchMessageA) DispatchMessageA =
        reinterpret_cast<decltype(&::DispatchMessageA)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "DispatchMessageA"));

    inline decltype(&::DrawTextA) DrawTextA =
        reinterpret_cast<decltype(&::DrawTextA)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "DrawTextA"));

    inline decltype(&::DrawTextW) DrawTextW =
        reinterpret_cast<decltype(&::DrawTextW)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "DrawTextW"));

    inline decltype(&::GetAsyncKeyState) GetAsyncKeyState =
        reinterpret_cast<decltype(&::GetAsyncKeyState)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "GetAsyncKeyState"));

    inline decltype(&::GetFocus) GetFocus =
        reinterpret_cast<decltype(&::GetFocus)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "GetFocus"));

    inline decltype(&::GetMessageA) GetMessageA =
        reinterpret_cast<decltype(&::GetMessageA)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "GetMessageA"));

    inline decltype(&::IntersectRect) IntersectRect =
        reinterpret_cast<decltype(&::IntersectRect)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "IntersectRect"));

    inline decltype(&::MessageBoxA) MessageBoxA =
        reinterpret_cast<decltype(&::MessageBoxA)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "MessageBoxA"));

    inline decltype(&::OffsetRect) OffsetRect =
        reinterpret_cast<decltype(&::OffsetRect)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "OffsetRect"));

    inline decltype(&::PeekMessageA) PeekMessageA =
        reinterpret_cast<decltype(&::PeekMessageA)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "PeekMessageA"));

    inline decltype(&::PostMessageA) PostMessageA =
        reinterpret_cast<decltype(&::PostMessageA)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "PostMessageA"));

    inline decltype(&::RedrawWindow) RedrawWindow =
        reinterpret_cast<decltype(&::RedrawWindow)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "RedrawWindow"));

    inline decltype(&::SetFocus) SetFocus =
        reinterpret_cast<decltype(&::SetFocus)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "SetFocus"));

    inline decltype(&::ShowCursor) ShowCursor =
        reinterpret_cast<decltype(&::ShowCursor)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "ShowCursor"));

    inline decltype(&::ShowWindow) ShowWindow =
        reinterpret_cast<decltype(&::ShowWindow)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "ShowWindow"));

    inline decltype(&::TranslateMessage) TranslateMessage =
        reinterpret_cast<decltype(&::TranslateMessage)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "TranslateMessage"));

    inline decltype(&::UpdateWindow) UpdateWindow =
        reinterpret_cast<decltype(&::UpdateWindow)>(
            Cicada::GetProcFromLoadedModule("USER32.dll", "UpdateWindow"));

} // namespace USER32
