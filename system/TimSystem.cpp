#include "TimSystem.hpp"
#include <cstdint>
#include <cstdio>
#include "Shutdown.hpp"

static uint8_t g_TimStateByte = 0;           // From (param >> 24)
static int g_TimCounter = 0;                 // Incremented by RegisterTim
static struct TimObject* g_TimListHead = nullptr; // First registered TIM
static int g_TimScratch = 0;                 // Unknown purpose, reserved

// Global state
static uint8_t g_TimStateByte = 0;
static int g_TimCounter = 0;
static void* g_TimListHead = nullptr;
static int _g_TimSomeOtherValue = 0;

// TODO: implement TimObject::Reset() — used by LoadTimFromMemory

bool InitializeTimSystem(uint32_t param) {
    g_TimStateByte = (param >> 24) & 0xFF;
    g_TimCounter = 0;
    g_TimListHead = nullptr;
    g_TimScratch = 0;

    printf("[TIM] Initialized. State byte = %02X\n", g_TimStateByte);

    InitializeTimObjectExSystem();

    return true;
}

void ShutdownTimSystem() {
    // Later: clean up loaded TIMs or memory pool
    g_TimListHead = nullptr;
    g_TimCounter = 0;
    g_TimScratch = 0;
    printf("[TIM] Shutdown complete.\n");
}

void InitTimObjectExPool() {
    // TODO: implement pool initialization for TimObjectEx
    // Possibly pre-allocates space for UI overlays or card images
    printf("[TIM] InitTimObjectExPool (unimplemented stub)\n");
}

static uint32_t g_TimObjectPoolFlags = 0;
static void* g_TimObjectPool = nullptr; // ← Eventually map to DAT_004cd0b0

void ShutdownTimObjectExPool() {
    if ((g_TimObjectPoolFlags & 1) == 0) {
        g_TimObjectPoolFlags |= 1;

        // TODO: destruct each object in g_TimObjectPool using TimObject::~TimObject
        printf("[TIM] ShutdownTimObjectExPool (stubbed)\n");
    }
}

void RegisterTimObjectPoolShutdown() {
    SafeRegisterOnExitCallback(ShutdownTimObjectExPool);
    printf("[TIM] Registered ShutdownTimObjectExPool callback\n");
}

void InitializeTimObjectExSystem() {
    InitTimObjectExPool();
    RegisterTimObjectPoolShutdown();
    printf("[TIM] Extended TIM object system initialized.\n");
}
