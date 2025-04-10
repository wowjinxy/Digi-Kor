#include "Shutdown.hpp"
#include <cstdlib>

void SafeRegisterOnExitCallback(ShutdownCallback callback) {
    atexit(callback);
}
