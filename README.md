
This code demonstrates a technique to hook into a running process (in this case, Notepad) and intercept a specific API function call (LoadLibraryA). Let’s break down the main components:

Key Concepts:
Hooking: This is the practice of intercepting function calls, messages, or events passed within a system. In this example, the LoadLibraryA function is hooked.
Process Injection: The code targets the Notepad process (notepad.exe), reads its memory, and modifies it to hook into a Windows API call (LoadLibraryA), redirecting execution to a custom function (HookedLoadLibraryA).
Detailed Breakdown:
Global Variables:

bytesWritten: Used to track how many bytes are written during memory operations.
loadLibraryOriginalBytes: Stores the original bytes of LoadLibraryA, so the function can be restored after it’s hooked.
patch: The modified bytes used to hook LoadLibraryA.
messageMemoryAddress: The memory address where a custom message is stored inside the target process.
HookedLoadLibraryA: This is the custom function that will replace LoadLibraryA. It:

Prints a message to the console.
Unhooks LoadLibraryA by restoring its original bytes.
Calls the original LoadLibraryA.
Hooks it again by writing the patched bytes.
GetProcessIdByName:

This function takes the name of a process (notepad.exe) and retrieves its process ID (PID) by iterating over the system’s running processes using CreateToolhelp32Snapshot and Process32First/Next.
HookLoadLibraryInNotepad:

Retrieves the process ID of notepad.exe.
Opens the Notepad process with OpenProcess to gain access to its memory.
Gets the address of LoadLibraryA inside Notepad’s memory using GetProcAddress.
Reads the original bytes of LoadLibraryA into loadLibraryOriginalBytes.
Allocates memory inside the Notepad process to store a custom message (Hey you found me!).
Writes this message into the allocated memory inside Notepad.
Allocates memory in Notepad for the custom function (HookedLoadLibraryA).
Modifies the original LoadLibraryA function bytes (patching) to point to the custom HookedLoadLibraryA.
Changes the memory protection so the patch can be applied using VirtualProtectEx.
Writes the patch bytes into LoadLibraryA and restores the memory protection.
Random String Generator:

generateRandomString: Generates a random string of a specified length, used in the main loop for user input validation.
Main Function:

Runs an infinite loop displaying random characters.
Asks the user to input either 1 or 2.
If 1 is chosen, it calls HookLoadLibraryInNotepad, which will hook the LoadLibraryA function in the Notepad process.
If 2 is chosen, the program quits.
How it works:
Intercepting API Calls: When a process (e.g., Notepad) calls LoadLibraryA to load a library, the hook redirects this call to HookedLoadLibraryA. This function prints the intercepted library file name and then calls the original LoadLibraryA.
Message Injection: The code writes a custom message into the Notepad process memory and displays the memory address where it is stored.

![image](https://github.com/user-attachments/assets/0a7eb8ab-8885-4c17-8e2b-482bc805263d)
![image](https://github.com/user-attachments/assets/eb02ff72-d8c5-4e9f-93d5-f49827c03d30)
![image](https://github.com/user-attachments/assets/1655e26f-576a-4923-abc1-004724a90db9)
![image](https://github.com/user-attachments/assets/b2185b82-d4c7-43ac-bad5-85dce3824107)
![image](https://github.com/user-attachments/assets/c24dfa10-6b1e-4485-9bb1-a37385e4ffc4)

