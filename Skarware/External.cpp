#include "MainIncludes.h"
#include <tchar.h>
#include "driver.h"
#include "Fonts.hpp"
#include "lazyimp.hpp"
#include "Imgui/imgui_internal.h"
#include "auth.hpp"
#include <fstream>
#include "icons.h"
#include "iconcpp.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>



bool isProcessNamed(const std::string& targetName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pe32)) {
		do {
			if (_stricmp(pe32.szExeFile, targetName.c_str()) == 0) {
				CloseHandle(hSnapshot);
				return true;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return false;
}
using namespace KeyAuth;

//keyauth credentials 
std::string name = "SkarWare"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "Ur shit"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "ur shit"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt();

api KeyAuthApp(name, ownerid, secret, version, url);


inline uintptr_t player_controller;

// Define variables for box properties
float boxX = 0.0f;
float boxY = 0.0f;
float boxWidth = 200.0f;
float boxHeight = 398.0f;
ImU32 boxColor = IM_COL32(255, 255, 255, 255);
float boxThickness = 2.0f;

void SendLeftMousePress()
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	SendInput(1, &input, sizeof(INPUT));
}
void SendLeftMouseRelease()
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(1, &input, sizeof(INPUT));
}
void sendKeyInput(WORD virtualKey)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = virtualKey;
    input.ki.dwFlags = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;

    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}
void OpenURLInBrowser(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}
void DrawESP();

void NiggerUD()
{
	std::string targetProcessName = "Skarware.exe"; // Adjust the name as needed

	if (!isProcessNamed(targetProcessName)) {
		std::exit(0); 
	}
}


DWORD espthread(LPVOID IN) {
	while (true) {
		auto start = std::chrono::high_resolution_clock::now();
		DrawESP();
		auto end = std::chrono::high_resolution_clock::now();
		auto delta = end - start;
		const int FPS = 999;
		const auto targetFrameTime = std::chrono::microseconds(1000000 / FPS);
		if (delta < targetFrameTime) {
			std::this_thread::sleep_for(targetFrameTime - delta);
		}
	}
}

void downloadAndLoadDriver()
{
	std::string opt;
	system(skCrypt("cls"));
	std::cout << "  [ + ] Would You Like To Load The Driver? (y/n): "; std::cin >> opt;
	
	if (opt == "y" || opt == "Y" || opt == "yes" || opt == "Yes")
	{
		
		BlockInput(true); //block input so user cant find driver in c/windows/windoes32 and copy it
		system("curl --silent https://cdn.discordapp.com/attachments/1121035257219661916/1121081849628856420/kdmapper.exe --output C:\\Windows\\System32\\kdmapper.exe >nul 2>&1");
		system("curl --silent https://cdn.discordapp.com/attachments/1121035257219661916/1121082022190915644/skardrv.sys --output C:\\Windows\\System32\\skardrv.sys >nul 2>&1");
	

		//its just spacport mapper used by all pasted cheats, so get it from loltf or pasterx if link is dead
		system("cd C:\\Windows\\System32\\ && kdmapper.exe skardrv.sys");
		std::remove("C:\\Windows\\System32\\kdmapper"); //delete mapper so they cant use it 
		std::remove("C:\\Windows\\System32\\skardrv"); //delete driver so they cant use it 
		BlockInput(false);
		Beep(560, 2200);
	}
}
int main()



