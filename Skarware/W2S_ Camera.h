namespace Actors
{
	DWORD_PTR Gworld;
	DWORD_PTR Rootcomp;
	DWORD_PTR Localplayer;
	DWORD_PTR PlayerController;
	DWORD_PTR LocalPawn;
	DWORD_PTR PlayerState;
	DWORD_PTR Persistentlevel;
	uintptr_t PlayerCameraManager;
	DWORD_PTR GameState;
	DWORD_PTR PlayerArray;
	DWORD_PTR LocalPlayers;
	uintptr_t Gameinstance;
}

#define PI 3.14159265358979323846f

struct _MATRIX
{
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};

	_MATRIX() : _11(1.f), _12(0.f), _13(0.f), _14(0.f),
		_21(0.f), _22(1.f), _23(0.f), _24(0.f),
		_31(0.f), _32(0.f), _33(1.f), _34(0.f),
		_41(0.f), _42(0.f), _43(0.f), _44(1.f) {}
};

_MATRIX Matrix(Vector3 euler, Vector3 Vec4, Vector3 origin = Vector3(0, 0, 0))
{
	const float radPitch = euler.x * PI / 180.f;
	const float radYaw = euler.y * PI / 180.f;
	const float radRoll = euler.z * PI / 180.f;
	const float SP = sinf(radPitch);
	const float CP = cosf(radPitch);
	const float SY = sinf(radYaw);
	const float CY = cosf(radYaw);
	const float SR = sinf(radRoll);
	const float CR = cosf(radRoll);

	_MATRIX mat;

	mat.m[0][0] = CP * CY;
	mat.m[0][1] = CP * SY;
	mat.m[0][2] = SP;
	mat.m[0][3] = 0.f;

	mat.m[1][0] = SR * SP * CY - CR * SY;
	mat.m[1][1] = SR * SP * SY + CR * CY;
	mat.m[1][2] = -SR * CP;
	mat.m[1][3] = 0.f;

	mat.m[2][0] = -(CR * SP * CY + SR * SY);
	mat.m[2][1] = CY * SR - CR * SP * SY;
	mat.m[2][2] = CR * CP;
	mat.m[2][3] = 0.f;

	mat.m[3][0] = origin.x;
	mat.m[3][1] = origin.y;
	mat.m[3][2] = origin.z;
	mat.m[3][3] = 1.f;

	return mat;
}

struct CamewaDescwipsion
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
	char Useless[0x18];
};

CamewaDescwipsion GetViewPoint()
{
	char v1; // r8
	CamewaDescwipsion ViewPoint = read<CamewaDescwipsion>(base_address + 0xE9AD420);
	BYTE* v2 = (BYTE*)&ViewPoint;
	int i; // edx
	__int64 result; // rax

	v1 = 0x40;
	for (i = 0; i < 0x40; ++i)
	{
		*v2 ^= v1;
		result = (unsigned int)(i + 0x17);
		v1 += i + 0x17;
		v2++;
	}

	return ViewPoint;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	CamewaDescwipsion vCamera = GetViewPoint();
	_MATRIX tempMatrix = Matrix(vCamera.Rotation, Vector3(0, 0, 0));
	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);
	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));
	if (vTransformed.z < 1.f) vTransformed.z = 1.f;
	return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}