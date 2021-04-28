// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>

#include <windows.h>
#include <TlHelp32.h>

#include "mem.h"
//#include "proc.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    //Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "AC PWN Client\n\n";

    //Get module base addr 
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
    uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0x10f4f4);

    bool bHealth{ false }, bArmor{ false }, bGrenade{ false }, bAmmo{ false }, bDamage{ false }, bFireRate{ false }, bQuickReload{ false },
        bOutOfMap{ false }, bRecoil{ false };

    //hack loop
    while (true)
    {
        //key input
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            std::cout << "Health Toggled " << !bHealth << std::endl;
            bHealth = !bHealth;
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            std::cout << "Armor Toggled " << !bArmor << std::endl;
            bArmor = !bArmor;
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            std::cout << "Grenade Toggled " << !bGrenade << std::endl;
            bGrenade = !bGrenade;
        }
        if (GetAsyncKeyState(VK_NUMPAD4) & 1)
        {
            std::cout << "Ammo Toggled " << !bAmmo << std::endl;
            bAmmo = !bAmmo;
        }
        if (GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            bDamage = !bDamage;
        }
        if (GetAsyncKeyState(VK_NUMPAD6) & 1)
        {
            bFireRate = !bFireRate;
        }
        if (GetAsyncKeyState(VK_NUMPAD7) & 1)
        {
            bQuickReload = !bQuickReload;
        }
        if (GetAsyncKeyState(VK_NUMPAD8) & 1)
        {
            bOutOfMap = !bOutOfMap;

            if (bOutOfMap)
            {
                std::cout << "BYE BYE LMAO ;)\n";

                *(float*)(*localPlayerPtr + 0x3c) = 125;
            }

            else 
            {
                std::cout << "Welcome back\n";
                *(float*)(*localPlayerPtr + 0x3c) = 0;
            }
        }
        if (GetAsyncKeyState(VK_NUMPAD9) & 1)
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                std::cout << "No recoil ON\n";
                //No recoil
                *(int*)mem::FindDMAAddr(moduleBase + 0x10f4f4, { 0x374, 0xc, 0x120 }) = 0;
                *(int*)mem::FindDMAAddr(moduleBase + 0x10f4f4, { 0x374, 0xc, 0x122 }) = 0;
                *(int*)mem::FindDMAAddr(moduleBase + 0x10f4f4, { 0x374, 0xc, 0x116 }) = 0;
            }

            else
            {
                std::cout << "No recoil OFF [disabled]\n";
            }
        }

        //continuous write [freeze]

        if (localPlayerPtr)
        {
            if (bHealth)
            {
                *(int*)(*localPlayerPtr + 0xf8) = 1337;
            }
            
            if (bArmor)
            {
                *(int*)(*localPlayerPtr + 0xfc) = 1337;
            }
            
            if (bGrenade)
            {
                *(int*)(*localPlayerPtr + 0x158) = 1337;
            }

            if (bAmmo)
            {
                uintptr_t ammoAddr = mem::FindDMAAddr(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 });
                int* ammo = (int*)ammoAddr;
                *ammo = 1337;

            }
            Sleep(5);
        }
    }

    //cleanup & eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

