#include "string"
#include "mutex"
#include "iostream"
#include <direct.h>

#include "cdmain.h"

#include "patch/global.h"
#include "patch/mp.h"
#include "patch/testdrive.h"
#include "patch/intrface.h"
#include "patch/gameplay.h"

#define CONCAT(a, b) a##b
#define UNIQUE_VAR(name) CONCAT(name, __LINE__)

constexpr unsigned int hash_time()
{
    unsigned int hash = 0;
    const char* time = __TIME__;  // "HH:MM:SS"

    for (int i = 0; time[i] != '\0'; ++i) {
        hash = hash * 31 + time[i];
    }
    return hash;
}

std::mutex mainmutex;

ini::File patchconf = ini::open(std::filesystem::current_path().string() + "\\CrashPatch.ini");

void Init()
{
    injector::WriteMemory<int>(0x485C86, hash_time(), true);

    std::lock_guard<std::mutex> lock(mainmutex);

    bool PlayerIdleConf = patchconf["CrashPatch"].get<int>("PlayerIdleInPTB");
    bool TestDriveRespawnConf = patchconf["CrashPatch"].get<int>("TestDriveRespawn");
    bool SkipTitleConf = patchconf["CrashPatch"].get<int>("SkipTitleScreen");
    bool ShowUserNameConf = patchconf["CrashPatch"].get<int>("ShowUserNameInMainMenu");
    bool SpeedLimiterConf = patchconf["CrashPatch"].get<int>("EnableSpeedLimiter");
    bool SpeedHackConf = patchconf["CrashPatch"].get<int>("AllowSpeedHack");
    bool TestDriveInMultiplayerConf = patchconf["CrashPatch"].get<int>("TestDriveInMultiplayer");
    bool AllowRandomNumPlatesConf = patchconf["CrashPatch"].get<int>("AllowRandomNumPlates");
    bool DynamicSkyBoxConf = patchconf["CrashPatch"].get<int>("DynamicSkyBox");
    bool VignetteDuringGameplayConf = patchconf["CrashPatch"].get<int>("ShowVignetteDuringGameplay");

    /* Handling one-time patches (not requiring any loop) */

    skip_intro = SkipTitleConf;

    if (ShowUserNameConf)
    {
        ShowUserName();
    }

    if (TestDriveInMultiplayerConf)
    {
        TestDriveInMultiplayer();
    }

    if (VignetteDuringGameplayConf)
    {
        VignetteDuringGameplay();
    }

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

    while (true)
    {
        /* Handling looping patches */

        if (PlayerIdleConf)
        {
            PTBPlayerIdle();
        }

        if (TestDriveRespawnConf)
        {
            TestDriveRespawn();
        }

        if (SpeedLimiterConf)
        {
            CarSpeedLimiter();
        }

        if (!SpeedHackConf)
        {
            DisableCarSpeedHack();
        }

        if (AllowRandomNumPlatesConf)
        {
            RandomNumPlates();
        }

        if (DynamicSkyBoxConf)
        {
            DynamicSkyBox();
        }

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
        OilEffect();
        NoDamage();
        MPFinishScreen();
        MPKick();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}