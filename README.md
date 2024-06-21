# Windows API Hooking
 This code demonstrates a technique known as function hooking. It hooks the LoadLibraryA function in a running notepad.exe process and replaces it with a custom function that prints messages to the console whenever LoadLibraryA is called. Here's a breakdown of how it works:

Key Components:
Global Variables and Hook Function:
bytesWritten, loadLibraryOriginalBytes, patch, messageMemoryAddress: These are global variables used to manage the hook and store the original bytes of the LoadLibraryA function.
HookedLoadLibraryA: This is the hook function that replaces the original LoadLibraryA function. It prints messages to the console and then calls the original LoadLibraryA.
Functions:
HookedLoadLibraryA:

This function is called instead of the original LoadLibraryA once the hook is in place.
It prints "Ohai from the hooked function" and the name of the library being loaded.
It temporarily restores the original LoadLibraryA function, calls it, and then re-applies the hook.
GetProcessIdByName:

This function retrieves the process ID of a running process given its name (e.g., notepad.exe).
It uses the Toolhelp32 snapshot to iterate over the list of running processes and find the one with the specified name.
HookLoadLibraryInNotepad:

This function sets up the hook for LoadLibraryA in the notepad.exe process.
It finds the process ID of notepad.exe and opens the process with OpenProcess.
It retrieves the address of the LoadLibraryA function in the kernel32.dll module.
It reads the original bytes of the LoadLibraryA function and stores them.
It allocates memory in the notepad.exe process to store the hook function and the message string.
It writes the hook function and the message string to the allocated memory.
It creates a patch (machine code) that redirects calls to LoadLibraryA to the hook function.
It modifies the memory protection of the LoadLibraryA function to make it writable, applies the patch, and then restores the original memory protection.
main:

This function provides a simple menu to either show the original MessageBox, hook LoadLibraryA in notepad.exe, or exit.
It waits for the user to open notepad.exe before attempting to hook LoadLibraryA.
Detailed Steps in HookLoadLibraryInNotepad:
Process ID Retrieval:

Uses GetProcessIdByName to get the process ID of notepad.exe.
Process Opening:

Opens the notepad.exe process with OpenProcess to get a handle with all access rights.
Function Address Retrieval:

Retrieves the address of LoadLibraryA in kernel32.dll using GetProcAddress.
Reading Original Bytes:

Reads the original bytes at the LoadLibraryA address to store them for later restoration.
Memory Allocation:

Allocates memory in the notepad.exe process for the hook function and the message string.
Writing Hook and Message:

Writes the hook function and the message string to the allocated memory in notepad.exe.
Patch Creation:

Creates a patch that redirects the execution flow from the original LoadLibraryA to the hook function.
Memory Protection Modification and Patching:

Changes the memory protection of the LoadLibraryA function to writable.
Writes the patch to redirect the function to the hook.
Restores the original memory protection.
