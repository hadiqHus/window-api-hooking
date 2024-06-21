# Windows API Hooking
 This code demonstrates a windows api hooking. It hooks a dll function in a running process and replaces it with a custom function that prints messages to the console whenever the libary is called. 

Key Components:
Global Variables and Hook Function: bytesWritten, loadLibraryOriginalBytes, patch, messageMemoryAddress: These are global variables used to manage the hook and store the original bytes of the LoadLibraryA function.
HookedLoadLibraryA: This is the hook function that replaces the original LoadLibraryA function. It prints messages to the console and then calls the original LoadLibraryA.

Functions:
HookedLoadLibraryA: This function is called instead of the original LoadLibraryA once the hook is in place. It prints "Ohai from the hooked function" and the name of the library being loaded. It temporarily restores the original LoadLibraryA function, calls it, and then re-applies the hook.

GetProcessIdByName: This function retrieves the process ID of a running process given its name (e.g., notepad.exe). It uses the Toolhelp32 snapshot to iterate over the list of running processes and find the one with the specified name.

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

![memoryaddress](https://github.com/hadiqHus/window-api-hooking/assets/64806441/e849ca23-f3dd-44e7-a15d-244486519b40)
![processhacker](https://github.com/hadiqHus/window-api-hooking/assets/64806441/c8dc474f-5f98-4772-a309-3829d6026846)
![loadlibary](https://github.com/hadiqHus/window-api-hooking/assets/64806441/29666196-6279-4b0e-a5e8-8d5cfb519877)

