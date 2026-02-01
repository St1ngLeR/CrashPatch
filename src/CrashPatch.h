#include "string"
#include "mutex"
#include <random>
#include "iostream"
#include <direct.h>

#include "cdmain.h"

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

int layout;

float vehicleblast_timer;

float view_range = 10000000.f;

int player_id0;

float old_car_timer;

float finishscreen_posy_start = -0.5f;

float intro;

bool intro_anim;

bool skip_intro;

std::string testdrive_default_settings = "opps=0 damagerealism=0 gamevar=testdrive-default";

bool ui_menus;

char* save_content = static_cast<char*>(malloc(4194304));

bool SaveContentUnlimit_init;

std::string ambsound/*(64, '\0')*/;

int is_ambsound_playing = 0;
int is_ambsound_looped = 0;
int ambsound_to_load = 0;

DWORD p_ambfile;
std::string ambfile/*(1024, '\0')*/;
std::string ambfile_path;

std::string allcarfolders;

bool AllCarsInMP_init;

bool AftBurParams_init;

std::filesystem::path aftbur_fullpath;

double expldam = -200;

int garage_state;

BYTE sound_slots = 128;

std::vector<std::string> numplates_l = { "l_def.tga" };
std::vector<std::string> numplates_s = { "s_def.tga" };
std::vector<std::string> numplates_u = { "u_def.tga" };

int numplate_index;

bool RandomNumPlates_init;

std::random_device rd;

BYTE seltrack_anim;

bool FixSelTrackPreview_init;

const char* oileffect_key = "OILWARNING";
const char* oileffect_section = "gametext/hud.txt";

std::vector<float> old_latforce;
std::vector<float> old_tractionat90;
std::vector<bool> oileffect_init;
std::vector<float> oileffect_latforce;
std::vector<float> oileffect_tractionat90;

const char* nodamage_key = "COUNTERNONE";
const char* nodamage_section = "gametext/general.txt";
const char* nodamage_value = "-1";

bool NoDamage_init;

void __declspec(naked) a_VehicleBlastBomb()
{
    __asm
    {
        mov eax, esi
        push[esp + 0x14]
        call sub_509AA0
        mov eax, [esp + 0x90]
        jmp loc_508FA6

        sub_509AA0 :
        push 0x509AA0
            retn

            loc_508FA6 :
        push 0x508FA6
            retn
    }
}

void __declspec(naked) a_NoOppsBombRun()
{
    __asm
    {
        cmp dl, 01
        je loc_581F1E

        cmp dl, 02
        jne loc_5822DA

        jmp loc_581F1E

        loc_5822DA:
            push 0x5822DA
            retn

        loc_581F1E:
            push 0x581F1E
            retn
    }
}

void __declspec(naked) RepairPlayerCar()
{
    __asm
    {
        push ebx
        mov ebx, [player_id0]
        mov eax, ebx
        pop ebx

        push 0x44BA60
        retn
    }
}

void __declspec(naked) RespawnOnCPPlayerCar()
{
    __asm
    {
        push ebx
        mov ebx, [player_id0]
        mov eax, ebx
        pop ebx

        push 0x507400
        retn
    }
}

void __declspec(naked) a_FinishScreenBlinds()
{
    __asm
    {
        mov edx, 0x6E22E8
        push finishscreen_posy_start
        jmp sub_59FB67

        sub_59FB67 :
        push 0x59FB67
            retn
    }
}

void __declspec(naked) a_ReplayShowHUD()
{
    __asm
    {
        cmp bh, 6
        je loc_5B9FB7
        cmp bh, 7
        je loc_5B9FB7
        jmp loc_5BA02E

        loc_5B9FB7:
            push 0x5B9FB7
            retn

        loc_5BA02E:
            push 0x5BA02E
            retn
    }
}

void __declspec(naked) a_TestDriveOptions()
{
    __asm
    {
    start:
        push ebx
        push ecx
        sub esp, 0x10
        cmp dl, 5
        ja end
        xor eax, eax
        mov al, dl
        
        cmp eax, 0
        je case0
        cmp eax, 1
        je case1
        cmp eax, 2
        je case2
        cmp eax, 3
        je case3
        cmp eax, 4
        je case4
        cmp eax, 5
        je case5

    case0:
    case1:
    case2:
    case5:
        jmp end

    case3:
        jmp loc_58956B

    case4:
        jmp middle

    middle:
        mov eax, 4  // Test Drive
        call sub_57C7B0
        add esp, 0x10
        pop ecx
        pop ebx
        retn

    end:
        add esp, 0x10
        pop ecx
        pop ebx
        retn

    sub_57C7B0:
        push 0x57C7B0
        retn

    loc_58956B:
        push 0x58956B
        retn
    }
}

void __declspec(naked) a_EnvSound()
{
    __asm
    {
        call sub_603DE0

        mov ebx, -1
        mov edx, ds: ambsound
        mov eax, esp
        call sub_69586C
        mov ecx, ds : 0x7DF0AC
        mov edx, eax
        mov eax, ecx
        call sub_6560D0
        mov eax, esp
        xor edx, edx
        call sub_6959C9

        cmp byte ptr [is_ambsound_looped], 1
        jne stop_sound

    main_sound:
        cmp byte ptr [is_ambsound_playing], 1
        je skip_sound

        push 0
        mov ecx, -1
        mov ebx, ds: ambsound
        push 1
        mov eax, ds: 0x7DF0AC
        xor edx, edx
        call sub_657BC0

        mov byte ptr [is_ambsound_playing], 1

    skip_sound:
        jmp loc_5A3F7A

    stop_sound:
        mov byte ptr[is_ambsound_playing], 0
        jmp loc_5A3F7A

    sub_657BC0:
        push 0x657BC0
        retn

    sub_69586C:
        push 0x69586C
        retn

    sub_6560D0:
        push 0x6560D0
        retn

    sub_6959C9:
        push 0x6959C9
        retn

    sub_603DE0:
        push 0x603DE0
        retn

    loc_5A3F7A:
        push 0x5A3F7A
        retn
    }
}

