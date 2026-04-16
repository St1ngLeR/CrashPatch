#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <random>
#include <fstream>
#include <string>

DWORD p_CDDir = 0x7E5FF0;
DWORD p_CDRender = 0x7CF700;
DWORD p_Player = 0x78CEE0;
DWORD p_CDRace = 0x7DEA20;
DWORD p_CDRaceInfo = 0x78D5F0;
DWORD p_PlayersCount = 0x78CEDC;
DWORD p_CDControl = 0x78D5F4;
DWORD p_CDNetwork = 0x7CD0E0;
DWORD p_CrashGUI = 0x78D65C;

float old_car_timer;

int player_id0;

std::random_device rd;

enum class CDPlayer
{
	NumMeshes = 0x108,
	PosX = 0xB14,
	PosY = 0xB18,
	PosZ = 0xB1C,
	PhysicsHandler = 0xB74,
	GravMultiplier = 0x948,
	Physics = 0x95B,
	WorldCollision = 0x95D,
	MinRPM = 0x4950,
	CarMaxSpeed = 0x4964,
	EnergyLoss = 0x65F0,
	DeformFactor = 0x65FC,
	SteerMultiplier = 0x688C,
	DamageRealism = 0x6894,
	CurWheelsAngle = 0x69D8,
	AftburPtr = 0x6E80,
	CarEnginePtr = 0x6E78,
	MinigunPtr = 0x6E7C,
	CarCurSpeed = 0x6EB8,
	CarHealth = 0x6EBC,
	MissileAmmo = 0x7310,
	AI_Debug_Nodes = 0x3E348,
	AI_Debug_Waypoints = 0x3E34D,
	AI_Debug_CarPassing = 0x3E34E,
	AI_Debug_TweakFree = 0x3E34F,
	AutomaticShifting = 0x6888,
	WheelPtr = 0x68CC,
	AxleFPtr = 0x6E70,
	AxleRPtr = 0x6E74,
	WheelFLPosX = 0x6EC4,
	WheelFLPosY = 0x6EC8,
	WheelFLPosZ = 0x6ECC,
	WheelFRPosX = 0x6ED0,
	WheelFRPosY = 0x6ED4,
	WheelFRPosZ = 0x6ED8,
	WheelRLPosX = 0x6EDC,
	WheelRLPosY = 0x6EE0,
	WheelRLPosZ = 0x6EE4,
	WheelRRPosX = 0x6EE8,
	WheelRRPosY = 0x6EEC,
	WheelRRPosZ = 0x6EF0,
	CarMsgControl = 0x6E9A,
	RecoveryState = 0x72C8,
	CarTimer = 0x72F4,
	BombState = 0x97E8,
	Surface01 = 0x1AA4,
	Surface02 = 0x1B14,
	Surface03 = 0x1B84,
	Surface04 = 0x1BF4,
	SpringLoadFL = 0x6A2C,
	SpringLoadFR = 0x6B94,
	SpringLoadRL = 0x6CFC,
	SpringLoadRR = 0x6E64,
};

enum class CDAftbur
{
	HeatRiseSpeed = 0x3E8,
	HeatCoolDownSpeed = 0x3EC,
	Acceleration = 0x3E4
};

enum class CDMinigun
{
	Ammo = 0x4874
};

enum class CDWheel
{
	Health = 0x4B50,
	SurfaceType = 0x4B2C,
	LatForce = 0x4860,
	TractionAt90 = 0x4864
};

enum class CDCarEngine
{
	Gear1 = 0x2FC,
	Gear2 = 0x2FD,
	CurRPM1 = 0x30C,
	CurRPM2 = 0x310
};

enum class CDGameMode
{
	WreckingMatch = 0,
	StuntShow = 1,
	BombRun = 2,
	Race = 3,
	TestDrive = 4,
	HoldTheFlag = 5,
	PassTheBomb = 6,
	MiniGames = 7
};

enum class CDRaceState
{
	Waiting = 1,
	Countdown = 2,
	InProcess = 3,
	Finished = 4
};

