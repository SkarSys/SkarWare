static void xCreateWindow();
static void xInitD3d();
static void xMainLoop();
static void xShutdown();
#include "Cheat.h"
#include "Print.hpp"
#include "d3d9_x.h"
#include "xor.hpp"
#include <tchar.h>
#include <dwmapi.h>
#include <vector>
#include "Keybind.h"
#include "Threads.h"
#include "Offsets.h"
#include "xstring"
#include "BoneIndex.h"
#include "Aimbot.cpp"
#include "DrawEsp.h"
#include "W2S_ Camera.h"
#include "icons.h"
#include "definitions.h"
#include "Includes.h"
#define color1 (WORD)(0x0001 | 0x0000)
#define color2 (WORD)(0x0002 | 0x0000)
#define color3 (WORD)(0x0003 | 0x0000)
#define color4 (WORD)(0x0004 | 0x0000)
#define color5 (WORD)(0x0005 | 0x0000)
#define color6 (WORD)(0x0006 | 0x0000)
#define color7 (WORD)(0x0007 | 0x0000)
#define color8 (WORD)(0x0008 | 0x0000)
#define color9 (WORD)(0x0008 | 0x0000)
#define COLOR(h, c) SetConsoleTextAttribute(h, c);
ImFont* m_pFont;
Vector3 localactorpos;
uint64_t TargetPawn;
int localplayerID;
RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;
DWORD ScreenCenterX;
static ImColor Color;
DWORD ScreenCenterY;
DWORD_PTR closestPawn = NULL;
Vector3 Location;
static LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static HWND Window = NULL;
IDirect3D9Ex* p_Object = NULL;
static LPDIRECT3DDEVICE9 D3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;


template<class T>
struct TArray {
	friend struct FString;

public:
	inline TArray() {
		Data = nullptr;
		Count = Max = 0;
	};

	inline INT Num() const {
		return Count;
	};

	inline T& operator[](INT i) {
		return Data[i];
	};

	inline BOOLEAN IsValidIndex(INT i) {
		return i < Num();
	}

private:
	T* Data;
	INT Count;
	INT Max;
};

typedef struct _FNlEntity
{
	uint64_t Actor;
	int ID;
	uint64_t CurrentActorMesh;
	DWORD_PTR rootcomp;
	std::string name;
}FNlEntity;
std::vector<FNlEntity> entityList;