void __declspec(naked) a_EnvSoundHandler()
{
    __asm
    {
        mov dl, [eax + 0x734]
        cmp dl, 0
        jne loop_zero
        mov byte ptr[is_ambsound_looped], 1
        jmp loc_5B5589

    loop_zero:
        mov byte ptr[is_ambsound_looped], 0
        jmp loc_5B5589

    loc_5B5589:
        push 0x5B5589
        retn
    }
}

void __declspec(naked) a_AmbienceFileName()  // a very-very-very hacky way to get ambience filename pointer
{
    __asm
    {
        mov p_ambfile, ecx
        mov ecx, [esi]
        mov edx, eax
        mov eax, ecx

        jmp loc_63BCF6

    loc_63BCF6:
        push 0x63BCF6
        retn
    }
}

void __declspec(naked) a_CarColorSlider()
{
    __asm
    {
        mov ds: 0x795D1C, ah
        mov ds: 0x79620A, ah
        jmp loc_52CE96

    loc_52CE96:
        push 0x52CE96
        retn
    }
}

void __declspec(naked) a_BombRunHUD()
{
    __asm
    {
        mov eax, ds: 0x78D5F0
        call sub_687050
        mov ecx, [eax + 0x70]
        push ecx
        mov eax, ds: 0x78CEE0
        mov eax, [eax + 4]
        mov edx, eax
        mov edx, [edx + 0x97B0]
        jmp loc_5B2696

    sub_687050:
        push 0x687050
        retn

    loc_5B2696:
        push 0x5B2696
        retn
    }
}

void __declspec(naked) a_Test()
{
    __asm
    {
        call sub_504CE0
        mov eax, [esp + 0x9E8]
        fstp dword ptr [esp + 0x9EC]
        call sub_504DA0
        jmp loc_4FAFA0

    sub_504CE0:
        push 0x504CE0
        retn

    sub_504DA0:
        push 0x504DA0
        retn

    loc_4FAFA0:
        push 0x4FAFA0
        retn
    }
}

void __declspec(naked) a_DamageFromExplosion()
{
    __asm
    {
        fld dword ptr [eax + 0x944]
        fmul dword ptr [eax + 0x65F0]
        fmul qword ptr [expldam]
        faddp st(1), st(0)
        jmp loc_5C05C6

    loc_5C05C6:
        push 0x5C05C6
        retn
    }
}

void __declspec(naked) a_CareerBar()
{
    __asm
    {
        call sub_53B4B0
        call sub_511400

        jmp loc_5387C2

    sub_53B4B0:
        push 0x53B4B0
        retn

    sub_511400:
        push 0x511400
        retn

    loc_5387C2:
        push 0x5387C2
        retn
    }
}

void __declspec(naked) a_ColoredCarsOnRadar()
{
    __asm
    {
        mov eax, ds: 0x78CEE0
        mov eax, [ecx + eax + 4]
        mov eax, [eax + 0x68A8]

        mov edi, eax

        jmp loc_5B75A8

    loc_5B75A8:
        push 0x5B75A8
        retn
    }
}

void __declspec(naked) a_GarageState()
{
    __asm
    {
        mov dl, byte ptr [eax + 0x84]
        mov byte ptr [garage_state], dl
        cmp dl, 04
        jmp loc_535C6E

    loc_535C6E:
        push 0x535C6E
        retn
    }
}

void __declspec(naked) a_HideUnusedElemsTrkLoad()
{
    __asm
    {
        call sub_6959C9

        mov eax, ds: 0x7A92F0   // lblTrackName
        mov dl, 0
        call sub_4970E0

        mov eax, ds : 0x7A92F4  // lblAuthor
        mov dl, 0
        call sub_4970E0

        jmp loc_596250

    loc_596250:
        push 0x596250
        retn

    sub_4970E0:
        push 0x4970E0
        retn

    sub_6959C9:
        push 0x6959C9
        retn
    }
}

void __declspec(naked) AirTimeList()
{
    __asm
    {
        cmp byte ptr [eax + 0x6D], 3
        je airtime
        mov eax, ds: [0x78CEE0]
        jmp loc_5AAE7F

    airtime:
        mov eax, ds : [0x78CEE0]
        add eax, esi
        mov eax, [eax + 0x4]
        call sub_5061B0
        push 1
        push 0x7CC780
        push[eax + 0x1D10]
        call sub_484C80
        jmp loc_5AAEB1

    loc_5AAE7F:
        push 0x5AAE7F
        retn

    sub_5061B0:
        push 0x5061B0
        retn

    sub_484C80:
        push 0x484C80
        retn

    loc_5AAEB1:
        push 0x5AAEB1
        retn
    }
}

void __declspec(naked) AirTimeListRanking()
{
    __asm
    {
        mov eax, ds: [0x78D5F0]
        cmp byte ptr [eax + 0x6D], 3
        je airtime
        cmp bh, 01
        jne loc_63E243
        jmp loc_63E190

    airtime:
        mov eax, ds: [0x78CEE0]
        mov eax,[ecx+eax+04]
        call sub_5061B0
        fild dword ptr [eax + 0x1D10]
        mov eax, ds: [0x78CEE0]
        fstp dword ptr [esp+edx+0x80]
        mov eax,[ecx+eax+0x4]
        call sub_5061B0
        fild dword ptr [eax+0x1D14]
        fstp dword ptr [esp+edx+0x20]
        jmp loc_63E10D

    sub_5061B0:
        push 0x5061B0
        retn

    loc_63E10D:
        push 0x63E10D
        retn

    loc_63E243:
        push 0x63E243
        retn

    loc_63E190:
        push 0x63E190
        retn
    }
}

