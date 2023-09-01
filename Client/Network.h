#include "stdafx.h"
enum TokenType_e : int
{
	ONE_DAY = 0,
	TOKEN_7_DAYS = 1,
	TOKEN_31_DAYS = 2,
	TOKEN_Lifetime = 3,
	TOKEN_INVALID = 4,
};
struct REDEEM_REQUEST
{
	BYTE SessionKey[16];
	char Token[14];
	BYTE CPUKey[0x10];
};
struct REDEEM_RESPONSE
{
	BYTE SessionKey[16];
	DWORD Status;
	TokenType_e Days;
};
struct CHECK_REQUEST
{
	BYTE CPUKey[0x10];
	char Token[14];
	TokenType_e Days;
	BYTE SessionKey[16];
};
struct CHECK_RESPONSE
{
	TokenType_e Days;
};
struct INIT_REQUEST {
	DWORD Version;
	BYTE CPUKey[0x10], ModuleDigest[0x14];
	KEY_VAULT KV;
};
struct INIT_RESPONSE {
	DWORD Status;
};
struct PRES_REQUEST {
	DWORD TitleID;
	BYTE CPUKey[0x10], ModuleDigest[0x14];
	CHAR Gamertag[0x10];

};
struct PRES_RESPONSE {
	DWORD Status;
	DWORD TimeRemaining[3];
	DWORD TimeUnbanned[3];
	DWORD ReserveDays;
};
struct XKEC_REQUEST {
	BYTE CPUKey[0x10], HVSalt[0x10];
	BOOL Crl;
	BOOL Fcrt;
	BOOL KVType;
	BYTE SerialByte;
};
struct XKEC_RESPONSE {
	DWORD Status;
};
struct XOSC_REQUEST {
	BYTE xoscbuffer[0x2E0];
	DWORD Title;
	BYTE CPUKey[0x10];
	BYTE KVDigest[0x14];
	BYTE Final1[0x10];
	BYTE Final2[0x8];
};

struct XOSC_RESPONSE {
	DWORD Status;
};
#pragma pack(pop)

#define COMMAND_INIT 0x00000001
#define COMMAND_PRES 0x00000002
#define COMMAND_XKEC 0x00000003
#define COMMAND_XOSC 0x00000004
#define COMMAND_CHECK_TOKEN 0x00000005
#define COMMAND_RED_TOKEN 0x00000006
BOOL SendServerCommands(DWORD Command, PVOID Buffer, DWORD Size);
BOOL ReceiveData(PVOID Buffer, DWORD Size);
BOOL ProcessCommand(DWORD Command, PVOID Request, DWORD RequestSize, PVOID Response, DWORD ResponseSize, BOOL Close);
BOOL Connect();
VOID FreeSession();
//VOID HandleUpdate(BOOL Reboot);
VOID InitializeConnection();
VOID ServerPresence();
void InitiateTokenSystem();
VOID ProtectMyMemorySpace();
void AddRegionToWhitelist(DWORD start, DWORD length);
extern BOOL GetKVSetDate(string File);