enum class CDEventType
{
	stunt_timelimit = 0,
	stunt_timelimit_team = 1,
	stunt_targetscore = 2,
	stunt_targetscore_team = 3,
	speed_default = 14,
	race_laps = 4,
	race_ptp = 5,
	race_ko = 6,
	ctf_default = 11,
	ctf_team = 12,
	ptb_default = 13,
	minigame_longjump = 16,
	minigame_vehicleblast = 17,
	minigame_checkpointchase = 18,
	minigame_maxdestruction = 19,
	wreck_lms = 7,
	wreck_dm = 8,
	wreck_dm_team = 9,
	wreck_advance = 10,
	testdrive_default = 15
};

enum class CDRaceCamera
{
	ChaseNear = 0,
	ChaseFar = 1,
	Cockpit = 2,
	Roof = 3,
	Hood = 4,
	Bumper = 5,
	Intro = 6,
	Outro = 7,
	RearView = 8,
	Custom1 = 9,
	Custom2 = 10,
	TVStuntShow = 12,
	TVCarCloseUps = 13,
	TVHelicopter = 14,
	TVDriver = 15,
	FreeCarCam = 16,
	FreeTrackCam = 17
};

enum class CDFreeTrackCamCtrl
{
	PosX = 0x20,
	PosY = 0x24,
	PosZ = 0x28,
	Yaw = 0x2C,
	Pitch = 0x30,
	Roll = 0x38,
	FOV = 0x34
};

struct Quaternion
{
	float w, x, y, z;
};

struct Matrix4x4
{
	float data[4][4];
};

float degreesToRadians(float degrees)
{
	return degrees * M_PI / 180.0;
}

