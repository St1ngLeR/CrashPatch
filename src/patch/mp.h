/* Multiplayer-related patches */

std::string allcarfolders;

bool AllCarsInMP_init;

std::string continuerace_key = "QUESTIONCONTINUERACE";
std::string continuerace_section = "CrashPatch.hud";

float mpvote_btn_x1 = -80.f;
float mpvote_btn_x2 = 80.f;

int mpvote_btns = 1;

float waithost_1; // animation timer
float waithost_2 = 1.f; // alpha
float waithost_3 = 400.f;   // pos Y    // 350.f
float waithost_4;   // pos X - center of window

std::string waithost_string;

std::string gui_mplobby = "lobby2.cgf";

std::string btn_kick = "btnKickPlayer";
int btn_kick_ptr = 0x7A7744;    // replacing unused "lblNextOnlineMode" element on "Kick Player" button

std::string dlg_kick_key = "gametext/netlobby.txt";
std::string dlg_kick_section = "LOBBYKICKPLAYERCONFIRM";

std::string dlg_kick_no;
std::string dlg_kick_yes;
std::string dlg_kick_text;

void __declspec(naked) MPVoting_btns()
{
    __asm
    {
        cmp byte ptr [mpvote_btns], 0
        jne main

        push 0x3F800000
        push 0x461C4000
        push waithost_1
        push waithost_2
        push 0xFFA800
        push waithost_string
        mov ecx, 5
        mov ebx, 1
        push waithost_3
        mov edx, 1
        push waithost_4
        mov eax, 2
        push 0
        call sub_482930

        jmp loc_5B5783

    main:
        push [eax+0x744]
        mov ebx,0x6E4FFB    // RACEENDCONTINUE
        mov edx,0x6E500B    // gametext/hud.txt
        mov eax, ds: [0x7E3110]
        push [ebp+0x62]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[ebp-0xE]
        call sub_69586C
        call sub_695B53
        push eax
        push 0x41C00000
        push 0x430C0000
        mov eax,ds: [0x7CF700]
        push 0x43C80000
        call sub_5EA350
        fld dword ptr [eax+0x1C]
        fmul dword ptr ds: [0x6E518A]
        sub esp,4
        fadd dword ptr ds: [mpvote_btn_x1]
        mov eax,[ebp+0x5E]
        fstp dword ptr [esp]
        mov esi,[eax+0x738]
        push 0
        test esi,esi
        sete al
        movsx eax,al
        xor edx,edx
        call sub_4A6C00
        lea eax,[ebp-0xE]
        call sub_6959C9
        mov eax,ds: [ebp+0x5E]
        mov ebx,0x6E501C    // RACEENDRETRY
        mov edx,0x6E5029    // gametext/hud.txt
        push [eax+0x744]
        mov eax,ds: [0x7E3110]
        push [ebp+0x62]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[ebp-0xDE]
        call sub_69586C
        call sub_695B53
        push eax
        push 0x41C00000
        push 0x430C0000
        mov eax,ds: [0x7CF700]
        push 0x43C80000
        call sub_5EA350
        fld dword ptr [eax+0x1C]
        fmul dword ptr ds: [0x6E518A]
        sub esp,0x4
        fadd dword ptr ds: [mpvote_btn_x2]
        mov eax,[ebp+0x5E]
        fstp dword ptr [esp]
        mov edi,[eax+0x738]
        push 0
        cmp edi,1
        sete al
        movsx eax,al
        xor edx,edx
        call sub_4A6C00
        lea eax,[ebp-0xDE]

        jmp loc_5B577E

    sub_482930:
        push 0x482930
        retn

    loc_5B5783:
        push 0x5B5783
        retn

    sub_4A6C00:
        push 0x4A6C00
        retn

    sub_695B53:
        push 0x695B53
        retn

    sub_6959C9:
        push 0x6959C9
        retn

    sub_69586C:
        push 0x69586C
        retn

    sub_66F410:
        push 0x66F410
        retn

    sub_5EA350:
        push 0x5EA350
        retn

    loc_5B577E:
        push 0x5B577E
        retn
    }
}

void __declspec(naked) MPVoting_actions()
{
    __asm
    {
        mov edx, 4
        mov eax, ebp
        jmp loc_5B41A0

    sub_5B3F90:
        push 0x5B3F90
        retn

    loc_5B41A0:
        push 0x5B41A0
        retn
    }
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
        } while (FindNextFile(hFind, &findfiledata) != 0);
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

            std::transform(allcarfolders.begin(), allcarfolders.end(), allcarfolders.begin(), ::tolower);

            injector::WriteMemory(0x564887, static_cast<void*>(allcarfolders.data()), true);
            AllCarsInMP_init = true;
        }
    }
}