void SetWindowToTarget()
{
	while (true)
	{
		if (hwnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(hwnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(Window, GameRect.left, GameRect.top, Width, Height, true);
		}
		else
		{
			exit(0);
		}
	}
}
void xShutdown()
{
	TriBuf->Release();
	D3dDevice->Release();
	p_Object->Release();
	DestroyWindow(Window);
	UnregisterClass(skCrypt("AMD SOFTWARE: Adrenaline Edition"), NULL);
}
const MARGINS Margin = { -1 };
void xCreateWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = "AMD SOFTWARE: Adrenaline Edition";
	wc.lpfnWndProc = WinProc;
	RegisterClassEx(&wc);

	if (hwnd)
	{
		GetClientRect(hwnd, &GameRect);
		POINT xy;
		ClientToScreen(hwnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else
		exit(2);
	ShowWindow(Window, SW_SHOW);

	Window = CreateWindowEx(NULL, "AMD SOFTWARE: Adrenaline Edition", "AMD SOFTWARE: Adrenaline Edition", WS_POPUP | WS_VISIBLE, 0, 0, Width, Height, 0, 0, 0, 0);

	DwmExtendFrameIntoClientArea(Window, &Margin);
	SetWindowLong(Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);
}
void HitBoxAimKey()
{
	if (def::hitboxpos == 0)
	{
		def::hitbox = 68; //head
	}
	else if (def::hitboxpos == 1)
	{
		def::hitbox = 67; //neck
	}
	else if (def::hitboxpos == 2)
	{
		def::hitbox = 36; //chest
	}
	else if (def::hitboxpos == 3)
	{
		def::hitbox = 2; //pelvis
	}

	if (def::aimkeypos == 0)
	{
		def::aimkey = 0x01;//left mouse button
	}
	else if (def::aimkeypos == 1)
	{
		def::aimkey = 0x02;//right mouse button
	}
	else if (def::aimkeypos == 2)
	{
		def::aimkey = 0x04;//middle mouse button
	}
	else if (def::aimkeypos == 3)
	{
		def::aimkey = 0x05;//x1 mouse button
	}
	else if (def::aimkeypos == 4)
	{
		def::aimkey = 0x06;//x2 mouse button
	}
	else if (def::aimkeypos == 5)
	{
		def::aimkey = 0x03;//control break processing
	}
	else if (def::aimkeypos == 6)
	{
		def::aimkey = 0x08;//backspace
	}
	else if (def::aimkeypos == 7)
	{
		def::aimkey = 0x09;//tab
	}
	else if (def::aimkeypos == 8)
	{
		def::aimkey = 0x0c;//clear
	}
	else if (def::aimkeypos == 9)
	{
		def::aimkey == 0x0D;//enter
	}
	else if (def::aimkeypos == 10)
	{
		def::aimkey = 0x10;//shift
	}
	else if (def::aimkeypos == 11)
	{
		def::aimkey = 0x11;//ctrl
	}
	else if (def::aimkeypos == 12)
	{
		def::aimkey == 0x12;//alt
	}
	else if (def::aimkeypos == 13)
	{
		def::aimkey == 0x14;//caps lock
	}
	else if (def::aimkeypos == 14)
	{
		def::aimkey == 0x1B;//esc
	}
	else if (def::aimkeypos == 15)
	{
		def::aimkey == 0x20;//space
	}
	else if (def::aimkeypos == 16)
	{
		def::aimkey == 0x30;//0
	}
	else if (def::aimkeypos == 17)
	{
		def::aimkey == 0x31;//1
	}
	else if (def::aimkeypos == 18)
	{
		def::aimkey == 0x32;//2
	}
	else if (def::aimkeypos == 19)
	{
		def::aimkey == 0x33;//3
	}
	else if (def::aimkeypos == 20)
	{
		def::aimkey == 0x34;//4
	}
	else if (def::aimkeypos == 21)
	{
		def::aimkey == 0x35;//5
	}
	else if (def::aimkeypos == 22)
	{
		def::aimkey == 0x36;//6
	}
	else if (def::aimkeypos == 23)
	{
		def::aimkey == 0x37;//7
	}
	else if (def::aimkeypos == 24)
	{
		def::aimkey == 0x38;//8
	}
	else if (def::aimkeypos == 25)
	{
		def::aimkey == 0x39;//9
	}
	else if (def::aimkeypos == 26)
	{
		def::aimkey == 0x41;//a
	}
	else if (def::aimkeypos == 27)
	{
		def::aimkey == 0x42;//b
	}
	else if (def::aimkeypos == 28)
	{
		def::aimkey == 0x43;//c
	}
	else if (def::aimkeypos == 29)
	{
		def::aimkey == 0x44;//d
	}
	else if (def::aimkeypos == 30)
	{
		def::aimkey == 0x45;//e
	}
	else if (def::aimkeypos == 31)
	{
		def::aimkey == 0x46;//f
	}
	else if (def::aimkeypos == 32)
	{
		def::aimkey == 0x47;//g
	}
	else if (def::aimkeypos == 33)
	{
		def::aimkey == 0x48;//h
	}
	else if (def::aimkeypos == 34)
	{
		def::aimkey == 0x49;//i
	}
	else if (def::aimkeypos == 35)
	{
		def::aimkey == 0x4A;//j
	}
	else if (def::aimkeypos == 36)
	{
		def::aimkey == 0x4B;//k
	}
	else if (def::aimkeypos == 37)
	{
		def::aimkey == 0x4C;//L
	}
	else if (def::aimkeypos == 38)
	{
		def::aimkey == 0x4D;//m
	}
	else if (def::aimkeypos == 39)
	{
		def::aimkey == 0x4E;//n
	}
	else if (def::aimkeypos == 40)
	{
		def::aimkey == 0x4F;//o
	}
	else if (def::aimkeypos == 41)
	{
		def::aimkey == 0x50;//p
	}
	else if (def::aimkeypos == 42)
	{
		def::aimkey == 0x51;//q
	}
	else if (def::aimkeypos == 43)
	{
		def::aimkey == 0x52;//r
	}
	else if (def::aimkeypos == 44)
	{
		def::aimkey == 0x53;//s
	}
	else if (def::aimkeypos == 45)
	{
		def::aimkey == 0x54;//t
	}
	else if (def::aimkeypos == 46)
	{
		def::aimkey == 0x55;//u
	}
	else if (def::aimkeypos == 47)
	{
		def::aimkey == 0x56;//v
	}
	else if (def::aimkeypos == 48)
	{
		def::aimkey == 0x57;//w
	}
	else if (def::aimkeypos == 49)
	{
		def::aimkey == 0x58;//x
	}
	else if (def::aimkeypos == 50)
	{
		def::aimkey == 0x59;//y
	}
	else if (def::aimkeypos == 51)
	{
		def::aimkey == 0x5A;//z
	}
	else if (def::aimkeypos == 52)
	{
		def::aimkey == 0x60;//numpad 0
	}
	else if (def::aimkeypos == 53)
	{
		def::aimkey == 0x61;//numpad 1
	}
	else if (def::aimkeypos == 54)
	{
		def::aimkey == 0x62;//numpad 2
	}
	else if (def::aimkeypos == 55)
	{
		def::aimkey == 0x63;//numpad 3
	}
	else if (def::aimkeypos == 56)
	{
		def::aimkey == 0x64;//numpad 4
	}
	else if (def::aimkeypos == 57)
	{
		def::aimkey == 0x65;//numpad 5
	}
	else if (def::aimkeypos == 58)
	{
		def::aimkey == 0x66;//numpad 6
	}
	else if (def::aimkeypos == 59)
	{
		def::aimkey == 0x67;//numpad 7
	}
	else if (def::aimkeypos == 60)
	{
		def::aimkey == 0x68;//numpad 8
	}
	else if (def::aimkeypos == 61)
	{
		def::aimkey == 0x69;//numpad 9
	}
	else if (def::aimkeypos == 62)
	{
		def::aimkey == 0x6A;//multiply
	}
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		xShutdown();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = D3dDevice->Reset(&d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void xInitD3d()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.hDeviceWindow = Window;
	d3dpp.Windowed = TRUE;
	p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX9_Init(D3dDevice);
	auto& Style = ImGui::GetStyle();
	XorS(font, "C:\\Windows\\Fonts\\Arial.ttf");
	m_pFont = io.Fonts->AddFontFromFileTTF(font.decrypt(), 14.0f, nullptr, io.Fonts->GetGlyphRangesDefault());
	p_Object->Release();
}
std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}


//all calculations 
Vector3 CalculateNewRotation(Vector3& zaz, Vector3& daz)
{
	Vector3 dalte = zaz - daz;
	Vector3 ongle;
	float hpm = sqrtf(dalte.x * dalte.x + dalte.y * dalte.y);
	ongle.y = atan(dalte.y / dalte.x) * 57.295779513082f;
	ongle.x = (atan(dalte.z / hpm) * 57.295779513082f) * -1.f;
	if (dalte.x >= 0.f) ongle.y += 180.f;
	return ongle;
}

void normalize(Vector3& in)
{
	if (in.x > 89.f) in.x -= 360.f;
	else if (in.x < -89.f) in.x += 360.f;

	while (in.y > 180)in.y -= 360;
	while (in.y < -180)in.y += 360;
	in.z = 0;
}

void RadarRange(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

void fortnite_radar(float x, float y, float size, bool rect)
{
	if (radar)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		ImGui::Begin(("##radar"), nullptr, flags);

		float render_size = 6;
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		if (rect) {
			drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + size, y + size), ImGui::GetColorU32({ 0.13f, 0.13f, 0.13f, 1.f }), 0.0f, 0);
			drawList->AddRect(ImVec2(x, y), ImVec2(x + size, y + size), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 0.0f, 0, 1.0f);
		}
		else {
			drawList->AddCircleFilled(ImVec2(x + size / 2, y + size / 2), size, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.5f }), 1000);
			drawList->AddCircleFilled(ImVec2(x + size / 2, y + size / 2), render_size / 2 * sqrt(2), ImGui::GetColorU32({ 0.0f, 1.0f, 0.0f, 1.0f }), 1000);
			drawList->AddCircle(ImVec2(x + size / 2, y + size / 2), size, ImGui::GetColorU32({ 70.0f, 70.0f, 17.0f, 70.0f }), 1000, 1);

			// Add a border around the outer circle
			drawList->AddCircle(ImVec2(x + size / 2, y + size / 2), size, ImGui::GetColorU32({ 200, 200, 200, 255 }), 1000, 1);

			// Add an inner green circle with no border
			drawList->AddCircleFilled(ImVec2(x + size / 2, y + size / 2), render_size / 2 * sqrt(2), ImGui::GetColorU32({ 0.0f, 1.0f, 0.0f, 1.0f }), 1000);
		}

		ImGui::End();
	}
}


