#include "InputHook.h"
#include "InputNetwork.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	static bool shiftIsDown = false;

	if (nCode == HC_ACTION)
	{
		KBDLLHOOKSTRUCT *kbd = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		if (!(kbd->flags & LLKHF_INJECTED))
		{

			INPUT input = { 0 };
			input.type = INPUT_KEYBOARD;
			input.ki.wVk = (WORD)kbd->vkCode;
			input.ki.wScan = (WORD)kbd->scanCode;
			input.ki.dwFlags |= kbd->flags & LLKHF_UP ? KEYEVENTF_KEYUP : 0;
			//inputFlag |= kbd->flags & LLKHF_ALTDOWN ? KEYEVENTF_KEYUP : 0;

			if (kbd->vkCode == 'C' && GetKeyState(VK_LMENU) && GetKeyState(VK_LCONTROL))
			{
				input.ki.wVk = VK_LCONTROL;
				input.ki.wScan = 0x1d;
				input.ki.dwFlags = KEYEVENTF_KEYUP;
				NetworkSendInput(input);
				input.ki.wVk = VK_LMENU;
				input.ki.wScan = 0x38;
				NetworkSendInput(input);
				ExitProcess(-1);
			}

			if (kbd->vkCode == VK_INSERT && GetKeyState(VK_LMENU) && GetKeyState(VK_LCONTROL))
			{
				kbd->vkCode = VK_DELETE;
			}

			//OutputDebugString(L"Input\r\n");

			switch (kbd->vkCode)
			{
			case VK_SHIFT:
			case VK_LSHIFT:
			case VK_RSHIFT:
				OutputDebugString(kbd->flags & LLKHF_UP ? L"SHIFT UP\r\n" : L"shift down\r\n");
				break;
			case VK_MENU:
			case VK_LMENU:
			case VK_RMENU:
				OutputDebugString(kbd->flags & LLKHF_UP ? L"ALT UP\r\n" : L"alt down\r\n");
				break;
			default:
				WCHAR buffer[255];
				wsprintf(buffer, L"VK: %x, %c, %s\r\n", kbd->vkCode, kbd->vkCode, kbd->flags & LLKHF_UP ? L"UP" : L"down");
				OutputDebugString(buffer);
				break;
			}
			
			//SendInput(1, &input, sizeof(input));
			
			NetworkSendInput(input);

			if (kbd->vkCode == VK_MENU    || kbd->vkCode == VK_LMENU    || kbd->vkCode == VK_RMENU ||
				kbd->vkCode == VK_CONTROL || kbd->vkCode == VK_LCONTROL || kbd->vkCode == VK_RCONTROL)
			{
				return 0;
			}
			return 1;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	static POINT lastMousePoint = { LONG_MAX, LONG_MAX };

	if (nCode == HC_ACTION)
	{
		MSLLHOOKSTRUCT* ms = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

		if (!(ms->flags & LLMHF_INJECTED))
		{
			INPUT input = {0};
			input.type = INPUT_MOUSE;
		
			switch (wParam)
			{
			case WM_MOUSEWHEEL:
				input.mi.dwFlags = MOUSEEVENTF_WHEEL;
				input.mi.mouseData = (SHORT)HIWORD(ms->mouseData);
				break;
			case WM_MOUSEHWHEEL:
				input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
				input.mi.mouseData = (SHORT)HIWORD(ms->mouseData);
				break;
			case WM_LBUTTONDOWN:
				input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				break;
			case WM_LBUTTONUP:
				input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				break;
			case WM_RBUTTONDOWN:
				input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
				break;
			case WM_RBUTTONUP:
				input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
				break;
			case WM_MOUSEMOVE:
				if (lastMousePoint.x != LONG_MAX)
				{
					input.mi.dwFlags = MOUSEEVENTF_MOVE;// | MOUSEEVENTF_ABSOLUTE;// | MOUSEEVENTF_VIRTUALDESK;

					//input.mi.dx = (ms->pt.x * 65535)/1920;// - lastMousePoint.x;
					//input.mi.dy = (ms->pt.y * 65535)/1200;// - lastMousePoint.y;
					input.mi.dx = ms->pt.x  - lastMousePoint.x;
					input.mi.dy = ms->pt.y - lastMousePoint.y;

					WCHAR buffer[255];
					wsprintf(buffer, L"Mouse Move x:%d, y:%d; x:%d, y:%d\r\n", input.mi.dx, input.mi.dy, ms->pt.x, ms->pt.y);
					OutputDebugString(buffer);
				
				}
				lastMousePoint = ms->pt;
				NetworkSendInput(input);

				if (ms->pt.x < 10 || ms->pt.x > 1900 || ms->pt.y < 10 || ms->pt.y > 1100)
				{
					input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
					input.mi.dx = 0xFFFF / 2;
					input.mi.dy = 0xFFFF / 2;
					UINT i = SendInput(1, &input, sizeof(input));
					lastMousePoint.x = 1920/2;
					lastMousePoint.y = 1200/2;
					return 1;
				}
				return 0;
				break;
			}

			NetworkSendInput(input);
			//SendInput(1, &input, sizeof(input));

			
			return 1;
		}
		else
		{
			int i = 0;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}