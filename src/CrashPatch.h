#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#include "injector/injector.hpp"

#include "cdmain.h"

#include "patch/global.h"
#include "patch/mp.h"
#include "patch/testdrive.h"
#include "patch/intrface.h"
#include "patch/gameplay.h"

using namespace std::chrono_literals;

LONG WINAPI CrashHandler(EXCEPTION_POINTERS* pExceptionInfo)
{
    std::string dumpPath = CDDir() + "\\minidumps\\";
    std::filesystem::create_directories(dumpPath);

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t time_since_epoch = std::chrono::system_clock::to_time_t(now);
    long long timestamp_int = static_cast<long long>(time_since_epoch);
    std::string filename = dumpPath + "crashpatch_" + std::to_string(timestamp_int) + ".dmp";
    HANDLE hFile = CreateFile(filename.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION minidumpInfo;
        minidumpInfo.ExceptionPointers = pExceptionInfo;
        minidumpInfo.ThreadId = GetCurrentThreadId();
        minidumpInfo.ClientPointers = FALSE;
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &minidumpInfo, NULL, NULL);
        CloseHandle(hFile);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

namespace
{
    // Configuration keys
    constexpr auto CONFIG_SECTION = "CrashPatch";
    constexpr auto KEY_PLAYER_IDLE = "PlayerIdleInPTB";
    constexpr auto KEY_TESTDRIVE_RESPAWN = "TestDriveRespawn";
    constexpr auto KEY_SKIP_TITLE = "SkipTitleScreen";
    constexpr auto KEY_SHOW_USERNAME = "ShowUserNameInMainMenu";
    constexpr auto KEY_SPEED_LIMITER = "EnableSpeedLimiter";
    constexpr auto KEY_SPEED_HACK = "AllowSpeedHack";
    constexpr auto KEY_TESTDRIVE_MP = "TestDriveInMultiplayer";
    constexpr auto KEY_RANDOM_PLATES = "AllowRandomNumPlates";
    constexpr auto KEY_DYNAMIC_SKYBOX = "DynamicSkyBox";
    constexpr auto KEY_VIGNETTE_GAMEPLAY = "ShowVignetteDuringGameplay";
    constexpr auto KEY_OIL_TIRES = "OilOnTires";

    // Default values (all disabled)
    constexpr int DEFAULT_DISABLED = 0;

    // Configuration structure
    struct PatchConfig
    {
        bool playerIdleInPTB = false;
        bool testDriveRespawn = false;
        bool skipTitleScreen = false;
        bool showUserNameInMainMenu = false;
        bool enableSpeedLimiter = false;
        bool allowSpeedHack = false;
        bool testDriveInMultiplayer = false;
        bool allowRandomNumPlates = false;
        bool dynamicSkyBox = false;
        bool showVignetteDuringGameplay = false;
        bool oilOnTires = false;

        static PatchConfig loadFromFile(const std::filesystem::path& configPath)
        {
            PatchConfig cfg;
            try
            {
                // Assume ini::open returns an object compatible with .get<int>(key)
                auto iniFile = ini::open(configPath.string());

                // Helper lambda to read boolean values (0/1) with default
                auto readBool = [&](const char* key, bool defaultValue) -> bool
                {
                    try
                    {
                        return iniFile[CONFIG_SECTION].get<int>(key) != 0;
                    }
                    catch (...)
                    {
                        return defaultValue;
                    }
                };
                
                cfg.playerIdleInPTB = readBool(KEY_PLAYER_IDLE, DEFAULT_DISABLED);
                cfg.testDriveRespawn = readBool(KEY_TESTDRIVE_RESPAWN, DEFAULT_DISABLED);
                cfg.skipTitleScreen = readBool(KEY_SKIP_TITLE, DEFAULT_DISABLED);
                cfg.showUserNameInMainMenu = readBool(KEY_SHOW_USERNAME, DEFAULT_DISABLED);
                cfg.enableSpeedLimiter = readBool(KEY_SPEED_LIMITER, DEFAULT_DISABLED);
                cfg.allowSpeedHack = readBool(KEY_SPEED_HACK, DEFAULT_DISABLED);
                cfg.testDriveInMultiplayer = readBool(KEY_TESTDRIVE_MP, DEFAULT_DISABLED);
                cfg.allowRandomNumPlates = readBool(KEY_RANDOM_PLATES, DEFAULT_DISABLED);
                cfg.dynamicSkyBox = readBool(KEY_DYNAMIC_SKYBOX, DEFAULT_DISABLED);
                cfg.showVignetteDuringGameplay = readBool(KEY_VIGNETTE_GAMEPLAY, DEFAULT_DISABLED);
                cfg.oilOnTires = readBool(KEY_OIL_TIRES, DEFAULT_DISABLED);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Failed to load config: " << e.what() << std::endl;
            }
            return cfg;
        }
    };

    // Compile‑time hash of __TIME__ (original logic preserved)
    constexpr unsigned int hashTime()
    {
        const char* time = __TIME__;   // format "HH:MM:SS"
        unsigned int hash = 0;
        for (int i = 0; time[i] != '\0'; ++i)
            hash = hash * 31 + static_cast<unsigned int>(time[i]);
        return hash;
    }

    // Global flag to allow clean exit (if needed)
    std::atomic<bool> g_running{ true };
}

void Init()
{
    SetUnhandledExceptionFilter(CrashHandler);

    // --- One‑time startup ---
    // Write compile‑time hash to memory (original anti‑debug / obfuscation)
    injector::WriteMemory<int>(0x485C86, hashTime(), true);

    // Load configuration from INI file
    std::filesystem::path configPath = std::filesystem::current_path() / "CrashPatch.ini";
    PatchConfig cfg = PatchConfig::loadFromFile(configPath);

    // Apply one‑time patches
    skip_intro = cfg.skipTitleScreen; 

    if (cfg.showUserNameInMainMenu)
        ShowUserName();

    if (cfg.testDriveInMultiplayer)
        TestDriveInMultiplayer();

    if (cfg.showVignetteDuringGameplay)
        VignetteDuringGameplay();

    // Always‑called one‑time patches (no condition)
    CrshPath();
    FixLaunchParameters();
    NoPauseOnWindowNotFocused();
    ExtendedViewRange();
    NoOppsBombRun();
    FixNotAvailablePTP();
    FixCrash_GarageCurrLine();
    FinishScreenBlinds();
    CarColorSlider();
    CarCulling();
    TrackEditorSlopeLimit();
    ReplayShowHUD();
    LODImprovements();
    TestDriveOptions();
    MasterServerIP();
    HideTrackEditorHelp();
    BombRunHUD();
    FixObjectiveTextNewLineAppend();
    FixAfterRaceFinishPos();
    DamageFromExplosion();
    ColoredCarsOnRadar();
    AirTimeLogic();
    FixCrash_TrackEditorHiddenTileSelect();
    DisableHeightDependentCarRespawn();
    DisableRpmtexDependency();
    SkipEventLoading();
    TestDriveTweaks();
    FixDriverModels();
    ExtendAISetupsLimit();

    // --- Main loop (runs until g_running is set to false) ---
    while (g_running.load(std::memory_order_relaxed))
    {
        // Apply periodic patches (some conditional on config)
        if (cfg.playerIdleInPTB)
            PTBPlayerIdle();

        if (cfg.testDriveRespawn)
            TestDriveRespawn();

        if (cfg.enableSpeedLimiter)
            CarSpeedLimiter();

        if (!cfg.allowSpeedHack)
            DisableCarSpeedHack();

        if (cfg.allowRandomNumPlates)
            RandomNumPlates();

        if (cfg.dynamicSkyBox)
            DynamicSkyBox();

        if (cfg.oilOnTires)
            OilEffect();

        // Always‑called periodic patches
        SaveContentUnlimit();
        ChangeKeyboardLayout();
        VehicleBlastBomb();
        IntroMenu();
        HideUIInMenus();
        EnvSound();
        AllCarsInMP();
        AftBurParams();
        NewRandomSeed();
        FixSelTrackPreview();
        HideUnusedElemsTrkLoad();
        NoDamage();
        MPFinishScreen();
        MPKick();

        // Sleep to reduce CPU usage. Original was 1ms; increased to 10ms.
        // Adjust based on required responsiveness.
        std::this_thread::sleep_for(10ms);
    }
}