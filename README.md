# Windows API Hooking
 This code demonstrates a windows api hooking. It hooks a dll function in a running process and replaces it with a custom function that prints messages to the console whenever the libary is called. 

This function sets up the hook for LoadLibraryA in the notepad.exe process.
It finds the process ID of notepad.exe and opens the process with OpenProcess.
It retrieves the address of the LoadLibraryA function in the kernel32.dll module.
It reads the original bytes of the LoadLibraryA function and stores them.
It allocates memory in the notepad.exe process to store the hook function and the message string.
It writes the hook function and the message string to the allocated memory.
It creates a patch (machine code) that redirects calls to LoadLibraryA to the hook function.
It modifies the memory protection of the LoadLibraryA function to make it writable, applies the patch, and then restores the original memory protection.
This function provides a simple menu to either show the original MessageBox, hook LoadLibraryA in notepad.exe, or exit.
It waits for the user to open notepad.exe before attempting to hook LoadLibraryA.

![memoryaddress](https://github.com/hadiqHus/window-api-hooking/assets/64806441/001b7b2b-4440-4a13-a182-b30dfe557aee)
![processhacker](https://github.com/hadiqHus/window-api-hooking/assets/64806441/6f21cce1-1b15-4634-8734-92e6f14b7030)
![loadlibary](https://github.com/hadiqHus/window-api-hooking/assets/64806441/381f353c-051e-4a22-90f0-b0a0c54b63c4)