void MPFinishScreen()
{
    injector::MakeJNE(0x5B566F, MPVoting_btns, true);
    injector::WriteMemory(0x5B403A, 2, true);   // count of buttons to select

    if (CDNetwork())
    {
        injector::WriteMemory<BYTE>(0x064EDCE, 5, true);

        injector::MakeJMP(0x5B46D8, MPVoting_actions, true);
    }
    else
    {
        injector::WriteMemory<BYTE>(0x064EDCE, 4, true);

        injector::WriteMemory(0x5B46D8, 0xB1E8E889, true);
        injector::WriteMemory<BYTE>(0x5B46DC, 0xF8, true);
    }

    if (GetRaceState() == (BYTE)CDRaceState::Finished)
    {
        injector::WriteMemory(0x5B644C, continuerace_section.data(), true);
        injector::WriteMemory(0x5B6442, continuerace_key.data(), true);
    }
    else
    {
        injector::WriteMemory(0x5B644C, 0x6E5091, true);
        injector::WriteMemory(0x5B6442, 0x6E507E, true);
    }

    if (CDNetwork())
    {
        waithost_string = LocStr("CrashPatch.hud", "WAITINGFORSERVERCHOICE");

        waithost_1 = GetRaceTimer() / 100.f;
        waithost_4 = ((float)GetWindowWidth() / (float)GetWindowHeight() * 480) / 2;

        if (IsServer())
        {
            mpvote_btns = 1;
        }
        else
        {
            mpvote_btns = 0;
        }
    }

    if (mpvote_btns == 1)
    {
        injector::WriteMemory(0x64F413, 0xF64C98E8, true);
        injector::WriteMemory<BYTE>(0x64F417, 0xFF, true);
    }
    else
    {
        injector::MakeNOP(0x64F413, 5, true);
    }
}

void __declspec(naked) KickPlayerButton_func()
{
    __asm
    {
        push ecx
        cmp dl, 8
        jne end
        call GetCurPlayerInMPList
        push eax
        call KickPlayer
        add esp, 4

        sub ds: [0x7A7758], 1

        jmp end

    end:
        pop ecx
        retn
    }
}

void __declspec(naked) KickPlayerButton()
{
    __asm
    {
        push ebx
        push ecx
        push esi
        push edi
        sub esp, 0x40
        cmp dl, 5
        ja end
        xor eax, eax
        mov al, dl
        cmp al, 3
        je main
        jmp end

    main:
        push KickPlayerButton_func
        mov ebx, -1
        mov edx, dlg_kick_no
        lea eax, [esp + 0x34]
        call sub_69586C
        mov ebx, -1
        push eax
        mov edx, dlg_kick_yes
        lea eax, [esp + 0x28]
        call sub_69586C
        mov ebx, -1
        mov edx, dlg_kick_text
        mov ecx, eax
        lea eax, [esp + 0x18]
        call sub_69586C
        mov ebx, -1
        mov edx, 0x6C9FE6 //dlg_kick_no
        mov esi, eax
        lea eax, [esp + 0x08]
        call sub_69586C
        mov edi, ds: [0x78D65C]
        mov ebx, esi
        mov edx, eax
        mov eax, edi
        call sub_4A4D00
        mov eax, esp
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0x10]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0x20]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0x30]
        xor edx, edx
        call sub_6959C9
        add esp, 0x40
        pop edi
        pop esi
        pop ecx
        pop ebx
        ret

        /*push 0
        mov ebx, 0x6E1963
        mov edx, 0x6E1966
        mov eax, ds: [0x7E3110]
        call sub_66F410
        mov ebx, -1
        mov edx, eax
        lea eax, [esp + 0x14]
        call sub_69586C
        mov ebx, 0x6E197B
        push eax
        mov edx, 0x6E197F
        mov eax, ds: [0x7E3110]
        call sub_66F410
        mov ebx, -1
        mov edx, eax
        lea eax, [esp + 0x58]
        call sub_69586C

        mov ebx, -1
        mov edx, dlg_kick_section
        mov ecx, eax
        call sub_69586C

        //mov ebx, dlg_kick_section   // LOBBYKICKPLAYERCONFIRM
        //mov edx, dlg_kick_key   // gametext/netlobby.txt
        //mov ecx, eax
        //mov eax, ds: [0x7E3110]
        //call sub_66F410

        //call GetCurPlayerInMPList // I can't make this shit works
        //mov edx, 0  
        //shl edx, 6
        //add edx, 0x7A77C8
        //add edx, 4
        //mov ecx, edx
        //call sub_695B53

        //push eax  // this as well
        //push 0
        //push dlg_kick_section
        //push dlg_kick_key
        //mov ecx, ds: [0x7E3110]
        //push ecx
        //call sub_66F850

        mov ebx, -1
        mov edx, eax
        lea eax, [esp + 0x48]
        call sub_69586C
        mov ebx, -1
        mov edx, 0x6E19B2
        mov esi, eax
        lea eax, [esp + 0x28]
        call sub_69586C
        mov edi, ds: [0x78D65C]
        mov ebx, esi
        mov edx, eax
        mov eax, edi
        call sub_4A4D00
        lea eax, [esp + 0x20]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0x40]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0x50]
        xor edx, edx
        call sub_6959C9
        lea eax, [esp + 0x10]
        xor edx, edx
        call sub_6959C9
        add esp, 0xA0
        pop edi
        pop esi
        pop ecx
        pop ebx
        retn*/

    end:
        add esp, 0x40
        pop edi
        pop esi
        pop ecx
        pop ebx
        retn

        
    sub_66F850:
        push 0x66F850
        retn

    sub_695B53:
        push 0x695B53
        retn

    sub_66F410:
        push 0x66F410
        retn

    sub_69586C:
        push 0x69586C
        retn

    sub_4A4D00:
        push 0x4A4D00
        retn

    sub_6959C9:
        push 0x6959C9
        retn
    }
}