void __declspec(naked) NoDamageSelect()
{
    __asm
    {
        call sub_6959C9

        cmp byte ptr ds: [0x7A87FC], 0
        je loc_5835F9
        jmp nodamage
              
    nodamage:
        mov edx, 4
        mov eax, esi
        mov ebx, -1
        call sub_4B2790

        mov edx, 0x6DCC16
        lea eax, [esp + 0xC30]
        call sub_69586C
        mov ebx, dword ptr [nodamage_key]
        mov edx, dword ptr [nodamage_section]
        mov ecx, eax
        mov eax, ds: [0x7E3110]
        call sub_66F410
        mov ebx, -1
        mov edx, eax
        lea eax, [esp + 0xC40]
        call sub_69586C
        mov ebx, -1
        mov edx, dword ptr [nodamage_value]
        mov edi, eax
        lea eax, [esp + 0xC50]
        call sub_69586C
        mov ebx, edi
        mov edx, eax
        mov eax, esi
        call sub_4B3460
        lea eax, [esp + 0xC50]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0xC40]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0xC30]
        xor edx, edx
        mov ebx, -1
        call sub_6959C9
        jmp loc_58360A
        

    sub_4B2790:
        push 0x4B2790
        retn

    sub_6959C9:
        push 0x6959C9
        retn

    sub_4B3460:
        push 0x4B3460
        retn

    sub_69586C:
        push 0x69586C
        retn

    sub_66F410:
        push 0x66F410
        retn

    loc_5835F9:
        push 0x5835F9
        retn

    loc_58360A:
        push 0x58360A
        retn
    }
}

std::string GetExeDirectory()
{
    char buffer[MAX_PATH];
    DWORD result = GetModuleFileName(NULL, buffer, MAX_PATH);

    if (result == 0)
    {
        return "";
    }

    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

char crshpath_buffer[MAX_PATH];

void* crshpath = static_cast<void*>(malloc(300));
void* crshpath_str = static_cast<void*>(malloc(300));
void* crshpath_str2 = static_cast<void*>(malloc(300));
void* crshpath_root = static_cast<void*>(malloc(16));

std::string crshpath_root_str = "ROOT";

std::string curpth = GetExeDirectory();

void __declspec(naked) a_CrshPath()
{
    __asm
    {
        mov esi, dword ptr ds : [crshpath]
        /*mov eax, esi*/

        jmp finish

    finish:
        push 0x672303
        retn
    }
}

void __declspec(naked) a_SkipEventLoading()
{
    __asm
    {
        jmp loc_5454B6

    loc_5454B6:
        push 0x5454B6
        retn
    }
}

void __declspec(naked) a_VignetteDuringGameplay()
{
    __asm
    {
        push 0xFFFFFF
        push 1
        push 0x3F800000
        mov eax, ds: [0x7CF700]
        push 0x43F10000
        call sub_5EA350
        fld1
        sub esp, 4
        fadd dword ptr [eax + 0x1C]
        mov ebx, 1
        fstp dword ptr [esp]
        mov edx, 0x6E3301   // gfx/camera.tga
        push 0x0C0000000
        mov eax, ds: [0x7CF700]
        push 0x0C0000000
        call sub_60F0A0

        mov eax, [esp + 0x110]

        jmp loc_5A4528

    sub_5A9880:
        push 0x5A9880
        retn

    sub_5EA350:
        push 0x5EA350
        retn

    sub_60F0A0:
        push 0x60F0A0
        retn

    loc_5A4528:
        push 0x5A4528
        retn
    }
}

void ChangeKeyboardLayout()
{
    if (CDControl())
    {
        layout = LOWORD(GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL)));
        injector::WriteMemory<short>(CDControl() + 0x10, layout, true);
        injector::WriteMemory<short>(CDControl() + 0x12, layout, true);
    }
}

void NoPauseOnWindowNotFocused()
{
    injector::WriteMemory<short>(0x5F6465, 0x1B2, true);
}

void VehicleBlastBomb()
{
    if ((GetGameMode() == (BYTE)CDGameMode::MiniGames) && (GetEventType() == (BYTE)CDEventType::minigame_vehicleblast))
    {
        injector::WriteMemory(CDRaceInfo() + 0xB0, injector::ReadMemory<float>(CDRaceInfo() + 0x68, true) - injector::ReadMemory<float>(CDRaceInfo() + 0x84, true), true);

        LOOP_PLAYERS
        {
            if (GetPlayerParam<float>(CDPlayer::CarHealth, player - 1) > 0.f)
            {
                SetPlayerParam<BYTE>(CDPlayer::BombState, 1, player - 1);
            }
            else
            {
                SetPlayerParam<BYTE>(CDPlayer::BombState, 0, player - 1);
            }
        }
    }
    injector::MakeJMP(0x508F9F, a_VehicleBlastBomb, true);
}

void ExtendedViewRange()
{
    injector::WriteMemory(0x53F4B5, view_range, true);
    injector::WriteMemory(0x549E08, view_range, true);
    injector::WriteMemory(0x6D00B8, view_range, true);
    injector::WriteMemory(0x632E1D, view_range, true);
}

void PTBPlayerIdle()
{
    if ((GetGameMode() == (BYTE)CDGameMode::PassTheBomb) && (GetEventType() == (BYTE)CDEventType::ptb_default))
    {
        if (injector::ReadMemory<BYTE>(CDRaceInfo() + 0xAE, true) != 1)
        {
            LOOP_PLAYERS
            {
                SetPlayerParam<int>(CDPlayer::CarMsgControl, 0, player - 1);
                SetPlayerParam<int>((CDPlayer)((int)CDPlayer::CarMsgControl + 0x4), 0, player - 1);
                SetPlayerParam<int>((CDPlayer)((int)CDPlayer::CarMsgControl + 0x8), 0, player - 1);
                SetPlayerParam<int>((CDPlayer)((int)CDPlayer::CarMsgControl + 0xC), 0, player - 1);
            }
            injector::MakeNOP(0x448BF3, 6, true);
        }
        else
        {
            injector::WriteMemory(0x448BF3, 0x6E9AB088, true);
            injector::WriteMemory<short>(0x448BF7, 0, true);
        }
    }
}