Quaternion eulerToQuaternion(float roll_deg, float pitch_deg, float yaw_deg)
{
	float roll = degreesToRadians(roll_deg);
	float pitch = degreesToRadians(pitch_deg);
	float yaw = degreesToRadians(yaw_deg);

	float cy = cos(yaw * 0.5);
	float sy = sin(yaw * 0.5);
	float cp = cos(pitch * 0.5);
	float sp = sin(pitch * 0.5);
	float cr = cos(roll * 0.5);
	float sr = sin(roll * 0.5);

	Quaternion q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

Matrix4x4 quaternionToMatrix4x4(const Quaternion& q)
{
	Matrix4x4 mat;

	float norm = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	float w = q.w / norm;
	float x = q.x / norm;
	float y = q.y / norm;
	float z = q.z / norm;

	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	mat.data[0][0] = 1.0 - 2.0 * (yy + zz);
	mat.data[0][1] = 2.0 * (xy - wz);
	mat.data[0][2] = 2.0 * (xz + wy);
	mat.data[0][3] = 0.0;

	mat.data[1][0] = 2.0 * (xy + wz);
	mat.data[1][1] = 1.0 - 2.0 * (xx + zz);
	mat.data[1][2] = 2.0 * (yz - wx);
	mat.data[1][3] = 0.0;

	mat.data[2][0] = 2.0 * (xz - wy);
	mat.data[2][1] = 2.0 * (yz + wx);
	mat.data[2][2] = 1.0 - 2.0 * (xx + yy);
	mat.data[2][3] = 0.0;

	mat.data[3][0] = 0.0;
	mat.data[3][1] = 0.0;
	mat.data[3][2] = 0.0;
	mat.data[3][3] = 1.0;

	return mat;
}

Matrix4x4 rotationToMatrix4x4(float roll_deg, float pitch_deg, float yaw_deg)
{
	Quaternion q = eulerToQuaternion(roll_deg, pitch_deg, yaw_deg);
	return quaternionToMatrix4x4(q);
}

void WriteMemoryMatrix4x4(uintptr_t addr, Matrix4x4 mat, bool vp = false)
{
	injector::WriteMemory(addr + 0x0, mat.data[0][0], vp);
	injector::WriteMemory(addr + 0x4, mat.data[0][1], vp);
	injector::WriteMemory(addr + 0x8, mat.data[0][2], vp);
	injector::WriteMemory(addr + 0xC, mat.data[0][3], vp);

	injector::WriteMemory(addr + 0x10, mat.data[1][0], vp);
	injector::WriteMemory(addr + 0x14, mat.data[1][1], vp);
	injector::WriteMemory(addr + 0x18, mat.data[1][2], vp);
	injector::WriteMemory(addr + 0x1C, mat.data[1][3], vp);

	injector::WriteMemory(addr + 0x20, mat.data[2][0], vp);
	injector::WriteMemory(addr + 0x24, mat.data[2][1], vp);
	injector::WriteMemory(addr + 0x28, mat.data[2][2], vp);
	injector::WriteMemory(addr + 0x2C, mat.data[2][3], vp);
}

void __declspec(naked) RepairPlayerCar()
{
	__asm
	{
		push ebx
		mov ebx, [player_id0]
		mov eax, ebx
		pop ebx

		push 0x44BA60
		retn
	}
}

void __declspec(naked) RespawnOnCPPlayerCar()
{
	__asm
	{
		push ebx
		mov ebx, [player_id0]
		mov eax, ebx
		pop ebx

		push 0x507400
		retn
	}
}

template<typename T>
void WriteString(uintptr_t address, const char* text, bool vp = true)
{
	size_t len = strlen(text) + 1;
	size_t i = 0;

	for (; i + sizeof(T) <= len; i += sizeof(T)) {
		T chunk = *(T*)(text + i);
		injector::WriteMemory<T>(address + i, chunk, vp);
	}

	for (; i < len; i++) {
		injector::WriteMemory<uint8_t>(address + i, text[i], vp);
	}
}

std::string GetAmbEnvSoundFilePath(const std::string& filePath)
{
	std::ifstream file(filePath);
	std::string line;
	int currentLine = 0;

	if (!file.is_open())
	{
		return "";
	}

	while (std::getline(file, line) && currentLine < 28)
	{
		currentLine++;
	}

	if (currentLine < 28)
	{
		return "";
	}

	if (!line.empty())
	{
		size_t commentPos = line.find('#');
		if (commentPos != std::string::npos)
		{
			line = line.substr(0, commentPos);
		}

		while (!line.empty() && std::isspace(static_cast<unsigned char>(line.back())))
		{
			line.pop_back();
		}

		std::string charsToRemove = "\t\r\n ";
		while (!line.empty() && charsToRemove.find(line.back()) != std::string::npos)
		{
			line.pop_back();
		}
	}

	file.close();
	return line;
}

float StringToFloat(std::string s)
{
	std::replace(s.begin(), s.end(), ',', '.');
	return std::stof(s.c_str());
}

std::string GetStringSimple(const void* address)
{
	const char* ptr = static_cast<const char*>(address);
	std::string result;

	while (*ptr != '\0')
	{
		result.push_back(*ptr);
		++ptr;
	}

	return result;
}

std::string GetString(void* addr)
{
	if (addr == nullptr)
		return "";

	// ¬место reinterpret_cast<uintptr_t>(addr) читаем значение указател€ как uintptr_t из локальной переменной
	uintptr_t base = injector::ReadMemory<uintptr_t>(&addr);

	const uint32_t MAX_STRING_LENGTH = 1024 * 1024;

	auto IsReadableRange = [](const void* ptr, size_t size) -> bool {
		if (ptr == nullptr || size == 0)
			return false;
		uintptr_t start = reinterpret_cast<uintptr_t>(ptr);
		uintptr_t end = start + size;
		if (end < start)
			return false;
		uintptr_t current = start;
		while (current < end) {
			MEMORY_BASIC_INFORMATION mbi;
			if (VirtualQuery(reinterpret_cast<LPCVOID>(current), &mbi, sizeof(mbi)) == 0)
				return false;
			if (mbi.Protect == PAGE_NOACCESS ||
				mbi.Protect == PAGE_EXECUTE ||
				mbi.Protect & PAGE_GUARD)
				return false;
			current = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
		}
		return true;
		};

	uintptr_t lenAddr = base + 0x8;
	if (!IsReadableRange(reinterpret_cast<void*>(lenAddr), sizeof(uint32_t)))
		return "";

	uint32_t length = injector::ReadMemory<uint32_t>(lenAddr); // пр€мое разыменование
	if (length == 0 || length > MAX_STRING_LENGTH)
		return "";

	uintptr_t strAddr = base + 0xC;
	if (!IsReadableRange(reinterpret_cast<void*>(strAddr), length))
		return "";

	const char* pData = reinterpret_cast<const char*>(strAddr);
	uint32_t realLen = length;
	for (uint32_t i = 0; i < length; ++i) {
		if (pData[i] == '\0') {
			realLen = i;
			break;
		}
	}
	return std::string(pData, realLen);
}

//std::string GetString(void* addr)
//{
//	if (addr == nullptr)
//	{
//		return "";
//	}
//
//	try
//	{
//		uintptr_t base = reinterpret_cast<uintptr_t>(addr);
//
//		uint32_t* p_length = reinterpret_cast<uint32_t*>(base + 0x8);
//		uint32_t length = *p_length;
//
//		char* p_string_start = reinterpret_cast<char*>(base + 0xC);
//
//		std::string resultstr(p_string_start, length);
//
//		return resultstr;
//	}
//	catch (...)
//	{
//		return "";
//	}
//}

void SetString(void* baseAddress, const char* text)
{
	if (!baseAddress || !text)
	{
		return;
	}

	uint32_t* lengthPtr = (uint32_t*)baseAddress + 0x8;

	char* textPtr = reinterpret_cast<char*>
		(
		reinterpret_cast<char*>(baseAddress) + 0xC
		);

	*lengthPtr = static_cast<uint32_t>(strlen(text));

	strcpy(textPtr, text);
}

template<typename... Args>
std::string FormatStr(const std::string& text, Args... args)
{
	int size = std::snprintf(nullptr, 0, text.c_str(), args...);
	if (size < 0) {
		return text; // ќшибка форматировани€
	}

	std::vector<char> buffer(size + 1);
	std::snprintf(buffer.data(), buffer.size(), text.c_str(), args...);

	return std::string(buffer.data());
}

int GetWindowWidth()
{
	return injector::ReadMemory<int>(injector::ReadMemory<DWORD>(0x7CF6FC, true) + 0x250, false);
}

int GetWindowHeight()
{
	return injector::ReadMemory<int>(injector::ReadMemory<DWORD>(0x7CF6FC, true) + 0x254, false);
}

std::string CDDir()
{
	if (injector::ReadMemory<void*>(p_CDDir) != NULL)
	{
		void* addr = injector::ReadMemory<void*>(injector::ReadMemory<void*>(p_CDDir));

		return GetString(addr);
	}
}

DWORD CrashGUI()
{
	return injector::ReadMemory<DWORD>(p_CrashGUI);
}

BYTE GetLoadingState()
{
	return injector::ReadMemory<BYTE>(0x70EB5C);
}

DWORD CDNetwork()
{
	return injector::ReadMemory<DWORD>(p_CDNetwork);
}

bool IsServer()
{
	if (CDNetwork())
	{
		return !injector::ReadMemory<bool>(CDNetwork() + 0x4, false);
	}
}

std::string GetMPPlayerName(int id)
{
	if (CDNetwork())
	{
		return GetString((void*)(0x7A77C8 + (0x40 * id) + 4));
	}
}

BYTE GetCurPlayerInMPList()
{
	if (CDNetwork())
	{
		return injector::ReadMemory<BYTE>(CDNetwork() + 0xC, false);
	}
}

void SetCurPlayerInMPList(int id)
{
	if (CDNetwork())
	{
		injector::WriteMemory<BYTE>(CDNetwork() + 0xC, id, false);
	}
}

BYTE GetMPPlayersCount()
{
	if (CDNetwork())
	{
		return injector::ReadMemory<BYTE>(CDNetwork() + 0xE, false);
	}
}

bool IsVotingForRestart(int id)	// works only for host :(
{
	if (CDNetwork())
	{
		return injector::ReadMemory<bool>(CDNetwork() + 0xE4 + id, false);
	}
}

bool IsVotingForRaceEnd(int id)	// works only for host :(
{
	if (CDNetwork())
	{
		return injector::ReadMemory<bool>(CDNetwork() + 0xEC + id, false);
	}
}

bool IsRestartComing()	// works only for host :(
{
	if (CDNetwork())
	{
		return injector::ReadMemory<bool>(CDNetwork() + 0x134, false);
	}
}

bool IsRaceEndComing()	// works only for host :(
{
	if (CDNetwork())
	{
		return injector::ReadMemory<bool>(CDNetwork() + 0x135, false);
	}
}

DWORD CDRace()
{
	//return *(DWORD*)p_CDRace;
	return injector::ReadMemory<DWORD>(p_CDRace);
}

DWORD CDRaceInfo()
{
	//return *(DWORD*)p_CDRaceInfo;
	return injector::ReadMemory<DWORD>(p_CDRaceInfo);
}

DWORD CDRender()
{
	return injector::ReadMemory<DWORD>(p_CDRender);
}

DWORD CDControl()
{
	return injector::ReadMemory<DWORD>(p_CDControl);
}

float GetGameSpeed()
{
	return injector::ReadMemory<float>(injector::ReadMemory<DWORD>(0x7DC550, true) + 0x8, false);
}

int GetPlayersCount()
{
	//return *reinterpret_cast<int*>(p_PlayersCount);
	return injector::ReadMemory<int>(p_PlayersCount);
}

DWORD Player(int id)
{
	//return *(DWORD*)(*(DWORD*)(p_Player)+(4 + (8 * id)));
	return injector::ReadMemory<DWORD>((injector::ReadMemory<DWORD>(p_Player)) + (4 + (8 * id)));
}

template <typename T> T GetPlayerParam(CDPlayer param, int id)
{
	if (Player(id) != NULL)
	{
		//return *reinterpret_cast<T*>(Player(id) + (int)param);
		return injector::ReadMemory<T>(Player(id) + (int)param);
	}
}

template <typename T> void SetPlayerParam(CDPlayer param, T value, int id)
{
	if (Player(id) != NULL)
	{
		//*(T*)(Player(id) + (int)param) = value;
		injector::WriteMemory<T>(Player(id) + (int)param, value, false);
	}
}

DWORD Aftbur(DWORD player)
{
	if (Player(player) != NULL)
	{
		//return *(DWORD*)(Player(player) + (int)CDPlayer::AftburPtr);
		return injector::ReadMemory<DWORD>(Player(player) + (int)CDPlayer::AftburPtr);
	}
}

template <typename T> T GetAftburParam(CDAftbur param, int id)
{
	if (Aftbur(id) != NULL)
	{
		//return *reinterpret_cast<T*>(Aftbur(id) + (int)param);
		return injector::ReadMemory<T>(Aftbur(id) + (int)param);
	}
}

template <typename T> void SetAftburParam(CDAftbur param, T value, int id)
{
	if (Aftbur(id) != NULL)
	{
		//*(T*)(Aftbur(id) + (int)param) = value;
		injector::WriteMemory<T>(Aftbur(id) + (int)param, value, false);
	}
}

DWORD Minigun(DWORD player)
{
	if (Player(player) != NULL)
	{
		//return *(DWORD*)(Player(player) + (int)CDPlayer::MinigunPtr);
		return injector::ReadMemory<DWORD>(Player(player) + (int)CDPlayer::MinigunPtr);
	}
}

template <typename T> T GetMinigunParam(CDMinigun param, int id)
{
	if (Minigun(id) != NULL)
	{
		//return *reinterpret_cast<T*>(Minigun(id) + (int)param);
		return injector::ReadMemory<T>(Minigun(id) + (int)param);
	}
}

template <typename T> void SetMinigunParam(CDMinigun param, T value, int id)
{
	if (Minigun(id) != NULL)
	{
		//*(T*)(Minigun(id) + (int)param) = value;
		injector::WriteMemory<T>(Minigun(id) + (int)param, value, false);
	}
}

DWORD CarEngine(DWORD player)
{
	if (Player(player) != NULL)
	{
		return injector::ReadMemory<DWORD>(Player(player) + (int)CDPlayer::CarEnginePtr);
	}
}

template <typename T> T GetCarEngineParam(CDCarEngine param, int id)
{
	if (CarEngine(id) != NULL)
	{
		return injector::ReadMemory<T>(CarEngine(id) + (int)param);
	}
}

template <typename T> void SetCarEngineParam(CDCarEngine param, T value, int id)
{
	if (CarEngine(id) != NULL)
	{
		injector::WriteMemory<T>(CarEngine(id) + (int)param, value, false);
	}
}

DWORD CarSubmesh(DWORD player, int submesh_index, int offset = 0)
{
	if (Player(player) != NULL)
	{
		return injector::ReadMemory<DWORD>(injector::ReadMemory<DWORD>(Player(player) + 0x4) + 0xFC) + (0x314 * submesh_index) + offset;
	}
}

DWORD Wheel(DWORD player, int id)
{
	if (Player(player) != NULL)
	{
		return injector::ReadMemory<DWORD>(Player(player) + (int)CDPlayer::WheelPtr + (id * 0x168));
	}
}

template <typename T> T GetWheelParam(CDWheel param, int player_id, int wheel_id)
{
	if (Wheel(player_id, wheel_id) != NULL)
	{
		return injector::ReadMemory<T>(Wheel(player_id, wheel_id) + (int)param);
	}
}

template <typename T> void SetWheelParam(CDWheel param, T value, int player_id, int wheel_id)
{
	if (Wheel(player_id, wheel_id) != NULL)
	{
		injector::WriteMemory<T>(Wheel(player_id, wheel_id) + (int)param, value, false);
	}
}

DWORD GetRaceHandler()
{
	if (CDRace() != NULL)
	{
		DWORD ptr = injector::ReadMemory<DWORD>(CDRace() + 0x10);
		if (ptr != 0)
		{
			return injector::ReadMemory<DWORD>(ptr + 0x4);
		}
	}
	return 0; 
}

bool isGamePaused()
{
	if (CDRace() != NULL)
	{
		DWORD handler = GetRaceHandler();
		if (handler != 0)
		{
			return injector::ReadMemory<BYTE>(handler + 0x734);
		}
	}
	return false; // значение по умолчанию (игра не на паузе)
}

BYTE GetSpectatorID()
{
	if (CDRace() != NULL)
	{
		return injector::ReadMemory<BYTE>(GetRaceHandler() + 0x198);
	}
}

BYTE GetGameMode()
{
	if (CDRaceInfo() != NULL)
	{
		//return *(BYTE*)(CDRaceInfo());
		return injector::ReadMemory<BYTE>(CDRaceInfo());
	}
}

void SetGameMode(CDGameMode gamemode)
{
	if (CDRaceInfo() != NULL)
	{
		//*(BYTE*)(CDRaceInfo()) = (BYTE)gamemode;
		injector::WriteMemory(CDRaceInfo(), gamemode, false);
	}
}

BYTE GetRaceState()
{
	if (CDRaceInfo() != NULL)
	{
		//return *(BYTE*)(CDRaceInfo() + 0x80);
		return injector::ReadMemory<BYTE>(CDRaceInfo() + 0x80);
	}
}

BYTE GetEventType()
{
	if (CDRaceInfo() != NULL)
	{
		//return *(BYTE*)(CDRaceInfo() + 0x60);
		return injector::ReadMemory<BYTE>(CDRaceInfo() + 0x60);
	}
}

void SetEventType(CDEventType event_type)
{
	if (CDRaceInfo() != NULL)
	{
		//*(BYTE*)(CDRaceInfo() + 0x60) = (BYTE)event_type;
		injector::WriteMemory(CDRaceInfo() + 0x60, event_type, false);
	}
}

int GetEventTarget()
{
	if (CDRaceInfo() != NULL)
	{
		return injector::ReadMemory<int>(CDRaceInfo() + 0x64);
	}
}

void SetEventTarget(int target)
{
	if (CDRaceInfo() != NULL)
	{
		injector::WriteMemory(CDRaceInfo() + 0x64, target, false);
	}
}

DWORD GetRaceCameraHandler()
{
	if (CDRace() != NULL)
	{
		if (GetRaceState() == (BYTE)CDRaceState::InProcess)
		{
			return (injector::ReadMemory<DWORD>(injector::ReadMemory<DWORD>(CDRace() + 0x10) + 0xC));
		}
	}
}

BYTE GetRaceCameraID()
{
	if (CDRace() != NULL)
	{
		DWORD cameraHandler = GetRaceCameraHandler();
		if (cameraHandler != 0)
		{
			return injector::ReadMemory<BYTE>(cameraHandler + 0x8);
		}
	}
	return 0;
}

void SetRaceCameraID(CDRaceCamera cam)
{
	if (CDRace() != NULL)
	{
		//*(BYTE*)(*(DWORD*)(*(DWORD*)(CDRace() + 0x10) + 0xC) + 0x8) = (BYTE)cam;
		injector::WriteMemory(GetRaceCameraHandler() + 0x8, cam, false);
	}
}

BYTE GetRaceCameraAuto()
{
	if (CDRace() != NULL)
	{
		//return *(BYTE*)(*(DWORD*)(*(DWORD*)(CDRace() + 0x10) + 0xC) + 0x3C);
		return injector::ReadMemory<BYTE>(GetRaceCameraHandler() + 0x3C);
	}
}

void SetRaceCameraAuto(bool is_auto)
{
	if (CDRace() != NULL)
	{
		//*(BYTE*)(*(DWORD*)(*(DWORD*)(CDRace() + 0x10) + 0xC) + 0x3C) = (BYTE)is_auto;
		injector::WriteMemory(GetRaceCameraHandler() + 0x3C, is_auto, false);
	}
}

float GetFreeTrackCamParam(CDFreeTrackCamCtrl param)
{
	return injector::ReadMemory<float>(GetRaceCameraHandler() + (int)param, false);
}

void SetFreeTrackCamParam(CDFreeTrackCamCtrl param, float value)
{
	injector::WriteMemory(GetRaceCameraHandler() + (int)param, value, false);
}

DWORD GetAmbienceSystemPtr()
{
	if (CDRace() != NULL)
	{
		return injector::ReadMemory<DWORD>(CDRace() + 0x10);
	}
}

BYTE GetGarageColorType()
{
	if (*(DWORD*)0x7BC998)
	{
		return injector::ReadMemory<BYTE>(injector::ReadMemory<DWORD>(0x7BC998, true) + 0x14, false);
	}
}

std::string GetInterfacePage()
{
	if (CrashGUI())
	{
		return GetString(injector::ReadMemory<void*>(CrashGUI() + 0x4, true));
	}
}

int GetRaceTimer()
{
	if (CDRace())
	{
		return injector::ReadMemory<int>(CDRace() + 0xC, false);
	}
}

bool PressLMB()
{
	bool result;
	uintptr_t p_CDControl2 = *reinterpret_cast<uintptr_t*>(p_CDControl);
	if (p_CDControl2 != NULL)
	{
		result = *reinterpret_cast<BYTE*>(p_CDControl2 + 0x7C2);
	}
	//printf("L: %d\n", result);
	return result;
}

bool PressRMB()
{
	bool result;
	uintptr_t p_CDControl2 = *reinterpret_cast<uintptr_t*>(p_CDControl);
	if (p_CDControl2 != NULL)
	{
		result = *reinterpret_cast<BYTE*>(p_CDControl2 + 0x7C3);
	}
	//printf("R: %d\n", result);
	return result;
}

bool KeyHold(int key)
{
	int result;
	uintptr_t p_CDControl2 = *reinterpret_cast<uintptr_t*>(p_CDControl);
	if (p_CDControl2 != NULL)
	{
		result = *reinterpret_cast<BYTE*>(p_CDControl2 + key + 0x1A78);
	}

	if (result != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyPress(int key)
{
	static std::vector<unsigned char> prev_state;

	uintptr_t p_CDControl2 = *reinterpret_cast<uintptr_t*>(p_CDControl);
	if (p_CDControl2 == 0)
	{
		return false;
	}

	size_t idx = static_cast<size_t>(key + 0x1A78);
	if (idx >= prev_state.size()) prev_state.resize(idx + 1, 0);

	unsigned char cur = *reinterpret_cast<unsigned char*>(p_CDControl2 + idx);
	bool pressed = (cur != 0) && (prev_state[idx] == 0);

	prev_state[idx] = cur;

	//printf("Pointer: %p\n", (void*)p_CDControl);
	//printf("Current byte: %#02x, Previous byte: %#02x, Pressed(edge): %d\n", static_cast<unsigned int>(cur), static_cast<unsigned int>(prev_state[idx]), pressed);
	return pressed;
}

const char* LocStr(const char* section, const char* key)
{
	void* result;

	__asm
	{
		push ebx

		mov eax, ds: 0x7E3110
		mov edx, dword ptr[section]
		mov ebx, dword ptr[key]

		mov ecx, 0x66F410
		call ecx

		mov result, eax

		pop ebx
	}

	return static_cast<const char*>(result);
}

void KickPlayer(int id)
{
	__asm
	{
		mov eax, ds: [0x7CD0E0]
		mov edx, id
		mov ecx, 0x5C8900
		call ecx

		mov eax, id
		mov ecx, 0x55EF90
		call ecx
	}
}

void ShowHUDUpperMessage(const char* text, int color)
{
	if (GetRaceState() != (BYTE)CDRaceState::Finished)
	{
		injector::WriteMemory<float>(GetRaceHandler() + 0x72C, 4.f, false);
		WriteString<uint32_t>(GetRaceHandler() + 0x628, text, false);
		injector::WriteMemory<int>(GetRaceHandler() + 0x728, color, false);
	}
}

#define LOOP_PLAYERS for (int player = 1; player <= GetPlayersCount(); player++)