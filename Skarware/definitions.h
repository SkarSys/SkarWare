#include <Windows.h>
#include <iostream>
#define ViewYawMin 0x331C
#define ViewYawMax 0x3320
#define ViewPitchMin 0x3314
#define ViewPitchMax 0x3318
#define AimPitchMin 0x1138
#define AimPitchMax 0x113C
namespace def
{
	// Aimbot Definition's
	bool Aimbot = false;
	static int aimkey;
	static int hitbox;
	float AimbotFov = 150;
	static int VisDist = 50;
	static int aimkeypos = 1;
	static int hitboxpos = 0;
}

// Fov - Crosshair - AimKey
bool fovcircle = false;
bool square_fov = false;
bool crosshair = false;
bool fovcirclefilled = false;
bool circletype = false;
int keybind = 0;
bool SquareFovFilled = false; 
int CrosshairSize = 5;


// Esp
bool Esp = true;
bool drawbase = true;
bool SelfEsp = false;
bool PlayerDistanceEsp = false;
bool Snaplines = false;
bool thick = false;
float BoxWidthValue = 0.550;
int LineThinkness = 1;
int Boxthinkness = 0.5;
float ChangerFOV = 80;
bool Skeleton = false;
bool macro = false;
char keyInput[32];
int selectedKeyIndex = 0;

//BOX OPTION

bool CornerBoxEsp = false;
bool BoxEsp = false;
bool fillbox = false;

//BOX OPTION



// Other
int RenderOption = 0;
int tab;
bool WindowStreamProof = false;
bool isLightTheme = false;
bool isHightContrastTheme = false;
bool isHightContrastOrangeTheme = true;
bool isBorderedGUI = true;

int BoxOption = 0;

// radar
bool radar = false;
bool rect_radar = false;
float radar_position_x{ 84.494f };
float radar_position_y{ 77.841f };
float radar_size{ 150.f };
float RadarDistance = { 700.f };

// Menu
bool ShowMenu = true;
