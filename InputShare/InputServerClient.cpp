#include <Windows.h>
#include "InputHook.h"
#include "InputNetwork.h"

int main(int argc, char* argv[])
{
	char *ip = argv[1];
	int port = 5555;
	if (argc > 2)
		port = atoi(argv[2]);

	NetworkStartup(ip, port);

	SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
	SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	NetworkShutdown();

	return 0;
}