void add_players_radar(Vector3 WorldLocation)
{
	if (radar)
	{
		static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		ImGui::Begin(("##radar"), nullptr, flags);
		int ScreenX, ScreenY = 0;
		const auto& GetWindowDrawList = ImGui::GetWindowDrawList();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImVec2 center = ImVec2(ScreenX, ScreenY);
		const float outerRadius = 13.0f;
		const float innerRadius = 6.0f;
		const ImU32 outerColor = ImGui::GetColorU32({ 1.f, 1.f, 1.f, 0.5f });
		const ImU32 innerColor = ImGui::GetColorU32({ 1.f, 0.75f, 0.25f, 0.8f });

		drawList->AddCircle(center, outerRadius, ImGui::GetColorU32({ 0.9f, 0.9f, 0.9f, 1.0f }), 12, 1.0f);
		drawList->AddCircle(center, outerRadius, outerColor, 12, 1.0f);
		drawList->AddCircleFilled(center, innerRadius, innerColor, 12);
		ImGui::End();
	}
}


std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void DrawString(float fontSize, int x, int y, RGBA* color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 153.0, color->B / 51.0, color->A / 255.0)), text.c_str());
}

struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};

using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;
static std::uint32_t _GetProcessId(std::string process_name) {
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE)
		return 0;

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
		if (process_name.compare(processentry.szExeFile) == 0)
			return processentry.th32ProcessID;
	}
	return 0;
}

