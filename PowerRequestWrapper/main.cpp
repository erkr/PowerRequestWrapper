#include <windows.h>
#include <WinBase.h>
#include <stdio.h>
#include <tchar.h>
#include <atlstr.h>
#include "getopt.h"
#include "main.h"


int launchApplicationAndWait(LPWSTR commandLine)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        commandLine,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
		int lastError = GetLastError();
		if (lastError == 740) {
	       _tprintf( L"CreateProcess failed - The requested command requires elevation\n" );
		}
		else {
		   _tprintf( L"CreateProcess failed (%d); please check the command: \"%s\".\n", lastError, commandLine);
		}
        return 1;
    }
	
    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
	return 0;
}

/*
wchar_t *GetWChar(char* charIn)
{
	size_t size = strlen(charIn) + 1;
	wchar_t* charOut = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, charOut, size, charIn, size - 1);
	return charOut;  // client needs to delete the returned object!
}
*/

int _tmain( int argc, TCHAR *argv[] )
{
	int result = 1;

	// parse arguments
	int opt = 0;
	wchar_t* cmd = NULL;
	wchar_t* msg = NULL;
	while ((opt = getopt(argc, argv, L"c:m:n")) != -1) {
		switch (opt) {
		case 'n':
			FreeConsole(); // close console window
			break;
		case 'c':
			cmd = optarg;
			break;
		case 'm':
			msg = optarg; 
			break;
		default: /* '?' */
			fwprintf(stderr, L"Usage: %s [-c commandline] [-m message] [-n]\n", argv[0]);
			fwprintf(stderr, L"- example1: %s -c notepad -m \"PC will not sleep until you close Notepad\"\n", argv[0]);
			fwprintf(stderr, L"- example2: %s -m \"PC will not sleep until you close the popup dialog\"\n", argv[0]);
			fwprintf(stderr, L"- Use -n option to run without a console window (no errors/output feedback!)\n");

			exit(2);
		}
	}
	// This point in the code is reached with valid options only

	// Create a system availablity request to keep the system from automatically sleeping
	POWER_REQUEST_CONTEXT myPowerRequestContext;
	HANDLE hPowerRequest;

	// Set up the diagnostic string
	myPowerRequestContext.Version = POWER_REQUEST_CONTEXT_VERSION;
	myPowerRequestContext.Flags = POWER_REQUEST_CONTEXT_SIMPLE_STRING;
	if (msg) {
		// Pass the specified message
		myPowerRequestContext.Reason.SimpleReasonString = msg;
	}
	else if (cmd) {  // pass the command as the message
		myPowerRequestContext.Reason.SimpleReasonString = cmd;
	}
	else {  // no options on the command line; the interactive message:
		myPowerRequestContext.Reason.SimpleReasonString = L"Unlock by pressing OK in pop-up dialog";
	}
	_tprintf(L"%s started: \"%s\".\n", argv[0], myPowerRequestContext.Reason.SimpleReasonString);

	// Create the request, get a handle
	hPowerRequest = PowerCreateRequest(&myPowerRequestContext);

	// Set a system request to prevent automatic sleep
	PowerSetRequest(hPowerRequest, PowerRequestSystemRequired);
	
	// start the action that needs to be protected for the PC going into sleep mode
	if (cmd) {  // launch the command and Wait till it terminates
		result = launchApplicationAndWait(cmd);
	} else {  // wait till dialog is closed
		result = MessageBox(NULL, L"Computer is kept awake until you close this dialog", argv[0], MB_OK);
	}

	// Clear the request
	PowerClearRequest(hPowerRequest,PowerRequestSystemRequired);
	CloseHandle(hPowerRequest);
	_tprintf(L"%s stopped: Power Lock released.\n", argv[0]);

	exit(result);
}
