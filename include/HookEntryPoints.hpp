﻿#pragma once

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

// 🖋️ Rendering + UI
#include "CDWWnd.hpp"
#include "SDL2.hpp"
#include "render/TextRenderer.h"
