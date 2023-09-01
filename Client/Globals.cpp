#include "stdafx.h"
#include "wchar.h"
namespace Globals {
	PCHAR pathKeyVault = "";
	PCHAR pathCPUKey = "";
	PCHAR pathXeX = "";
	PCHAR customxex = "";
	PCHAR PATH_NewXEX;
	//UI
	PCHAR PATH_rainbow;
	PCHAR PATH_linear;
	PCHAR PATH_radial;
	PCHAR PATH_EverythingOff;
	XNOTIFYQUEUEUI_TYPE NotifyType = XNOTIFYUI_TYPE_CONSOLEMESSAGE;
	BYTE CurrentMACAddress[0x6] = { 0 };
	BYTE SpoofedMACAddress[0x6] = { 0 };
	BOOL DashLoaded = FALSE;
	BYTE CPUKey[0x10] = { 0 };
	BYTE KVCPUKey[0x10] = { 0 };
	BYTE CPUKeyDigest[0x14] = { 0 };
	BYTE KVDigest[0x14] = { 0 };
	KEY_VAULT KV = { 0 };
	BOOL Crl = FALSE, Fcrt = FALSE, kvtype = TRUE;
	DWORD UpdateSequence = 0x00000008;
	BYTE Signature[0x80], TitleDigest[0x10] = { 0 };
	BYTE ECCDigest[0x14] = { 0 };
	BYTE PATCH_XAM[0x58] = { 0 };
	SOCKET Socket = INVALID_SOCKET;
	BYTE TempBuffer[0x4080] = { 0 };
	BOOL Connected = FALSE;
	BOOL InitialConnected = FALSE;
	BOOL Initialized = FALSE;
	BOOL Authenticated = FALSE;
	u32 Days = 0, Hours = 0, Minutes = 0, DaysUnbanned = 0, HoursUnbanned = 0, MinutesUnbanned = 0;
	INT ReserveDays = 0;
	BYTE IPAddress[0x4] = { 74, 91, 113, 103 }; //74.91.113.103
	WORD Port = 4444;
	DWORD Status;
	INT ConnectionAttempt = 0;
	DWORD Addresses[8] = { 0 };
	BOOL RunningFromUSB = FALSE;
	BOOL ExpiredNotify = FALSE, DayExpiredNotify = FALSE, NewDayStartedNotify = FALSE;
}