void TestDriveRespawn()
{
    if (CDRace())
    {
        if ((GetGameMode() == (BYTE)CDGameMode::TestDrive) && (GetEventType() == (BYTE)CDEventType::testdrive_default))
        {
            if (GetPlayerParam<float>(CDPlayer::CarHealth, 0) > 0.f)
            {
                old_car_timer = GetPlayerParam<float>(CDPlayer::CarTimer, 0);
            }
            else
            {
                if (GetPlayerParam<float>(CDPlayer::CarTimer, 0) >= old_car_timer + 3.f)
                {
                    RepairPlayerCar();
                    RespawnOnCPPlayerCar();
                }
            }
        }
    }
}

void NoOppsBombRun()
{
    injector::MakeJMP(0x581F15, a_NoOppsBombRun, true);
}

void FixNotAvailablePTP()
{
    injector::MakeNOP(0x58ACF1, 6, true);
}

void DetachCarPartsPhysics()    // TODO: fix huge FPS drops
{
    injector::WriteMemory<short>(0x472FFB, 0x1B2, true);    // apply physics
    injector::WriteMemory<float>(0x44759C, 1000.f, true);   // increase detached parts lifetime
}

void FixCrash_GarageCurrLine()
{
    injector::MakeNOP(0x533BA8, 6, true);
}

void FinishScreenBlinds()
{
    injector::MakeJMP(0x59FB60, a_FinishScreenBlinds, true);
}

void CarColorSlider()
{
    injector::MakeJMP(0x52CE90, a_CarColorSlider, true);
}

void CarCulling()
{
    injector::MakeJMP(0x462FB7, 0x4632E2, true);
    injector::MakeNOP(0x462FBC, 1, true);
}

void TrackEditorSlopeLimit()
{
    injector::WriteMemory<short>(0x5943CB, 0x1B2, true);
}

void ReplayShowHUD()
{
    injector::MakeNOP(0x5B88E1, 6, true);
    injector::MakeNOP(0x5A4128, 6, true);
    injector::WriteMemory<BYTE>(0x5B8345, 1, true);
    injector::WriteMemory(0x5B84C9, 50.f, true);
    injector::MakeJMP(0x5BA029, a_ReplayShowHUD, true);
}

void IntroMenu()
{
    injector::WriteMemory<BYTE>(0x542493, 3, true);
    injector::WriteMemory<BYTE>(0x5424C0, 3, true);

    if (skip_intro == false)
    {
        if (injector::ReadMemory<BYTE>(0x796330, true) == 3)
        {
            intro = injector::ReadMemory<float>(injector::ReadMemory<DWORD>(0x78D65C) + 0x284);

            if (KeyPress(0x1C))
            {
                injector::WriteMemory(injector::ReadMemory<DWORD>(0x78D65C, true) + 0x284, -0.5f, true);
                intro_anim = true;
            }
        }

        if (injector::ReadMemory<BYTE>(0x796330, true))
        {
            intro = injector::ReadMemory<float>(injector::ReadMemory<DWORD>(0x78D65C) + 0x284);
        }

        if (intro >= 0.f)
        {
            if (intro_anim == true)
            {
                injector::WriteMemory(0x796330, 4, true);
            }
        }
    }
    else
    {
        injector::MakeJMP(0x54265E, 0x54272E, true);

        if (injector::ReadMemory<BYTE>(0x796330, true) == 3)
        {
            intro = injector::ReadMemory<float>(injector::ReadMemory<DWORD>(0x78D65C) + 0x284);

            if (intro_anim == false)
            {
                injector::WriteMemory(injector::ReadMemory<DWORD>(0x78D65C, true) + 0x284, -0.5f, true);
                intro_anim = true;
            }
        }

        if (injector::ReadMemory<BYTE>(0x796330, true))
        {
            intro = injector::ReadMemory<float>(injector::ReadMemory<DWORD>(0x78D65C) + 0x284);
        }

        if (intro >= 0.f)
        {
            if (intro_anim == true)
            {
                injector::WriteMemory(0x796330, 4, true);
            }
        }
    }
}

void LODImprovements()
{
    //injector::WriteMemory<double>(0x6F23D6, 0, true);
    injector::WriteMemory<double>(0x6F23DE, -100, true);
    injector::MakeNOP(0x5E05EF, 2, true);
    injector::WriteMemory<double>(0x6EA530, 100, true);
}

void TestDriveOptions()
{
    injector::WriteMemory(0x57C195, static_cast<void*>(testdrive_default_settings.data()), true);
    injector::WriteMemory(0x588593, a_TestDriveOptions, true);
    injector::MakeJMP(0x58449F, 0x583DD0, true);
    injector::MakeNOP(0x589D23, 6, true);
}

void HideUIInMenus()
{
    if (*(BYTE*)0x7CF704 == 1)
    {
        if (!CDRace())
        {
            if (KeyHold(0x1D))
            {
                if (KeyHold(0x2A))
                {
                    if (KeyPress(0x23))
                    {
                        ui_menus = !ui_menus;
                    }
                }
            }
        }
        else
        {
            if (ui_menus == true)
            {
                ui_menus = false;
            }
        }

        if (ui_menus == true)
        {
            injector::WriteMemory(0x6ED588, 0.f, true);
        }
        else
        {
            injector::WriteMemory(0x6ED588, 0.002083333442f, true);
        }
    }
}

void MasterServerIP()
{
    injector::WriteMemory<short>(0x5688B6, 0x1B2, true);
}