struct FString : private TArray<WCHAR> {
	FString() {
		Data = nullptr;
		Max = Count = 0;
	}

	FString(LPCWSTR other) {
		Max = Count = static_cast<INT>(wcslen(other));

		if (Count) {
			Data = const_cast<PWCHAR>(other);
		}
	};

	inline BOOLEAN IsValid() {
		return Data != nullptr;
	}

	inline PWCHAR c_str() {
		return Data;
	}
};

VOID(*FreeInternal)(PVOID) = nullptr;

VOID Free(PVOID buffer) {
	FreeInternal(buffer);
}


#pragma once

namespace detail
{
	extern "C" void* _spoofer_stub();

	template <typename Ret, typename... Args>
	static inline auto shellcode_stub_helper(
		const void* shell,
		Args... args
	) -> Ret
	{
		auto fn = (Ret(*)(Args...))(shell);
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct argument_remapper
	{
		template<
			typename Ret,
			typename First,
			typename Second,
			typename Third,
			typename Fourth,
			typename... Pack
		>
		static auto do_call(const void* shell, void* shell_param, First first, Second second,
			Third third, Fourth fourth, Pack... pack) -> Ret
		{
			return shellcode_stub_helper< Ret, First, Second, Third, Fourth, void*, void*, Pack... >(shell, first, second, third, fourth, shell_param, nullptr, pack...);
		}
	};

	template <std::size_t Argc>
	struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>>
	{
		template<
			typename Ret,
			typename First = void*,
			typename Second = void*,
			typename Third = void*,
			typename Fourth = void*
		>
		static auto do_call(
			const void* shell,
			void* shell_param,
			First first = First{},
			Second second = Second{},
			Third third = Third{},
			Fourth fourth = Fourth{}
		) -> Ret
		{
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr
				);
		}
	};
}

template <typename Ret, typename... Args>
static inline auto SpoofCall(Ret(*fn)(Args...), Args... args) -> Ret
{
	static const void* jmprbx = nullptr;
	if (!jmprbx) {
		const auto ntdll = reinterpret_cast<const unsigned char*>(::GetModuleHandleW(NULL));
		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(ntdll);
		const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(ntdll + dos->e_lfanew);
		const auto sections = IMAGE_FIRST_SECTION(nt);
		const auto num_sections = nt->FileHeader.NumberOfSections;

		constexpr char section_name[5]{ '.', 't', 'e', 'x', 't' };
		const auto     section = std::find_if(sections, sections + num_sections, [&](const auto& s) {
			return std::equal(s.Name, s.Name + 5, section_name);
			});

		constexpr unsigned char instr_bytes[2]{ 0xFF, 0x26 };
		const auto              va = ntdll + section->VirtualAddress;
		jmprbx = std::search(va, va + section->Misc.VirtualSize, instr_bytes, instr_bytes + 2);
	}

	struct shell_params
	{
		const void* trampoline;
		void* function;
		void* rdx;
	};

	shell_params p
	{
		jmprbx,
		reinterpret_cast<void*>(fn)
	};

	using mapper = detail::argument_remapper<sizeof...(Args), void>;
	return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
}

namespace SpoofRuntime {
	inline float acosf_(float x)
	{
		return SpoofCall(acosf, x);
	}

	inline float atan2f_(float x, float y)
	{
		return SpoofCall(atan2f, x, y);
	}
}
