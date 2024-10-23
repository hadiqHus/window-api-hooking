#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdlib>
#include <ctime>

SIZE_T bytesWritten = 0;
char loadLibraryOriginalBytes[6] = {};
char patch[6] = {};
LPVOID messageMemoryAddress = nullptr;  // Memory address for the message string in the target process

HMODULE __stdcall HookedLoadLibraryA(LPCSTR lpLibFileName) {
    std::cout << "Hey you found me!\n"; // Print message in the console
    std::cout << "LoadLibraryA called with: " << lpLibFileName << std::endl; // Print intercepted values
    // Unpatch LoadLibraryA
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), loadLibraryOriginalBytes, sizeof(loadLibraryOriginalBytes), &bytesWritten);
    // Call the original LoadLibraryA
    HMODULE result = LoadLibraryA(lpLibFileName);
    // Repatch LoadLibraryA
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), patch, sizeof(patch), &bytesWritten);
    return result;
}

DWORD GetProcessIdByName(const wchar_t* processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    if (Process32First(snapshot, &processEntry)) {
        do {
            if (wcscmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry));
    }
    CloseHandle(snapshot);
    return 0;
}

void HookLoadLibraryInNotepad() {
    DWORD processId = GetProcessIdByName(L"notepad.exe");
    if (processId == 0) {
        std::cerr << "Notepad is not running." << std::endl;
        return;
    }
    
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        std::cerr << "Failed to open Notepad process. Error: " << GetLastError() << std::endl;
        return;
    }

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    if (hKernel32 == NULL) {
        std::cerr << "Failed to get handle of kernel32.dll. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return;
    }
    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryA");
    if (loadLibraryAddr == NULL) {
        std::cerr << "Failed to get address of LoadLibraryA. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return;
    }

    SIZE_T bytesRead = 0;
    if (!ReadProcessMemory(hProcess, loadLibraryAddr, loadLibraryOriginalBytes, sizeof(loadLibraryOriginalBytes), &bytesRead)) {
        std::cerr << "Failed to read original bytes from LoadLibraryA. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return;
    }
    void* remoteMemory = VirtualAllocEx(hProcess, NULL, sizeof(void*), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (remoteMemory == NULL) {
        std::cerr << "Failed to allocate memory in Notepad process. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return;
    }

    const char message[] = "Hey you found me!";
    messageMemoryAddress = VirtualAllocEx(hProcess, NULL, sizeof(message), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (messageMemoryAddress == NULL) {
        std::cerr << "Failed to allocate memory for the message in Notepad process. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, messageMemoryAddress, message, sizeof(message), &bytesWritten)) {
        std::cerr << "Failed to write the message to Notepad process. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, messageMemoryAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, remoteMemory, (void*)&HookedLoadLibraryA, sizeof(void*), NULL)) {
        std::cerr << "Failed to write shellcode to Notepad process. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, messageMemoryAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }

    void* hookedLoadLibraryAddress = remoteMemory;
    memcpy_s(patch, 1, "\x68", 1);
    memcpy_s(patch + 1, 4, &hookedLoadLibraryAddress, 4);
    memcpy_s(patch + 5, 1, "\xC3", 1);

    DWORD oldProtect;
    if (!VirtualProtectEx(hProcess, loadLibraryAddr, sizeof(patch), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        std::cerr << "Failed to change memory protection. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, messageMemoryAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }
    if (!WriteProcessMemory(hProcess, loadLibraryAddr, patch, sizeof(patch), &bytesWritten)) {
        std::cerr << "Failed to patch LoadLibraryA. Error: " << GetLastError() << std::endl;
        VirtualProtectEx(hProcess, loadLibraryAddr, sizeof(patch), oldProtect, &oldProtect);
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        VirtualFreeEx(hProcess, messageMemoryAddress, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return;
    }
    if (!VirtualProtectEx(hProcess, loadLibraryAddr, sizeof(patch), oldProtect, &oldProtect)) {
        std::cerr << "Failed to restore memory protection. Error: " << GetLastError() << std::endl;
    }
    std::cout << "LoadLibraryA hooked in Notepad.\n";
    std::cout << "LoadLibraryA memory address: " << loadLibraryAddr << std::endl;
    std::cout << "Message memory address: " << messageMemoryAddress << std::endl;
    CloseHandle(hProcess);
}
// Function to generate a random string of length 'n'
std::string generateRandomString(int n) {
    std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string randomString;

    for (int i = 0; i < n; ++i) {
        randomString += characters[rand() % characters.size()];
    }
    return randomString;
}

int main() {
    int choice; 
    srand((unsigned)time(0));
    do {
        std::string randomChars = generateRandomString(40000);
        std::cout << "<<--------------------1 or 2-------------------->>\n" << randomChars << std::endl;
        std::cin >> choice;
        switch (choice) {
        case 1:
            system("pause");
            HookLoadLibraryInNotepad();
            break;
        case 2:
            std::cout << "Quitting\n";
            break;
        default:
            std::string randomChars = generateRandomString(40000);
            break;
        }
    } while (choice != 2);
    return 0;
}