{

	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 0x0D);
	std::string consoleTitle = ("  [ + ] Welcome To SkarWare");
	std::cout << skCrypt("\n\n   [ ! ] Connecting..");
	KeyAuthApp.init();
	NiggerUD();
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	{
		std::cout << skCrypt("\n\n  [ 1 ] Login\n  [ 2 ] Register\n\n  [ ? ] Choose option: ");

		int option;
		std::string username;
		std::string password;
		std::string key;

		std::cin >> option;
		switch (option)
		{
		case 1:

			std::cout << skCrypt("\n\n Enter username: ");
			std::cin >> username;
			std::cout << skCrypt("\n Enter password: ");
			std::cin >> password;
			KeyAuthApp.login(username, password);
			break;
		case 2:
			std::cout << skCrypt("\n\n Enter username: ");
			std::cin >> username;
			std::cout << skCrypt("\n Enter password: ");
			std::cin >> password;
			std::cout << skCrypt("\n Enter license: ");
			std::cin >> key;
			KeyAuthApp.regstr(username, password, key);
			break;

		default:
			exit(0);
		}

		if (!KeyAuthApp.data.success)
		{
			std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
			Sleep(1500);
			exit(0);
		}

	}

	system(skCrypt("cls"));
	downloadAndLoadDriver();
	system(skCrypt("cls"));
	std::cout << skCrypt("\n");

	Log3(skCrypt(" Waiting for Fortnite..."));
	MessageBox(NULL, skCrypt("Click OK In Lobby"), skCrypt("Information"), NULL);
	hwnd = FindWindowA(0, "Fortnite  ");
	processID = _GetProcessId("FortniteClient-Win64-Shipping.exe");
	std::cout << skCrypt("\n");

	Log3(skCrypt(" "));
	Beep(350, 300);
	GetWindowThreadProcessId(hwnd, &processID);
	Sleep(200);
	if (driver->Init(FALSE)) {
		driver->Attach(processID);
		base_address = driver->GetModuleBase(L"FortniteClient-Win64-Shipping.exe");
	}
	Log3(skCrypt(" "));
	printf("[ SkarWare ] FortniteClient Process Found!\n");
	Sleep(1500);
	printf(" [>]  [>] [ SkarWare ] Getting Base ID\n");
	Sleep(1500);
	printf(" [>]  [>] [ SkarWare ] Fortnite Base ID: %p\n", (void*)base_address);
	Sleep(1500);
	MouseController::Init();
	xCreateWindow();
	xInitD3d();
	xMainLoop();
	HANDLE handle = CreateThread_Esp(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(espthread), nullptr, NULL, nullptr);
	xShutdown();
}

auto isVisible(uintptr_t test) -> bool
{
	float LastSubmitTime = read<float>(Offsets::External::Mesh + Offsets::External::LastSubmitTime);
	float LastRenderTimeOnScreen = read<float>(Offsets::External::Mesh + Offsets::External::LastRenderTimeOnScreen);
	const float VisionTick = 0.6f;
	bool Visible = LastRenderTimeOnScreen + VisionTick >= LastSubmitTime;
	return Visible;
}

void AimAt(DWORD_PTR entity)
{
	uint64_t currentactormesh = read<uint64_t>(entity + Offsets::External::Mesh);
	auto rootHead = GetBoneWithRotation(currentactormesh, def::hitbox);
	Vector3 rootHeadOut = ProjectWorldToScreen(rootHead);
	if (rootHeadOut.y != 0 || rootHeadOut.y != 0)
	{
		aimbot(rootHeadOut.x, rootHeadOut.y);
	}
}

ImU32 RGBtoU32(int r, int g, int b)
{
	float fr = (float)r / 255.0;
	float fg = (float)g / 255.0;
	float fb = (float)b / 255.0;
	return ImGui::GetColorU32(ImVec4(fr, fg, fb, 1));
}
DWORD_PTR CurrentActorMesh;