void MPKick()
{
    dlg_kick_no.reserve(8192);
    dlg_kick_yes.reserve(8192);
    dlg_kick_text.reserve(8192);

    WriteString<uint32_t>(0x6D0DDF, gui_mplobby.c_str(), true); // instead of overwriting string pointers I have to overwrite strings itselves because the game keeps throwing "str.length() > 4" error when quiting from setup event screen
    WriteString<uint32_t>(0x6D28C3, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D47F1, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D48C5, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D5000, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D50ED, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D5144, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D519A, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D520E, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D5266, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D7683, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6D899C, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6DDD13, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6DDDCD, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6DE65F, gui_mplobby.c_str(), true);
    WriteString<uint32_t>(0x6CF3EB, gui_mplobby.c_str(), true);

    /*injector::WriteMemory(0x539572, gui_mplobby.data(), true);
    injector::WriteMemory(0x542DE5, gui_mplobby.data(), true);
    injector::WriteMemory(0x54ACEC, gui_mplobby.data(), true);
    injector::WriteMemory(0x5562BE, gui_mplobby.data(), true);
    injector::WriteMemory(0x55666C, gui_mplobby.data(), true);
    injector::WriteMemory(0x5593DD, gui_mplobby.data(), true);
    injector::WriteMemory(0x55A582, gui_mplobby.data(), true);
    injector::WriteMemory(0x55A6D4, gui_mplobby.data(), true);
    injector::WriteMemory(0x55A87A, gui_mplobby.data(), true);
    injector::WriteMemory(0x55AA9E, gui_mplobby.data(), true);
    injector::WriteMemory(0x55B3A6, gui_mplobby.data(), true);
    injector::WriteMemory(0x56A6B3, gui_mplobby.data(), true);
    injector::WriteMemory(0x56F1F0, gui_mplobby.data(), true);
    injector::WriteMemory(0x5879BD, gui_mplobby.data(), true);
    injector::WriteMemory(0x587C6A, gui_mplobby.data(), true);
    injector::WriteMemory(0x58A327, gui_mplobby.data(), true);*/

    injector::WriteMemory(0x56402A, btn_kick.data(), true);
    injector::WriteMemory(0x5642D4, btn_kick.data(), true);
    injector::MakeNOP(0x5643CF, 5, true);


    if (CDNetwork())
    {
        //printf("%x - %s\n", (0x7A77C8 + (0x40 * GetCurPlayerInMPList()) + 4), GetMPPlayerName(GetCurPlayerInMPList()));

        dlg_kick_no = LocStr("gametext/general.txt", "NO");
        dlg_kick_yes = LocStr("gametext/general.txt", "YES");
        dlg_kick_text = FormatStr(LocStr("gametext/netlobby.txt", "LOBBYKICKPLAYERCONFIRM"), GetMPPlayerName(GetCurPlayerInMPList()));

        if (injector::ReadMemory<DWORD>(btn_kick_ptr, true))
        {
            injector::WriteMemory(injector::ReadMemory<DWORD>(btn_kick_ptr, true) + 0x50, KickPlayerButton, true);
            if (IsServer())
            {
                if (GetCurPlayerInMPList() == 0)    // the host can't kick itself
                {
                    injector::WriteMemory<BYTE>(injector::ReadMemory<DWORD>(btn_kick_ptr, true) + 0x54, 0, true);
                }
                else
                {
                    injector::WriteMemory<BYTE>(injector::ReadMemory<DWORD>(btn_kick_ptr, true) + 0x54, 1, true);
                }

                if (KeyPress(0xC8) || KeyPress(0xEC))
                {
                    if (GetCurPlayerInMPList() != 0)
                    {
                        SetCurPlayerInMPList(GetCurPlayerInMPList() - 1);
                    }
                }
                if (KeyPress(0xD0) || KeyPress(0xED))
                {
                    if (GetCurPlayerInMPList() < GetMPPlayersCount() - 1)
                    {
                        SetCurPlayerInMPList(GetCurPlayerInMPList() + 1);
                    }
                }

                if (GetCurPlayerInMPList() > GetMPPlayersCount() - 1)
                {
                    SetCurPlayerInMPList(GetMPPlayersCount() - 1);
                }
            }
            else
            {
                injector::WriteMemory<BYTE>(injector::ReadMemory<DWORD>(btn_kick_ptr, true) + 0x54, 0, true);
            }
        }
    }
}