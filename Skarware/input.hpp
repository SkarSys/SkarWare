#include "Skicript.hpp"
typedef struct _InjectedInputMouseInfo
{
	int DeltaX;
	int DeltaY;
	unsigned int MouseData;
	unsigned int TimeOffsetInMilliseconds;
	void* ExtraInfo;
} InjectedInputMouseInfo;
namespace MouseController
{
	static BYTE NtUserSendInput_Bytes[30];
	static BYTE NtUserGetAsyncKeyState_Bytes[30];
	static BYTE NtUserInjectMouseInput_Bytes[30];


	// Call this once in DllMain or main
	static BOOLEAN WINAPI Init()
	{
		// windows 8.1 / windows 10
		LPVOID NtUserInjectMouseInput_Addr = GetProcAddress(GetModuleHandle("win32u"), "NtUserInjectMouseInput");
		if (!NtUserInjectMouseInput_Addr)
		{
			NtUserInjectMouseInput_Addr = GetProcAddress(GetModuleHandle("user32"), "NtUserInjectMouseInput");
			if (!NtUserInjectMouseInput_Addr)
			{
				// Windows 7 or lower detected
				NtUserInjectMouseInput_Addr = GetProcAddress(GetModuleHandle("user32"), "InjectMouseInput");
				if (!NtUserInjectMouseInput_Addr)
					return FALSE;
			}
		}
		memcpy(NtUserInjectMouseInput_Bytes, NtUserInjectMouseInput_Addr, 30);
		return TRUE;
	}

	/* This function spoofs the function. It prevents BattlEye from scanning it! */
	static BOOLEAN WINAPI NtInjectMouse(InjectedInputMouseInfo* input, int count)
	{
		LPVOID NtUserInjectMouseInput_Spoof = VirtualAlloc(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE); // allocate space for syscall
		if (!NtUserInjectMouseInput_Spoof)
			return FALSE;
		memcpy(NtUserInjectMouseInput_Spoof, NtUserInjectMouseInput_Bytes, 30); // copy syscall
		NTSTATUS Result = reinterpret_cast<NTSTATUS(NTAPI*)(InjectedInputMouseInfo*, int)>(NtUserInjectMouseInput_Spoof)(input, count); // calling spoofed function
		ZeroMemory(NtUserInjectMouseInput_Spoof, 0x1000); // clean address
		VirtualFree(NtUserInjectMouseInput_Spoof, 0, MEM_RELEASE); // free it
		return (Result > 0); // return the status
	}

	/* This function moves the mouse using the syscall */
	static BOOLEAN WINAPI NtInjectMouse(int X, int Y)
	{
		InjectedInputMouseInfo info = { 0 };
		info.DeltaX = X;
		info.DeltaY = Y;
		return NtInjectMouse(&info, 1);
	}
}