void HideTrackEditorHelp()
{
    injector::WriteMemory(0x591909, 0, true);
}

void ShowUserName()
{
    injector::WriteMemory<short>(0x5475EF, 0x1B2, true);
}

void SaveContentUnlimit()
{
    if (SaveContentUnlimit_init == false)
    {
        if (*(BYTE*)0x7CF704 == 1)
        {
            injector::WriteMemory(0x6607C2, save_content, true);
            injector::WriteMemory(0x660826, save_content, true);
            injector::WriteMemory(0x660981, save_content, true);
            injector::WriteMemory(0x6609F5, save_content, true);
            injector::WriteMemory(0x660A20, save_content, true);
            injector::WriteMemory(0x660AA1, save_content, true);
            injector::WriteMemory(0x660B11, save_content, true);
            injector::WriteMemory(0x660B2F, save_content, true);
            injector::WriteMemory(0x660BB3, save_content, true);
            injector::WriteMemory(0x660BC9, save_content, true);
            injector::WriteMemory(0x660C6F, save_content, true);
            injector::WriteMemory(0x660D70, save_content, true);
            injector::WriteMemory(0x660E5C, save_content, true);
            injector::WriteMemory(0x660F27, save_content, true);
            injector::WriteMemory(0x660F62, save_content, true);
            injector::WriteMemory(0x660FF5, save_content, true);
            injector::WriteMemory(0x661022, save_content, true);
            injector::WriteMemory(0x6610AB, save_content, true);
            injector::WriteMemory(0x6610D1, save_content, true);
            injector::WriteMemory(0x661140, save_content, true);
            injector::WriteMemory(0x661161, save_content, true);
            injector::WriteMemory(0x661173, save_content, true);
            injector::WriteMemory(0x661191, save_content, true);
            injector::WriteMemory(0x661211, save_content, true);
            injector::WriteMemory(0x661291, save_content, true);
            injector::WriteMemory(0x661311, save_content, true);
            injector::WriteMemory(0x66188C, save_content, true);
            injector::WriteMemory(0x661962, save_content, true);
            injector::WriteMemory(0x661BFF, save_content, true);
            injector::WriteMemory(0x661C1D, save_content, true);
            injector::WriteMemory(0x661C37, save_content, true);

            injector::WriteMemory(0x66109C, save_content + 3, true);
            injector::WriteMemory(0x6617CB, save_content + 3, true);

            injector::WriteMemory(0x660BD4, save_content + 4, true);
            injector::WriteMemory(0x660C32, save_content + 4, true);
            injector::WriteMemory(0x66100D, save_content + 4, true);
            injector::WriteMemory(0x66102A, save_content + 4, true);
            injector::WriteMemory(0x661132, save_content + 4, true);
            injector::WriteMemory(0x661138, save_content + 4, true);
            injector::WriteMemory(0x6616A4, save_content + 4, true);
            injector::WriteMemory(0x661703, save_content + 4, true);
            injector::WriteMemory(0x661741, save_content + 4, true);
            injector::WriteMemory(0x661756, save_content + 4, true);
            injector::WriteMemory(0x6617DA, save_content + 4, true);
            injector::WriteMemory(0x66180E, save_content + 4, true);
            injector::WriteMemory(0x661815, save_content + 4, true);

            injector::WriteMemory(0x661146, save_content + 5, true);
            injector::WriteMemory(0x66181B, save_content + 5, true);

            SaveContentUnlimit_init = true;
        }
    }
}

void EnvSound()
{
    injector::MakeJMP(0x5B5583, a_EnvSoundHandler, true);
    injector::MakeJMP(0x63BCF0, a_AmbienceFileName, true);
    
    try
    {
        if (GetRaceState() != 0)
        {
            ambfile = GetString((void*)p_ambfile);

            if (!ambfile.empty())
            {
                if (!CDDir().empty())
                {
                    ambfile_path = (CDDir() + "\\textures\\ambience\\" + ambfile);

                    if (std::filesystem::exists(ambfile_path))
                    {
                        if (ambsound_to_load == 0)
                        {
                            ambsound = GetAmbEnvSoundFilePath(ambfile_path);
                            ambsound_to_load = 1;
                        }

                        if (!ambsound.empty())
                        {
                            if (std::filesystem::exists(CDDir() + "\\sounds\\" + ambsound))
                            {
                                injector::MakeJMP(0x5A3F75, a_EnvSound, true);
                            }
                            else
                            {
                                injector::WriteMemory(0x5A3F75, 0x05FE66E8, true);
                                injector::WriteMemory<BYTE>(0x5A3F79, 0, true);
                            }
                        }
                        else
                        {
                            ambsound_to_load = 0;
                        }
                    }
                    else
                    {
                        ambsound_to_load = 0;
                    }
                }
                else
                {
                    ambsound_to_load = 0;
                }
            }
            else
            {
                ambsound_to_load = 0;
            }
        }
        else
        {
            ambsound_to_load = 0;
        }
    }
    catch (...) {}
}

void getSubdirs(std::vector<std::string>& output, const std::string& path)
{
    WIN32_FIND_DATA findfiledata;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char fullpath[MAX_PATH];
    GetFullPathName(path.c_str(), MAX_PATH, fullpath, 0);
    std::string fp(fullpath);

    hFind = FindFirstFile((LPCSTR)(fp + "\\*").c_str(), &findfiledata);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0
                && (findfiledata.cFileName[0] != '.'))
            {
                output.push_back(findfiledata.cFileName);
            }
        }
        while (FindNextFile(hFind, &findfiledata) != 0);
    }
}