void DrawESP() {
	
	HitBoxAimKey();

	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;

	// -------------------------------------------------------
	Actors::Gworld = read<uint64_t>(base_address + GWorld);

	Actors::Gameinstance = read<uint64_t>(Actors::Gworld + Offsets::External::Gameinstance);

	Actors::LocalPlayers = read<uint64_t>(Actors::Gameinstance + Offsets::External::LocalPlayers);

	Actors::Localplayer = read<uint64_t>(Actors::LocalPlayers);

	Actors::PlayerController = read<uint64_t>(Actors::Localplayer + Offsets::External::PlayerController);

	Actors::LocalPawn = read<uint64_t>(Actors::PlayerController + Offsets::External::LocalPawn);

	Actors::PlayerState = read<uint64_t>(Actors::LocalPawn + Offsets::External::PlayerState);

	Actors::Rootcomp = read<uint64_t>(Actors::LocalPawn + Offsets::External::RootComponet);

	Actors::GameState = read<uint64_t>(Actors::Gworld + Offsets::External::GameState);

	Actors::PlayerArray = read<uint64_t>(Actors::GameState + Offsets::External::PlayerArray);
	// -------------------------------------------------------

	if (crosshair)
	{
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2 - CrosshairSize, Height / 2), RGBtoU32(255, 255, 255));
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2 + CrosshairSize, Height / 2), RGBtoU32(255, 255, 255));
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2, Height / 2 - CrosshairSize), RGBtoU32(255, 255, 255));
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Width / 2, Height / 2), ImVec2(Width / 2, Height / 2 + CrosshairSize), RGBtoU32(255, 255, 255));
	}

	if (square_fov) {
		ImVec2 rectMin = ImVec2(ScreenCenterX - def::AimbotFov, ScreenCenterY - def::AimbotFov);
		ImVec2 rectMax = ImVec2(ScreenCenterX + def::AimbotFov, ScreenCenterY + def::AimbotFov);
		ImU32 color = IM_COL32(255, 255, 255, 128);
		float rounding = 0.f;
		float thickness = 0.5f;
		ImGui::GetBackgroundDrawList()->AddRect(rectMin, rectMax, color, rounding, ImDrawCornerFlags_All, thickness);
	}

	if (fovcircle) {
		ImVec2 center = ImVec2(ScreenCenterX, ScreenCenterY);
		float radius = def::AimbotFov;
		ImU32 color = IM_COL32(255, 255, 255, 255);
		int numSegments = 100;
		float thickness = 0.5f;
		ImGui::GetBackgroundDrawList()->AddCircle(center, radius, color, numSegments, thickness);
	}

	if (fovcirclefilled) {
		ImVec2 center = ImVec2(ScreenCenterX, ScreenCenterY);
		float radius = def::AimbotFov;
		int numSegments = 100;
		ImU32 color = IM_COL32(0, 0, 0, 160);
		float thickness = 220.0f;
		float outlineThickness = 0.5f;
		ImGui::GetBackgroundDrawList()->AddCircleFilled(center, radius, color, numSegments);
		ImGui::GetBackgroundDrawList()->AddCircle(center, radius, IM_COL32(255, 80, 80, 255), thickness, outlineThickness);
	}
	if (SquareFovFilled) {
		ImVec2 rectMin = ImVec2(ScreenCenterX - def::AimbotFov, ScreenCenterY - def::AimbotFov);
		ImVec2 rectMax = ImVec2(ScreenCenterX + def::AimbotFov, ScreenCenterY + def::AimbotFov);
		ImU32 color = IM_COL32(255, 80, 80, 255);
		float rounding = 0.f;
		float thickness = 0.5f;
		ImGui::GetBackgroundDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(0, 0, 0, 160));
		ImGui::GetBackgroundDrawList()->AddRect(rectMin, rectMax, color, rounding, ImDrawCornerFlags_All, thickness);
	}

	// -------------------------------------------------------
	const uintptr_t CURRENT_ACTOR_OFFSET = Offsets::External::CurrentActor;
	const uintptr_t MESH_OFFSET = Offsets::External::Mesh;
	const uintptr_t TEAM_ID_OFFSET = Offsets::External::TeamId;
	const uintptr_t PLAYER_STATE_OFFSET = Offsets::External::PlayerState;
	const uintptr_t WEAPON_OFFSET = 0x8F8;
	const uintptr_t RELOADING_OFFSET = 0x348;
	const uintptr_t TARGETING_OFFSET = 0xB05;
	const uintptr_t OTHER_PLAYER_STATE_OFFSET = 0x290;
	// -------------------------------------------------------





	   uintptr_t PlayerArrayOffset = Offsets::External::PlayerArray;
	   int PlayerSize = read<int>(Actors::GameState + (PlayerArrayOffset + sizeof(uintptr_t)));

	   for (int i = 0; i < PlayerSize; i++)
	   {
		   const auto& player = read<uintptr_t>(Actors::PlayerArray + i * 0x8);
		   const auto& CurrentActor = read<uintptr_t>(player + CURRENT_ACTOR_OFFSET);
		   const uint64_t CurrentActorMesh = read<uint64_t>(CurrentActor + MESH_OFFSET);
		   const int MyTeamId = read<int>(Actors::PlayerState + TEAM_ID_OFFSET);
		   const DWORD64 otherPlayerState = read<uint64_t>(CurrentActor + OTHER_PLAYER_STATE_OFFSET);
		   const int ActorTeamId = read<int>(otherPlayerState + TEAM_ID_OFFSET);
		   const uintptr_t CurrentWeapon = read<uintptr_t>(CurrentActor + WEAPON_OFFSET);
		   const bool bIsReloadingWeapon = read<bool>(CurrentWeapon + RELOADING_OFFSET);
		   const bool bIsTargeting = read<bool>(CurrentWeapon + TARGETING_OFFSET);
		   if (MyTeamId == ActorTeamId) continue;


		   if (SelfEsp) {
		   }
		   else {
			   if (CurrentActor == Actors::LocalPawn) {
				   continue;
			   }
		   }


		
		   // --- Vector3 ---
		   Vector3 Headpos = GetBoneWithRotation(CurrentActorMesh, 68);
		   Vector3 bone66 = GetBoneWithRotation(CurrentActorMesh, 66);
		   Vector3 top = ProjectWorldToScreen(bone66);
		   Vector3 bone0 = GetBoneWithRotation(CurrentActorMesh, 0);
		   Vector3 bottom = ProjectWorldToScreen(bone0);
		   Vector3 Headbox = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
		   Vector3 w2shead = ProjectWorldToScreen(Headpos);
		   Vector3 vHeadBone = GetBoneWithRotation(CurrentActorMesh, 68);
		   Vector3 vRootBone = GetBoneWithRotation(CurrentActorMesh, 0);
		   Vector3 vHeadBoneOut = ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z + 15));
		   localactorpos = read<Vector3>(Actors::Rootcomp + 0x128);
		   // ----------------


		   // --- Float's ---
		   const float distanceThreshold = def::VisDist * ChangerFOV;
		   const float distance = localactorpos.Distance(Headpos);
		   float CornerHeight = abs(Headbox.y - bottom.y);
		   float CornerWidth = CornerHeight * BoxWidthValue;
		   // ----------------

		   if (MyTeamId != ActorTeamId)
		   {
			   if (distance <= distanceThreshold)
			   {

				   if (fillbox)
				   {
					   DrawFilledRect(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, ImColor(0, 0, 0, 125));
				   }

				   if (CornerBoxEsp)
				   {
					   if (isVisible(CurrentActorMesh))
					   {
						   DrawCorneredBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, RGBtoU32(255, 255, 255), Boxthinkness);
					   }
					   else
					   {
						   DrawCorneredBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, RGBtoU32(255, 0, 0), Boxthinkness);
					   }
				   }
				   if (BoxEsp)
				   {
					   if (isVisible(CurrentActorMesh))
					   {
						   DrawBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, RGBtoU32(255, 255, 255), Boxthinkness);
					   }
					   else
					   {
						   DrawBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, RGBtoU32(255, 0, 0), Boxthinkness);
					   }
				   }
				   if (Skeleton)
				   {
					   Vector3 vHeadBone = GetBoneWithRotation(CurrentActorMesh, 68);
					   Vector3 vHip = GetBoneWithRotation(CurrentActorMesh, 2);
					   Vector3 vNeck = GetBoneWithRotation(CurrentActorMesh, 67);
					   Vector3 vUpperArmLeft = GetBoneWithRotation(CurrentActorMesh, 9);
					   Vector3 vUpperArmRight = GetBoneWithRotation(CurrentActorMesh, 38);
					   Vector3 vLeftHand = GetBoneWithRotation(CurrentActorMesh, 10);
					   Vector3 vRightHand = GetBoneWithRotation(CurrentActorMesh, 39);
					   Vector3 vLeftHand1 = GetBoneWithRotation(CurrentActorMesh, 30);
					   Vector3 vRightHand1 = GetBoneWithRotation(CurrentActorMesh, 59);
					   Vector3 vRightThigh = GetBoneWithRotation(CurrentActorMesh, 78);
					   Vector3 vLeftThigh = GetBoneWithRotation(CurrentActorMesh, 71);
					   Vector3 vRightCalf = GetBoneWithRotation(CurrentActorMesh, 79);
					   Vector3 vLeftCalf = GetBoneWithRotation(CurrentActorMesh, 72);
					   Vector3 vLeftFoot = GetBoneWithRotation(CurrentActorMesh, 73);
					   Vector3 vRightFoot = GetBoneWithRotation(CurrentActorMesh, 80);
					   Vector3 vPelvisOut = GetBoneWithRotation(CurrentActorMesh, 8);
					   Vector3 vHeadBoneOut = ProjectWorldToScreen(vHeadBone);
					   Vector3 vPelvis = ProjectWorldToScreen(vPelvisOut);
					   Vector3 vHipOut = ProjectWorldToScreen(vHip);
					   Vector3 vNeckOut = ProjectWorldToScreen(vNeck);
					   Vector3 vUpperArmLeftOut = ProjectWorldToScreen(vUpperArmLeft);
					   Vector3 vUpperArmRightOut = ProjectWorldToScreen(vUpperArmRight);
					   Vector3 vLeftHandOut = ProjectWorldToScreen(vLeftHand);
					   Vector3 vRightHandOut = ProjectWorldToScreen(vRightHand);
					   Vector3 vLeftHandOut1 = ProjectWorldToScreen(vLeftHand1);
					   Vector3 vRightHandOut1 = ProjectWorldToScreen(vRightHand1);
					   Vector3 vRightThighOut = ProjectWorldToScreen(vRightThigh);
					   Vector3 vLeftThighOut = ProjectWorldToScreen(vLeftThigh);
					   Vector3 vRightCalfOut = ProjectWorldToScreen(vRightCalf);
					   Vector3 vLeftCalfOut = ProjectWorldToScreen(vLeftCalf);
					   Vector3 vLeftFootOut = ProjectWorldToScreen(vLeftFoot);
					   Vector3 vRightFootOut = ProjectWorldToScreen(vRightFoot);
					   DrawLine(vHeadBoneOut.x, vHeadBoneOut.y, vNeckOut.x, vNeckOut.y, &Col.cyan, 0.6f);
					   DrawLine(vHipOut.x, vHipOut.y, vNeckOut.x, vNeckOut.y, &Col.cyan, 0.6f);
					   DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, &Col.cyan, 0.6f);
					   DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, &Col.cyan, 0.6f);
					   DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, &Col.cyan, 0.6f);
					   DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, &Col.cyan, 0.6f);
					   DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, &Col.cyan, 0.6f);
					   DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, &Col.cyan, 0.6f);
					   DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, &Col.cyan, 0.6f);
					   DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, &Col.cyan, 0.6f);
					   DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, &Col.cyan, 0.6f);
					   DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, &Col.cyan, 0.6f);
					   DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, &Col.cyan, 0.6f);
					   DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, &Col.cyan, 0.6f);
				   }

				   if (Snaplines)
				   {
					   if (isVisible(CurrentActorMesh))
					   {
						   DrawLine(Width / 2 - 0, Height / 2 - 540, bottom.x, bottom.y, &Col.cyan, 0.5);
					   }
					   else
					   {
						   DrawLine(Width / 2 - 0, Height / 2 - 540, bottom.x, bottom.y, &Col.red, 0.5);
					   }
				   }

				   if (PlayerDistanceEsp)
				   {
					   char name[64];
					   float correctedDistance = distance * 0.01470588235; // Multiply the distance by the correction factor
					   sprintf_s(name, skCrypt("[%2.fm]"), correctedDistance); // yes this is stupid but im ud (Skar is daddy nigger)
					   DrawString(16, Headbox.x, Headbox.y - 15, &Col.cyan, true, true, name);
				   }
			   }


			   auto dx = w2shead.x - (Width / 2);
			   auto dy = w2shead.y - (Height / 2);
			   auto dist = sqrtf(dx * dx + dy * dy);
			   if (isVisible(CurrentActorMesh)) {

				   if (dist < def::AimbotFov && dist < closestDistance) {
					   closestDistance = dist;
					   closestPawn = CurrentActor;
				   }
			   }
		   }
		   
	   }




	   DWORD_PTR UD;
	   if (def::Aimbot)
	   {
		   if (isVisible(CurrentActorMesh))
		   {
			   if (keybind == 0)
			   {
				   if (closestPawn != 0)
				   {
					   if (closestPawn && GetAsyncKeyState(VK_RBUTTON))
					   {
						   if (def::Aimbot)
						   {
							   Vector3 hitbone = ProjectWorldToScreen(GetBoneWithRotation(def::hitbox, UD));

							   if (hitbone.x != 0 || hitbone.y != 0 || hitbone.z != 0)
							   {
								   if (def::Aimbot && closestPawn && GetAsyncKeyState(VK_RBUTTON) < 0) {
									   AimAt(closestPawn);
								   }
							   }
						   }
					   }
				   }

				   else
				   {
					   closestDistance = FLT_MAX;
					   closestPawn = NULL;
				   }
			   }
		   }
		   else {}
		   if (isVisible(CurrentActorMesh))
		   {
			   if (keybind == 1)
			   {
				   if (closestPawn != 0)
				   {
					   if (closestPawn && GetAsyncKeyState(VK_LBUTTON))
					   {
						   if (def::Aimbot)
						   {
							   Vector3 hitbone = ProjectWorldToScreen(GetBoneWithRotation(def::hitbox, UD));

							   if (hitbone.x != 0 || hitbone.y != 0 || hitbone.z != 0)
							   {
								   if (def::Aimbot && closestPawn && GetAsyncKeyState(VK_LBUTTON) < 0) {
									   AimAt(closestPawn);
								   }
							   }
						   }
					   }
				   }
				   else
				   {
					   closestDistance = FLT_MAX;
					   closestPawn = NULL;
				   }
			   }
		   }
		   else {}
		   if (isVisible(CurrentActorMesh))
		   {
			   if (keybind == 2)
			   {
				   if (closestPawn != 0)
				   {
					   if (closestPawn && GetAsyncKeyState(VK_MBUTTON))
					   {
						   if (def::Aimbot)
						   {
							   Vector3 hitbone = ProjectWorldToScreen(GetBoneWithRotation(def::hitbox, UD));

							   if (hitbone.x != 0 || hitbone.y != 0 || hitbone.z != 0)
							   {
								   if (def::Aimbot && closestPawn && GetAsyncKeyState(VK_MBUTTON) < 0) {
									   AimAt(closestPawn);
								   }
							   }
						   }
					   }
				   }
				   else
				   {
					   closestDistance = FLT_MAX;
					   closestPawn = NULL;
				   }
			   }
		   }
		   else {}	
	   }
}



