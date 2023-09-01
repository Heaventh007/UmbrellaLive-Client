#include "stdafx.h"
namespace Globals {
	extern PCHAR pathKeyVault;
	extern PCHAR pathCPUKey;
	extern PCHAR pathXeX;
	extern PCHAR customxex;
	extern PCHAR PATH_NewXEX;
	//UI
	extern PCHAR PATH_rainbow;
	extern PCHAR PATH_linear;
	extern PCHAR PATH_radial;
	extern PCHAR PATH_EverythingOff;
	extern XNOTIFYQUEUEUI_TYPE NotifyType;
	extern BYTE CurrentMACAddress[0x6];
	extern BYTE SpoofedMACAddress[0x6];
	extern BOOL DashLoaded;
	extern BYTE CPUKey[0x10];
	extern BYTE KVCPUKey[0x10];
	extern BYTE CPUKeyDigest[0x14];
	extern BYTE KVDigest[0x14];
	extern KEY_VAULT KV;
	extern BOOL Crl, Fcrt, kvtype;
	extern DWORD UpdateSequence;
	extern BYTE Signature[0x80];
	extern BYTE TitleDigest[0x10];
	extern BYTE ECCDigest[0x14];
	extern BYTE PATCH_XAM[0x58];
	extern SOCKET Socket;
	extern BYTE TempBuffer[0x4080];
	extern BOOL Connected;
	extern BOOL InitialConnected;
	extern BOOL Initialized;
	extern BOOL Authenticated;
	extern BOOL ExpiredNotify, DayExpiredNotify, NewDayStartedNotify;
	extern u32 Days, Hours, Minutes, DaysUnbanned, HoursUnbanned, MinutesUnbanned;
	extern INT ReserveDays;
	extern BYTE IPAddress[0x4];
	extern WORD Port;
	extern DWORD Status;
	extern BOOL RunningFromUSB;
	extern INT ConnectionAttempt;
}