void AllCarsInMP()
{
    if (AllCarsInMP_init == false)
    {
        if (*(BYTE*)0x7CF704 == 1)
        {
            std::vector<std::string> carfolders;

            getSubdirs(carfolders, CDDir() + "\\trkdata\\cars\\");
            sort(carfolders.begin(), carfolders.end());

            std::stringstream ss;
            for (const auto& carfolder : carfolders)
            {
                ss << carfolder << " ";
            }

            allcarfolders = ss.str();

            transform(allcarfolders.begin(), allcarfolders.end(), allcarfolders.begin(), ::tolower);

            injector::WriteMemory(0x564887, static_cast<void*>(allcarfolders.data()), true);
            AllCarsInMP_init = true;
        }
    }
}

void CarSpeedLimiter()
{
    if (CDRace())
    {
        LOOP_PLAYERS
        {
            float player_curspeed = GetPlayerParam<float>(CDPlayer::CarCurSpeed, player - 1);
            float player_maxspeed = GetPlayerParam<float>(CDPlayer::CarMaxSpeed, player - 1);

            if (fabs(player_curspeed) >= player_maxspeed)
            {
                SetPlayerParam(CDPlayer::CarCurSpeed, player_maxspeed, player - 1);
            }
        }
    }
}

void DisableCarSpeedHack()
{
    if (GetRaceState() >= (BYTE)CDRaceState::InProcess)
    {
        LOOP_PLAYERS
        {
            if (GetPlayerParam<BYTE>(CDPlayer::AutomaticShifting, player - 1) != 0)
            {
                if ((GetCarEngineParam<BYTE>(CDCarEngine::Gear1, player - 1) == 255) && GetPlayerParam<float>(CDPlayer::CarCurSpeed, player - 1) > 0.f)
                {
                    SetCarEngineParam(CDCarEngine::Gear1, 0, player - 1);
                    SetCarEngineParam(CDCarEngine::Gear2, 0, player - 1);
                }
            }
        }
    }
}

void BombRunHUD()
{
    injector::MakeJMP(0x5B266C, a_BombRunHUD, true);
}

void FixObjectiveTextNewLineAppend()
{
    injector::MakeNOP(0x646E1C, 5, true);
}

void FixAfterRaceFinishPos()
{
    injector::MakeNOP(0x63E87D, 4, true);
    injector::MakeNOP(0x63E057, 6, true);
    injector::MakeNOP(0x63E866, 3, true);
    injector::MakeNOP(0x63E096, 6, true);
}

void AftBurParams()
{
    float aftbur_acceleration;
    float aftbur_heatrisespeed;
    float aftbur_heatcooldownspeed;

    if (AftBurParams_init == false)
    {
        if (*(BYTE*)0x7CF704 == 1)
        {
            aftbur_fullpath = (CDDir() + "\\trkdata\\cars\\aftbur.dbs");

            std::ifstream file(aftbur_fullpath);
            std::string line;
            int lineCounter = 0;
            int lineIndex;
            int foundNext = 0;

            if (file.is_open()) {
                while (std::getline(file, line)) {
                    lineCounter++;

                    if (line.find("Next:") != std::string::npos) {
                        foundNext++;
                        lineIndex = lineCounter;
                    }
                    if (foundNext == 1)
                    {
                        if (lineCounter == lineIndex + 5)
                        {
                            aftbur_acceleration = StringToFloat(line.substr(0, line.find("#")));
                            injector::WriteMemory(0x46AA36, aftbur_acceleration, true);
                        }
                        if (lineCounter == lineIndex + 6)
                        {
                            aftbur_heatrisespeed = StringToFloat(line.substr(0, line.find("#")));
                            injector::WriteMemory(0x46AA5E, aftbur_heatrisespeed, true);
                        }
                        if (lineCounter == lineIndex + 7)
                        {
                            aftbur_heatcooldownspeed = StringToFloat(line.substr(0, line.find("#")));
                            injector::WriteMemory(0x46AA88, aftbur_heatcooldownspeed, true);
                        }
                    }
                }
                file.close();

                AftBurParams_init = true;
            }
        }
    }
}

void DamageFromExplosion()
{
    injector::MakeJMP(0x5C05C0, a_DamageFromExplosion, true);
}

void SoundSlotsFix()
{
    injector::MakeJMP(0x657749, 6, true);
    injector::MakeJMP(0x659E39, 6, true);
}

void NewRandomSeed()
{
    if (!CDNetwork())
    {
        unsigned int rd_seed = rd();

        auto time_seed = static_cast<unsigned int>(
            std::chrono::system_clock::now().time_since_epoch().count());

        std::seed_seq seed_seq{ rd_seed, time_seed };
        std::mt19937 generator(seed_seq);
        std::uniform_int_distribution<int> dist;

        injector::WriteMemory(/*0x695BD8*/ 0x695BDE, dist(generator), true);
    }
    else
    {
        injector::WriteMemory(0x695BDE, 0x3039, true);
    }
}

void TestDriveInMultiplayer()
{
    injector::WriteMemory<short>(0x588D32, 0x1B2, true);
    injector::WriteMemory<short>(0x5879A7, 0x1B2, true);
}

void CareerBar()
{
    injector::MakeJMP(0x5387BD, a_CareerBar, true);
    //injector::MakeJMP(0x535C67, a_GarageState, true);
}

void ColoredCarsOnRadar()
{
    injector::MakeNOP(0x5B75A1, 2, true);
    injector::MakeJMP(0x5B75A3, a_ColoredCarsOnRadar, true);
}