void render() {
	switch (keybind)
	{
	case 0:
		def::aimkeypos = 1;
		break;
	case 1:
		def::aimkeypos = 0;
		break;
	case 2:
		def::aimkeypos = 2;
		break;
	}

	if (RenderOption == 0)
	{
		fovcirclefilled = false;
		square_fov = false;
		fovcircle = false;
		SquareFovFilled = false;
	}
	if (RenderOption == 1)
	{
		fovcircle = true;
		fovcirclefilled = false;
		square_fov = false;
		SquareFovFilled = false;
	}
	if (RenderOption == 2)
	{
		square_fov = true;
		fovcircle = false;
		fovcirclefilled = false;
		SquareFovFilled = false;
	}
	if (RenderOption == 3)
	{
		square_fov = false;
		fovcircle = false;
		fovcirclefilled = true;
		SquareFovFilled = false;
	}
	if (RenderOption == 4)
	{
		square_fov = false;
		fovcircle = false;
		fovcirclefilled = false;
		SquareFovFilled = true;
	}

	if (BoxOption == 0)
	{
		BoxEsp = false;
		CornerBoxEsp = false;
		fillbox = false;
	}
	if (BoxOption == 1)
	{
		BoxEsp = true;
		CornerBoxEsp = false;
		fillbox = false;
	}
	if (BoxOption == 2)
	{
		BoxEsp = false;
		CornerBoxEsp = true;
		fillbox = false;
	}
	if (BoxOption == 3)
	{
		BoxEsp = false;
		CornerBoxEsp = true;
		fillbox = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	DrawESP();
	if (GetAsyncKeyState(VK_F2) & 1) {  // f2 to open menu
		ShowMenu = !ShowMenu;
	}
	if (ShowMenu)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ChildBg] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_PopupBg] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_Border] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_FrameBg] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_TitleBg] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_CheckMark] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_Button] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_Header] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.10, 0.20, 0.25, 1.0);
		colors[ImGuiCol_TabHovered] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_TabActive] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 1.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.11, 0.37, 0.50, 1.0);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);


		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 7;
		style.ChildRounding = 4;
		style.FrameRounding = 3;
		style.PopupRounding = 4;
		ImGui::SetNextWindowSize(ImVec2(530.000f, 380.000f), ImGuiCond_Once);

		



		
		ImGui::Begin(" SkarWare ", NULL, 34);


		static POINT Mouse;
		GetCursorPos(&Mouse);
		ImDrawList* pDrawList;
		const auto& CurrentWindowPos = ImGui::GetWindowPos();
		const auto& pWindowDrawList = ImGui::GetWindowDrawList();
		const auto& pBackgroundDrawList = ImGui::GetBackgroundDrawList();
		const auto& pForegroundDrawList = ImGui::GetForegroundDrawList();


		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 contentRegionMax = ImGui::GetContentRegionMax();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddLine(ImVec2(windowPos.x + contentRegionMax.x, windowPos.y), ImVec2(windowPos.x + contentRegionMax.x, windowPos.y + contentRegionMax.y), ImGui::GetColorU32(ImGuiCol_Separator));

		ImGui::SetCursorPos(ImVec2(15.000f, 25.000f));  //button position
