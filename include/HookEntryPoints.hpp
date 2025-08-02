#pragma once

// 🗃️ Config system
#include "../ConfigINI.h"

// 🎮 Core systems (game-level logic)
#include "main.h"
#include "system/InputSystem.hpp"
#include "system/time.h"
#include "system/CRT.h"
#include "system/ExceptionHandler.h"

// 🈁 Localization & String parsing
#include "localization/ConvertJKParser.h"

// 🖋️ UI (rendering handled by D3D8 shim)
#include "CDWWnd.hpp"
#include "render/TextRenderer.h"
