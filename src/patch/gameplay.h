/* Gameplay patches */

float vehicleblast_timer;

bool AftBurParams_init;

std::filesystem::path aftbur_fullpath;

double expldam = -200;

std::vector<std::string> numplates_l = { "l_def.tga" };
std::vector<std::string> numplates_s = { "s_def.tga" };
std::vector<std::string> numplates_u = { "u_def.tga" };

int numplate_index;

bool RandomNumPlates_init;

const char* oileffect_key = "OILWARNING";
const char* oileffect_section = "gametext/hud.txt";

std::vector<float> old_latforce;
std::vector<float> old_tractionat90;
std::vector<bool> oileffect_init;
std::vector<float> oileffect_latforce;
std::vector<float> oileffect_tractionat90;

float oileffect_latforce_subtract = 0.5f;
float oileffect_tractionat90_subtract = 0.05f;

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

void DetachCarPartsPhysics()    // TODO: fix huge FPS drops
{
    injector::WriteMemory<short>(0x472FFB, 0x1B2, true);    // apply physics
    injector::WriteMemory<float>(0x44759C, 1000.f, true);   // increase detached parts lifetime
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
                oileffect_latforce[player - 1] -= oileffect_latforce_subtract * GetGameSpeed();
                oileffect_tractionat90[player - 1] -= oileffect_tractionat90_subtract * GetGameSpeed();

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

void DisableHeightDependentCarRespawn()
{
    //injector::MakeNOP(0x4D7848, 2, true); // disable height limit
    injector::WriteMemory(0x6C246E, -100.f, true);
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
