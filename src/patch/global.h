/* Global patches */

int layout;

float view_range = 10000000.f;

char* save_content = static_cast<char*>(malloc(4194304));   // 4 MB

bool SaveContentUnlimit_init;

BYTE sound_slots = 128;

std::string ambsound;

int is_ambsound_playing = 0;
int is_ambsound_looped = 0;

int p_ambsound;

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

void __declspec(naked) a_GetEnvSound()
{
    __asm
    {
        xor ebx, ebx

        mov eax, [eax + 0x300]
        mov p_ambsound, eax

        lea eax, [esp + 0x54]
        jmp loc_43147C

    loc_43147C:
        push 0x43147C
        retn
    }
}

void __declspec(naked) a_EnvSoundResetInMPVote()
{
    __asm
    {
        mov byte ptr [is_ambsound_playing], 0
        lea eax, [esp + 0x4]
        call sub_69598E

        jmp loc_6474F3

    loc_6474F3:
        push 0x6474F3
        retn

    sub_69598E:
        push 0x69598E
        retn
    }
}

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
        fadd dword ptr[eax + 0x1C]
        mov ebx, 1
        fstp dword ptr[esp]
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

void ExtendedViewRange()
{
    injector::WriteMemory(0x53F4B5, view_range, true);
    injector::WriteMemory(0x549E08, view_range, true);
    injector::WriteMemory(0x6D00B8, view_range, true);
    injector::WriteMemory(0x632E1D, view_range, true);
}

void CarCulling()
{
    injector::MakeJMP(0x462FB7, 0x4632E2, true);
    injector::MakeNOP(0x462FBC, 1, true);
}

void LODImprovements()
{
    //injector::WriteMemory<double>(0x6F23D6, 0, true);
    injector::WriteMemory<double>(0x6F23DE, -100, true);
    injector::MakeNOP(0x5E05EF, 2, true);
    injector::WriteMemory<double>(0x6EA530, 100, true);
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
    ambsound.reserve(8192);

    injector::MakeJMP(0x5B5583, a_EnvSoundHandler, true);
    injector::MakeJMP(0x431476, a_GetEnvSound, true);
    injector::MakeJMP(0x6474EA, a_EnvSoundResetInMPVote, true);

    ambsound = GetString((void*)p_ambsound).data();
    std::transform(ambsound.begin(), ambsound.end(), ambsound.begin(), ::tolower);
    if (ambsound == "none")
    {
        injector::WriteMemory(0x5A3F75, 0x05FE66E8, true);
        injector::WriteMemory<BYTE>(0x5A3F79, 0, true);
    }
    else
    {
        injector::MakeJMP(0x5A3F75, a_EnvSound, true);
    }
}

void SoundSlotsFix()    // TODO
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

void VignetteDuringGameplay()
{
    injector::MakeJMP(0x5A4521, a_VignetteDuringGameplay, true);

    /*injector::WriteMemory(0x5A3F81, 0xAEE9, true);
    injector::WriteMemory<short>(0x5A3F85, 0x9000, true);

    injector::WriteMemory(0x5A4078, 0xFFFF0AE9, true);
    injector::WriteMemory<BYTE>(0x5A407C, 0xFF, true);*/
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

void DynamicSkyBox()
{
    if (CDRace())
    {
        Matrix4x4 skybox = rotationToMatrix4x4(0.f, -0.005f * (float)GetRaceTimer(), 0.f);
        WriteMemoryMatrix4x4(CDRender() + 0xFC4, skybox, true);
    }
}

void ExtendAISetupsLimit()
{
    int ai_setup_limit = 65536; // 256 - old value

    injector::WriteMemory(0x5D5C4E, ai_setup_limit, true);
    injector::WriteMemory(0x5D5C6E, ai_setup_limit, true);

    injector::WriteMemory(0x5D5C44, (ai_setup_limit * 16) + 4, true);
    injector::WriteMemory(0x5D5C69, (ai_setup_limit * 16) + 4, true);
}

void ExtendTexturesLimit()
{
    injector::WriteMemory<BYTE>(0x5E583E, 0xEB, true);
    //injector::MakeNOP(0x5E582D, 2, true);   
}

void FixMenuAmb()
{
    injector::WriteMemory<BYTE>(0x50E492, 0x9E, true);
    injector::WriteMemory<BYTE>(0x50E497, 0xA2, true);
    injector::WriteMemory<BYTE>(0x50E49C, 0xA6, true);
}

const char* garage_amb = "garage.amb.special";

void __declspec(naked) a_CustomizableGarageAmb()
{
    __asm
    {
        mov eax, [ebp + 0xBA]
        cmp byte ptr [eax + 0x14], 1
        jz loc_50E436
        mov ebx, -1
        mov edx, garage_amb
        lea eax, [ebp + 0x9E]
        call sub_69586C
        lea edx, [ebp - 0x342]
        call sub_42DEC0
        lea eax, [ebp + 0x9E]
        xor edx, edx
        call sub_6959C9

        jmp loc_50E436

    loc_50E436:
        push 0x50E436
        retn

    sub_42DEC0:
        push 0x42DEC0
        retn

    sub_69586C:
        push 0x69586C
        retn

    sub_6959C9:
        push 0x6959C9
        retn
    }
}

void CustomizableGarageAmb()
{
    injector::MakeJMP(0x50E379, a_CustomizableGarageAmb, true);
}

void __declspec(naked) a_SecretBonusCash()
{
    __asm
    {
        cmp byte ptr [esp + 0x80], 1
        jne skip
        cmp byte ptr [edi + 0xE2], 0
        jne skip
        add ecx, dword ptr [edi + 0x4C]

    skip:
        cmp byte ptr [esp + 0x84], 0
        jmp loc_57178C

    loc_57178C:
        push 0x57178C
        retn
    }
}

void SecretBonusCash()
{
    injector::MakeJMP(0x571784, a_SecretBonusCash, true);
}