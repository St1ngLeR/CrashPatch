/* "Test Drive" game mode patches */

std::string testdrive_default_settings = "\
opps=0 \
damagerealism=0 \
gamevar=testdrive-default \
forcestartgridpos[0]=1 \
forcestartgridpos[1]=2 \
forcestartgridpos[2]=3 \
forcestartgridpos[3]=4 \
forcestartgridpos[4]=5 \
forcestartgridpos[5]=6 \
forcestartgridpos[6]=7 \
forcestartgridpos[7]=8";

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

float respawn_time = 3.f;

void __declspec(naked) a_TestDriveRespawn()
{
    __asm
    {
        push esi
        mov eax, esi  
        fld dword ptr [eax + 0x72FC]
        fcomp dword ptr [respawn_time]
        fstsw ax
        sahf
        jb skip

        mov eax, esi  
        call sub_44BA60 
        mov eax, esi
        call sub_507400  

    skip:
        pop esi
        ret 4   

    sub_44BA60:
        push 0x44BA60
        retn

    sub_507400:
        push 0x507400
        retn
    }
}


void TestDriveRespawn()
{
    injector::MakeCALL(0x5060A0, a_TestDriveRespawn, true);
}

void TestDriveOptions()
{
    injector::WriteMemory(0x57C195, static_cast<void*>(testdrive_default_settings.data()), true);
    injector::WriteMemory(0x588593, a_TestDriveOptions, true);
    injector::MakeJMP(0x58449F, 0x583DD0, true);
    injector::MakeNOP(0x589D23, 6, true);
}

void TestDriveInMultiplayer()
{
    injector::WriteMemory<short>(0x588D32, 0x1B2, true);
    injector::WriteMemory<short>(0x5879A7, 0x1B2, true);
}

void TestDriveTweaks()
{
    injector::WriteMemory<short>(0x641179, 0x54EB, true);

    injector::WriteMemory<int>(0x63CFAE, 0xFFFF4BE9, true);
    injector::WriteMemory<BYTE>(0x63CFB2, 0xFF, true);

    injector::MakeNOP(0x5A086C, 2, true);
}
