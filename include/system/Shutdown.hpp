#pragma once

typedef void (*ShutdownCallback)();

void SafeRegisterOnExitCallback(ShutdownCallback callback);