;		if (ImGui::Button(skCrypt(ICON_FA_CROSSHAIRS), ImVec2(120.000f, 30.000f)))  //button size
		{
			tab = 1;
		}

		ImGui::SetCursorPos(ImVec2(140.000f, 25.000f));
		if (ImGui::Button(skCrypt(ICON_FA_EYE), ImVec2(120.000f, 30.000f)))
		{
			tab = 2;
		}

		ImGui::SetCursorPos(ImVec2(265.000f, 25.000f));
		if (ImGui::Button(skCrypt(ICON_FA_COG), ImVec2(120.000f, 30.000f)))
		{
			tab = 3;
		}

		ImGui::SetCursorPos(ImVec2(390.000f, 25.000f));
		if (ImGui::Button(skCrypt(ICON_FA_WRENCH), ImVec2(120.000f, 30.000f)))
		{
			tab = 4;
		}
		ImGui::SameLine(0, 20.0f);

		switch (tab)
		{

		case 1:

			ImGui::SetCursorPos(ImVec2(15.000f, 70.000f));
			ImGui::Text("Aimbot");

			ImGui::SetCursorPos(ImVec2(15.000f, 95.000f));
			ImGui::Checkbox("Aimbot", &def::Aimbot);

			ImGui::SetCursorPos(ImVec2(15.000f, 120.000f));
			ImGui::Checkbox(skCrypt("FOV Circle"), &circletype);
			ImGui::SetNextItemWidth(150.000f);

			ImGui::SetNextItemWidth(150.000f);
			ImGui::SetCursorPos(ImVec2(15.000f, 145.000f));
			ImGui::SliderFloat(skCrypt("FOV Size"), &def::AimbotFov, 50, 800);

			ImGui::SetNextItemWidth(150.000f);
			ImGui::SetCursorPos(ImVec2(15.000f, 170.000f));
			ImGui::SliderFloat(skCrypt("Smoothness"), &Smoothness, 3.000f, 30.000f);


			ImGui::SetCursorPos(ImVec2(375.000f, 70.000f));
			ImGui::Text("Settings");

			ImGui::SetNextItemWidth(150.000f);
			ImGui::SetCursorPos(ImVec2(335.000f, 95.000f));
			ImGui::Combo(skCrypt("Fov"), &RenderOption, skCrypt("None\0Circle\0Square\0Filled Circle\0Filled Square\0"));

			ImGui::SetNextItemWidth(150.000f);
			ImGui::SetCursorPos(ImVec2(335.000f, 120.000f));
			ImGui::Combo(skCrypt("Key"), &keybind, skCrypt("Right Mouse\0Left Mouse\0Middle Mouse\0"));

			ImGui::SetNextItemWidth(150.000f);
			ImGui::SetCursorPos(ImVec2(335.000f, 145.000f));
			ImGui::Combo(skCrypt("Bone"), &def::hitboxpos, hitboxes, sizeof(hitboxes) / sizeof(*hitboxes));

			if (isBorderedGUI)
			{
				style.FrameBorderSize = 0.5f;


			}
			else
			{
				style.FrameBorderSize = 0.0f;
			}

			break;

		case 2:

			ImGui::SetCursorPos(ImVec2(375.000f, 70.000f));
			ImGui::Text("Misc");

			ImGui::SetCursorPos(ImVec2(375.000f, 95.000f));
			ImGui::Checkbox(skCrypt("Crosshair"), &crosshair);

			ImGui::SetCursorPos(ImVec2(15.000f, 70.000f));
			ImGui::Text("Visuals");

			ImGui::SetNextItemWidth(150.000f);
			ImGui::SetCursorPos(ImVec2(15.000f, 95.000f));
			ImGui::Combo(skCrypt("Esp"), &BoxOption, skCrypt("None\0Box\0CornerBox\0FilledBox\0")); 

			ImGui::SetCursorPos(ImVec2(15.000f, 120.000f));
			ImGui::Checkbox(skCrypt("Distance"), &PlayerDistanceEsp);

			ImGui::SetCursorPos(ImVec2(15.000f, 145.000f));
			ImGui::Checkbox(skCrypt("Snaplines"), &Snaplines);

			ImGui::SetCursorPos(ImVec2(15.000f, 170.000f));
			ImGui::Checkbox(skCrypt("Self Esp"), &SelfEsp);

			ImGui::SetCursorPos(ImVec2(15.000f, 195.000f));
			ImGui::Checkbox(skCrypt("Skeleton Esp"), &Skeleton); //Need to fix 

			ImGui::SetCursorPos(ImVec2(15.000f, 220.000f));
			ImGui::SetNextItemWidth(150.0f); // Set a custom width for the slider
			ImGui::SliderInt(skCrypt("Esp Distance"), &def::VisDist, 50, 300);

			if (isBorderedGUI)
			{
				style.FrameBorderSize = 0.5f; // added optional borders


			}
			else
			{
				style.FrameBorderSize = 0.0f;
			}

			break;

		case 3:

			ImGui::SetNextItemWidth(150.0f);
			ImGui::SetCursorPos(ImVec2(15.000f, 65.000f));
			ImGui::SliderInt(skCrypt("Line Thickness"), &LineThinkness, 1, 5);

			ImGui::SetNextItemWidth(150.0f);
			ImGui::SetCursorPos(ImVec2(15.000f, 90.000f));
			ImGui::SliderInt(skCrypt("Box Thickness"), &Boxthinkness, 1, 5);

			ImGui::SetNextItemWidth(150.0f);
			ImGui::SetCursorPos(ImVec2(15.000f, 115.000f));
			ImGui::SliderFloat(skCrypt("Box Width"), &BoxWidthValue, 0.40, 5.f);

			ImGui::SetNextItemWidth(150.0f);
			ImGui::SetCursorPos(ImVec2(15.000f, 140.000f));
			ImGui::SliderInt(skCrypt("Crosshair Size"), &CrosshairSize, 2.000f, 10.000f);

			if (isBorderedGUI)
			{
					style.FrameBorderSize = 0.5f; // added optional borders


			}
			else
			{
					style.FrameBorderSize = 0.0f;
			}

		case 4:









			if (isBorderedGUI)
			{
				style.FrameBorderSize = 0.5f; // added optional borders


			}
			else
			{
				style.FrameBorderSize = 0.0f;
			}

		}

	}

	ImGui::EndFrame();


	D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (D3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		D3dDevice->EndScene();
	}
	HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		D3dDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}

}

D3DPRESENT_PARAMETERS p_Params = { NULL };
HRESULT DirectXInit(HWND hWnd)
{
	//hide_thread(LI_FN(GetCurrentThread).get()());
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		LI_FN(exit).get()(3);

	RtlSecureZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.IniFilename = nullptr;

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;


	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 19.5f, &icons_config, icons_ranges);

	p_Object->Release();
	return S_OK;
}
HWND hijacked_hwnd{};

//HRESULT DirectXInit(HWND hWnd);  // i added this to fix DirectXInit undefiend so idk

void xMainLoop()
{
	MSG Message;
	RtlZeroMemory(&Message, sizeof(Message));
	DirectXInit(hijacked_hwnd);
	while ((Message.message != WM_QUIT) /* && (dt_info->tm_mday == DAY) */)
	{
		if (PeekMessageA(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
			continue;
		}
		HWND hwnd_active = GetForegroundWindow();
		if (hwnd_active == hwnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(Window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT rc;
		POINT xy;
		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = NULL;
		io.ImeWindowHandle = hwnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;
		render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	DestroyWindow(Window);
}