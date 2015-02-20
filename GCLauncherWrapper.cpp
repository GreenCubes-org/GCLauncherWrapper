// GCLauncherWrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

// You can specify here paths to your JREs.
std::string java64 = ".\\jre\\64\\bin\\javaw.exe";
std::string java32 = ".\\jre\\32\\bin\\javaw.exe";

// Specify parameters to pass to JRE. All arguments
// passed to wrapper will be passed too
std::string launchParams = "-Xms64M -Xmx128M -jar greencubes.jar";

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
	}
	return bIsWow64;
}

int main(int argc, char* argv[])
{
	std::string javaExe;
	if (IsWow64())
	{
		javaExe = java64;
	}
	else
	{
		javaExe = java32;
	}
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::string command;
	command = javaExe + " " + launchParams;
	for (int i = 1; i < argc; ++i)
	{
		command = command + " " + argv[i];
	}

	//printf("Starting %s\n", command.c_str());
	// Start the child process. 
	if (!CreateProcessA(NULL,   // No module name (use command line)
		LPSTR(command.c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 1;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}


