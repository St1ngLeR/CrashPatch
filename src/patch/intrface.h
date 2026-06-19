/* Interface patches */

float finishscreen_posy_start = -0.5f;

float intro;

bool intro_anim;

bool skip_intro;

bool ui_menus;

int garage_state;

BYTE seltrack_anim;

bool FixSelTrackPreview_init;

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

void __declspec(naked) a_CarColorSlider()
{
    __asm
    {
        mov ds: 0x795D1C, ah
        mov ds: 0x79620A, ah
        mov ds: 0x796209, ah
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

void NoOppsBombRun()
{
    injector::MakeJMP(0x581F15, a_NoOppsBombRun, true);
}

void FixNotAvailablePTP()
{
    injector::MakeNOP(0x58ACF1, 6, true);
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

            if (KeyPress(0x1C) || KeyPress(0xDF))
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

void BombRunHUD()
{
    injector::MakeJMP(0x5B266C, a_BombRunHUD, true);
}

void FixObjectiveTextNewLineAppend()
{
    injector::MakeNOP(0x646E1C, 5, true);
}

void CareerBar()    // TODO
{
    injector::MakeJMP(0x5387BD, a_CareerBar, true);
    //injector::MakeJMP(0x535C67, a_GarageState, true);
}

void ColoredCarsOnRadar()
{
    injector::MakeNOP(0x5B75A1, 2, true);
    injector::MakeJMP(0x5B75A3, a_ColoredCarsOnRadar, true);
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

void FixCrash_TrackEditorHiddenTileSelect()
{
    injector::WriteMemory(0x4DD25D, 0x86E9, true);
    injector::WriteMemory<BYTE>(0x4DD261, 0, true);
}

void DisableRpmtexDependency()
{
    injector::WriteMemory<int>(0x5A5E16, 0x024BE9, true);
    injector::WriteMemory<short>(0x5A5E19, 0, true);
    injector::WriteMemory<short>(0x5A5BF1, 0x73EB, true);
    injector::WriteMemory<short>(0x5A5F0B, 0x7DEB, true);
    injector::MakeNOP(0x5A6061, 5, true);
    injector::MakeNOP(0x5A5D3E, 5, true);
    injector::MakeNOP(0x5A606A, 5, true);
}

void TypoFix_WreckPerfectWin()
{
    injector::WriteMemory(0x486B92, 0x6B72CD, true);
}

const char* career_section = "CrashPatch.career";
const char* dlgreqclassa_key = "DIALOG_EVENTREQUIRESCLASSA";
const char* dlgreqclassb_key = "DIALOG_EVENTREQUIRESCLASSB";
const char* dlgreqclassc_key = "DIALOG_EVENTREQUIRESCLASSC";
const char* dlgreqclassd_key = "DIALOG_EVENTREQUIRESCLASSD";
const char* dlgreqclasse_key = "DIALOG_EVENTREQUIRESCLASSE";

void __declspec(naked) a_EventParticipationCond()
{
    __asm
    {
        mov eax, ds: [0x7EA2CC]
        mov esi, ds: [0x78E7BC]
        mov dl, ds: [0x78E778]
        mov eax, ds: [eax+0xC]
        call sub_68D7E0 // eax - current career event

        cmp byte ptr [eax + 0x20], 0    // event participation condition
        jne eventcond
        jmp skip

    eventcond:
        cmp byte ptr [eax + 0x20], 1    // requiresclass
        je eventcond_requiresclass
        jmp skip

    eventcond_requiresclass:
        lea ecx, [eax + 0x24]

        mov edx, 0x702B7A   // a
        mov eax, ecx
        call sub_696058
        test eax, eax
        jne dlg_requiresclass_a

        mov edx, 0x702B7C   // b
        mov eax, ecx
        call sub_696058
        test eax, eax
        jne dlg_requiresclass_b

        mov edx, 0x702B7E   // c
        mov eax, ecx
        call sub_696058
        test eax, eax
        jne dlg_requiresclass_c

        mov edx, 0x702B80   // d
        mov eax, ecx
        call sub_696058
        test eax, eax
        jne dlg_requiresclass_d

        mov edx, 0x702B82   // e
        mov eax, ecx
        call sub_696058
        test eax, eax
        jne dlg_requiresclass_e

        jmp skip

    dlg_requiresclass_a:
        mov eax, ds: [0x7DE710]
        mov eax, ds: [eax + 0x8]
        cmp byte ptr [eax + 0x4908], 0x8
        je skip
        jmp dlg_requiresclass_a_fin

    dlg_requiresclass_a_fin:
        push 00
        mov ebx,0x6D6E5C
        mov edx,0x6D6E5F
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x64]
        call sub_69586C
        mov ebx, dlgreqclassa_key
        mov edx, career_section
        mov ecx,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x54]
        call sub_69586C
        mov ebx,0x6D6E90
        mov edx, 0x6D6E96
        mov esi,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x44]
        call sub_69586C
        mov edi,ds: [0x78D65C]
        mov ebx,esi
        mov edx,eax
        mov eax,edi
        call sub_4A4A70
        lea eax,[esp+0x40]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x50]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x60]
        xor edx,edx
        call sub_6959C9

        jmp end

    dlg_requiresclass_b:
        mov eax, ds: [0x7DE710]
        mov eax, ds: [eax + 0x8]
        cmp byte ptr [eax + 0x4908], 0x6
        je skip
        jmp dlg_requiresclass_b_fin

    dlg_requiresclass_b_fin:
        push 00
        mov ebx,0x6D6E5C
        mov edx,0x6D6E5F
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x64]
        call sub_69586C
        mov ebx, dlgreqclassb_key
        mov edx, career_section
        mov ecx,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x54]
        call sub_69586C
        mov ebx,0x6D6E90
        mov edx, 0x6D6E96
        mov esi,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x44]
        call sub_69586C
        mov edi,ds: [0x78D65C]
        mov ebx,esi
        mov edx,eax
        mov eax,edi
        call sub_4A4A70
        lea eax,[esp+0x40]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x50]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x60]
        xor edx,edx
        call sub_6959C9

        jmp end
            
    dlg_requiresclass_c:
        mov eax, ds: [0x7DE710]
        mov eax, ds: [eax + 0x8]
        cmp byte ptr [eax + 0x4908], 0x4
        je skip
        jmp dlg_requiresclass_c_fin

    dlg_requiresclass_c_fin:
        push 00
        mov ebx,0x6D6E5C
        mov edx,0x6D6E5F
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x64]
        call sub_69586C
        mov ebx, dlgreqclassc_key
        mov edx, career_section
        mov ecx,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x54]
        call sub_69586C
        mov ebx,0x6D6E90
        mov edx, 0x6D6E96
        mov esi,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x44]
        call sub_69586C
        mov edi,ds: [0x78D65C]
        mov ebx,esi
        mov edx,eax
        mov eax,edi
        call sub_4A4A70
        lea eax,[esp+0x40]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x50]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x60]
        xor edx,edx
        call sub_6959C9

        jmp end
            
    dlg_requiresclass_d:
        mov eax, ds: [0x7DE710]
        mov eax, ds: [eax + 0x8]
        cmp byte ptr [eax + 0x4908], 0x2
        je skip
        jmp dlg_requiresclass_d_fin

    dlg_requiresclass_d_fin:
        push 00
        mov ebx,0x6D6E5C
        mov edx,0x6D6E5F
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x64]
        call sub_69586C
        mov ebx, dlgreqclassd_key
        mov edx, career_section
        mov ecx,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x54]
        call sub_69586C
        mov ebx,0x6D6E90
        mov edx, 0x6D6E96
        mov esi,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x44]
        call sub_69586C
        mov edi,ds: [0x78D65C]
        mov ebx,esi
        mov edx,eax
        mov eax,edi
        call sub_4A4A70
        lea eax,[esp+0x40]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x50]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x60]
        xor edx,edx
        call sub_6959C9

        jmp end

    dlg_requiresclass_e:
        mov eax, ds: [0x7DE710]
        mov eax, ds: [eax + 0x8]
        cmp byte ptr [eax + 0x4908], 0x0
        je skip
        jmp dlg_requiresclass_e_fin

    dlg_requiresclass_e_fin:
        push 00
        mov ebx,0x6D6E5C
        mov edx,0x6D6E5F
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x64]
        call sub_69586C
        mov ebx, dlgreqclasse_key
        mov edx, career_section
        mov ecx,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x54]
        call sub_69586C
        mov ebx,0x6D6E90
        mov edx, 0x6D6E96
        mov esi,eax
        mov eax,ds: [0x7E3110]
        call sub_66F410
        mov ebx,-1
        mov edx,eax
        lea eax,[esp+0x44]
        call sub_69586C
        mov edi,ds: [0x78D65C]
        mov ebx,esi
        mov edx,eax
        mov eax,edi
        call sub_4A4A70
        lea eax,[esp+0x40]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x50]
        xor edx,edx
        call sub_6959C9
        lea eax,[esp+0x60]
        xor edx,edx
        call sub_6959C9

        jmp end

    end:
        push 0x5154D6
        retn

    skip:
        mov ebx, -1
        push 0x514FF0
        retn

    sub_68D7E0:
        push 0x68D7E0
        retn

    sub_696058:
        push 0x696058
        retn

    sub_66F410:
        push 0x66F410
        retn

    sub_69586C:
        push 0x69586C
        retn

    sub_4A4A70:
        push 0x4A4A70
        retn

    sub_6959C9:
        push 0x6959C9
        retn
    }
}

void EventParticipationCond()
{
    injector::MakeJMP(0x514FEB, a_EventParticipationCond, true);
}