void RandomNumPlates()
{
    if (injector::ReadMemory<BYTE>(0x796330, true) == 4) /*if (*(BYTE*)0x7CF704 == 1)*/
    {
        if (!CDDir().empty())
        {
            if (RandomNumPlates_init == false)
            {
                for (const auto& entry : std::filesystem::directory_iterator(CDDir() + "\\textures\\numplate\\"))
                {
                    if (entry.is_regular_file())
                    {
                        std::string filename = entry.path().filename().stem().string();
                        std::string extension = entry.path().extension().string();

                        if ((extension == ".tga") || (extension == ".dds"))
                        {
                            if (filename.rfind("l_", 0) == 0)
                            {
                                numplates_l.push_back(filename + ".tga");
                            }
                            else if (filename.rfind("s_", 0) == 0)
                            {
                                numplates_s.push_back(filename + ".tga");
                            }
                            else if (filename.rfind("u_", 0) == 0)
                            {
                                numplates_u.push_back(filename + ".tga");
                            }
                        }
                    }
                }
                RandomNumPlates_init = true;
            }
        }

        std::mt19937 gen(rd());

        auto SetRandomNumPlate = [&gen](const std::vector<std::string>& vec, DWORD addr, int index = -1)
            {
                if (!vec.empty()) {
                    std::uniform_int_distribution<> dist(0, vec.size() - 1);
                    if (index == -1)
                    {
                        index = dist(gen);
                    }
                    injector::WriteMemory(addr, (vec[index].data()), true);
                }
            };

        SetRandomNumPlate(numplates_l, 0x5D65A7, numplate_index);
        SetRandomNumPlate(numplates_s, 0x5D6584, numplate_index);
        SetRandomNumPlate(numplates_u, 0x5D65B3, numplate_index);

        if (CDNetwork() || ((GetGarageColorType() == 1) && (GetInterfacePage() != "careerselevent.cgf")))
        {
            numplate_index = 0;
        }
        else
        {
            numplate_index = -1;
        }
    }
}

void FixSelTrackPreview()
{
    if (*(BYTE*)0x7CF704 == 1)
    {
        seltrack_anim = injector::ReadMemory<BYTE>(0x7A8AFE, true);

        if (seltrack_anim == 0)
        {
            if (*(DWORD*)0x7CF704 && *(DWORD*)0x7A8D68)
            {
                if (FixSelTrackPreview_init == false)
                {
                    injector::WriteMemory(injector::ReadMemory<DWORD>(0x7A8D64) + 0x184, 1.f, true);
                    injector::WriteMemory(injector::ReadMemory<DWORD>(0x7A8D68) + 0x184, 0.f, true);

                    FixSelTrackPreview_init = true;
                }
            }
        }
        else
        {
            FixSelTrackPreview_init = false;
        }
    }
}

void HideUnusedElemsTrkLoad()
{
    injector::MakeJMP(0x59624B, a_HideUnusedElemsTrkLoad, true);
}

void OilEffect()
{
    if (CDRace())
    {
        old_latforce.resize(GetPlayersCount());
        old_tractionat90.resize(GetPlayersCount());
        oileffect_latforce.resize(GetPlayersCount());
        oileffect_tractionat90.resize(GetPlayersCount());
        oileffect_init.resize(GetPlayersCount());

        LOOP_PLAYERS
        {
            if ((GetPlayerParam<BYTE>((CDPlayer)((int)CDPlayer::Surface01 + 0x1), player - 1) == 2) ||
                (GetPlayerParam<BYTE>((CDPlayer)((int)CDPlayer::Surface02 + 0x1), player - 1) == 2) ||
                (GetPlayerParam<BYTE>((CDPlayer)((int)CDPlayer::Surface03 + 0x1), player - 1) == 2) ||
                (GetPlayerParam<BYTE>((CDPlayer)((int)CDPlayer::Surface04 + 0x1), player - 1) == 2))
            {
                oileffect_init[player - 1] = true;
                oileffect_latforce[player - 1] = 1000.f;
                oileffect_tractionat90[player - 1] = 100.f;

                if (player == 1)
                {
                    ShowHUDUpperMessage(LocStr("gametext/hud.txt", "OILWARNING"), 3);
                }
            }

            if (oileffect_init[player - 1] == true)
            {
                oileffect_latforce[player - 1] -= 0.5f;
                oileffect_tractionat90[player - 1] -= 0.05f;

                if (oileffect_latforce[player - 1] >= old_latforce[player - 1])
                {
                    for (int wheel = 0; wheel <= 3; wheel++)
                    {
                        SetWheelParam<float>(CDWheel::LatForce, oileffect_latforce[player - 1], player - 1, wheel);
                    }
                }
                else
                {
                    for (int wheel = 0; wheel <= 3; wheel++)
                    {
                        SetWheelParam<float>(CDWheel::LatForce, old_latforce[player - 1], player - 1, wheel);
                    }
                }
                if (oileffect_tractionat90[player - 1] >= old_tractionat90[player - 1])
                {
                    for (int wheel = 0; wheel <= 3; wheel++)
                    {
                        SetWheelParam<float>(CDWheel::TractionAt90, oileffect_tractionat90[player - 1], player - 1, wheel);
                    }
                }
                else
                {
                    for (int wheel = 0; wheel <= 3; wheel++)
                    {
                        SetWheelParam<float>(CDWheel::TractionAt90, old_tractionat90[player - 1], player - 1, wheel);
                    }
                }
                if ((oileffect_tractionat90[player - 1] <= old_tractionat90[player - 1]) && (oileffect_latforce[player - 1] <= old_latforce[player - 1]))
                {
                    oileffect_init[player - 1] = false;
                }
            }
            else
            {
                old_latforce[player - 1] = GetWheelParam<float>(CDWheel::LatForce, player - 1, 0);
                old_tractionat90[player - 1] = GetWheelParam<float>(CDWheel::TractionAt90, player - 1, 0);
            }
        }
    }
    if (GetRaceState() == (BYTE)CDRaceState::Countdown)
    {
        LOOP_PLAYERS
        {
            oileffect_init[player - 1] = false;
        }
    }
}

void AirTimeLogic()
{
    injector::MakeJMP(0x5AAE7A, AirTimeList, true);
    injector::MakeJMP(0x63E187, AirTimeListRanking, true);
}

void NoDamage()
{
    if (NoDamage_init == false)
    {
        injector::MakeJMP(0x5835F4, NoDamageSelect, true);
        injector::MakeNOP(0x5DAFBC, 6, true);
        injector::MakeNOP(0x5DAFCC, 6, true);

        NoDamage_init = true;
    }

    LOOP_PLAYERS
    {
        if (GetPlayerParam<float>(CDPlayer::DeformFactor, player - 1) < 0.f)
        {
            SetPlayerParam(CDPlayer::EnergyLoss, 0.f, player - 1);
            SetPlayerParam(CDPlayer::DeformFactor, 0.f, player - 1);
        }
    }
}

void FixCrash_TrackEditorHiddenTileSelect()
{
    injector::WriteMemory(0x4DD25D, 0x86E9, true);
    injector::WriteMemory<BYTE>(0x4DD261, 0, true);
}

void DisableHeightDependentCarRespawn()
{
    //injector::MakeNOP(0x4D7848, 2, true); // disable height limit
    injector::WriteMemory(0x6C246E, -100.f, true);
}

void DisableRpmtexDependency()
{
    injector::WriteMemory(0x5A5DA9, 0x02C1E9, true);
    injector::WriteMemory<BYTE>(0x5A5DAD, 0, true);
    injector::MakeNOP(0x5A5DAE, 4, true);

    injector::MakeNOP(0x64FDD1, 5, true);

    injector::WriteMemory<short>(0x671176, 0xDB31, true);
    injector::MakeNOP(0x671178, 3, true);
}

void CrshPath() // I hate so much how this thing is works but hey, it works!
{
    injector::MakeNOP(0x672386, 5, true);

    injector::WriteMemory(crshpath, crshpath_str, true);
    injector::WriteMemory<int>((int)crshpath + 0x4, 0, true);
    injector::WriteMemory<int>((int)crshpath + 0x8, curpth.length(), true);
    injector::WriteMemory((int)crshpath + 0xC, crshpath_str2, true);
    injector::WriteMemory((int)crshpath + 0x10, crshpath_root, true);

    injector::WriteMemory(crshpath_str, 1, true);
    injector::WriteMemory<int>((int)crshpath_str + 0x4, curpth.length(), true);
    injector::WriteMemory<int>((int)crshpath_str + 0x8, curpth.length(), true);
    WriteString<uint32_t>((uintptr_t)crshpath_str + 0xC, curpth.c_str(), true);

    WriteString<uint32_t>((uintptr_t)crshpath_str2, curpth.c_str(), true);

    injector::WriteMemory(crshpath_root, 1, true);
    injector::WriteMemory<int>((int)crshpath_root + 0x4, crshpath_root_str.length(), true);
    injector::WriteMemory<int>((int)crshpath_root + 0x8, crshpath_root_str.length(), true);
    WriteString<uint32_t>((uintptr_t)crshpath_root + 0xC, crshpath_root_str.c_str(), true);

    injector::MakeJMP(0x6723C3, a_CrshPath, true);
}

void FixLaunchParameters()
{
    injector::WriteMemory(0x6955EC, 0x7E30F9, true);
    injector::MakeNOP(0x695612, 6, true);
}

void SkipEventLoading()
{
    injector::MakeJMP(0x54586F, a_SkipEventLoading, true);
}

void VignetteDuringGameplay()
{
    injector::MakeJMP(0x5A4521, a_VignetteDuringGameplay, true);

    /*injector::WriteMemory(0x5A3F81, 0xAEE9, true);
    injector::WriteMemory<short>(0x5A3F85, 0x9000, true);

    injector::WriteMemory(0x5A4078, 0xFFFF0AE9, true);
    injector::WriteMemory<BYTE>(0x5A407C, 0xFF, true);*/
}

void CarPhysicsIdleFix()    // TODO
{
    if (CDRace())
    {
        LOOP_PLAYERS
        {
            bool CarPhysicsIdleFix_init;
            float old_car_timer;

            if (GetCarEngineParam<float>(CDCarEngine::CurRPM1, player - 1) == GetPlayerParam<float>(CDPlayer::MinRPM, player - 1))
            {
                if (GetPlayerParam<float>(CDPlayer::CarTimer, player - 1) >= old_car_timer + 0.5f)
                {
                    if (CarPhysicsIdleFix_init == false)
                    {
                        SetPlayerParam<BYTE>(CDPlayer::PhysicsHandler, 1, player - 1);
                        if (GetPlayerParam<BYTE>(CDPlayer::PhysicsHandler, player - 1) == 1)
                        {
                            CarPhysicsIdleFix_init = true;
                        }
                    }
                }
            }
            else
            {
                old_car_timer = GetPlayerParam<float>(CDPlayer::CarTimer, player - 1);
                CarPhysicsIdleFix_init = false;
            }
        }
    }
}

void DynamicSkyBox()
{
    if (CDRace())
    {
        Matrix4x4 skybox = rotationToMatrix4x4(0.f, -0.005f * (float)GetRaceTimer(), 0.f);
        WriteMemoryMatrix4x4(CDRender() + 0xFC4, skybox, true);
    }
}

void TestDriveTweaks()
{
    injector::WriteMemory<short>(0x641179, 0x54EB, true);

    injector::WriteMemory<int>(0x63CFAE, 0xFFFF47E9, true);
    injector::WriteMemory<BYTE>(0x63CFB2, 0xFF, true);
}

void Init()
{
    injector::WriteMemory<int>(0x485C86, hash_time(), true);

    ambsound.reserve(8192);
    ambfile.reserve(8192);
    ambfile_path.reserve(8192);

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
    VignetteDuringGameplay();
    TestDriveTweaks();

    while (true)
    {
        /* Handling looping patches */

        if (CDRace())
        {
            if (GetRaceState() == (BYTE)CDRaceState::InProcess)
            {
                player_id0 = Player(0);
            }
        }

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
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}