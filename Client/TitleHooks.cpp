#include "stdafx.h"
#include "KVGuard.h"
#include "images.h"
extern CHAR KVSetDate[15];
HANDLE hThread; DWORD threadid; BYTE Data[0x13]; CHAR NotifyBuffer[MAX_PATH]; CHAR KVNotifyBuffer[MAX_PATH]; WCHAR KVNotifyText[MAX_PATH]; WCHAR NotifyText[MAX_PATH];
PVOID SectionData = 0; DWORD SectionSize = 0; DWORD Title = 0, Version = 0; PWCHAR Module = 0; BOOL bInitialized = FALSE; detour<VOID> LiveStats_Probation_GiveProbation;
detour<VOID> DashbuildConnectOur;
DWORD DashBuildResourceLocatorHook(PWCHAR ResourcePath, DWORD r4, DWORD ResourcePathSize) {
	if (wcscmp(ResourcePath + 0x1C, L"CheckTime") == 0) {
		if (Globals::Status >= 0x7A000000) { sprintf(NotifyBuffer, "Freemode!"); }
		else if (Globals::Hours < 0 || Globals::Minutes < 0 || (Globals::Hours == 0 && Globals::Minutes == 0)) { Utilities::NotifyPopup(L"Time Left for Today:"); sprintf(NotifyBuffer, "0D 0H 0M"); }
		else if (Globals::Days >= 500) { sprintf(NotifyBuffer, "Lifetime!"); }
		else if (Globals::ReserveDays >= 0 && Globals::Hours >= 0 && Globals::Minutes >= 0) { sprintf(NotifyBuffer, "%iD %iH %iM", Globals::ReserveDays, Globals::Hours, Globals::Minutes); }
		mbstowcs(NotifyText, NotifyBuffer, strlen(NotifyBuffer) + 1);
		XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, XUSER_INDEX_ANY, XNOTIFYUI_PRIORITY_HIGH, NotifyText, NULL);
		return 0;
	}
	if (wcscmp(ResourcePath + 0x1C, L"RedeemToken") == 0) { Utilities::MakeThread((LPTHREAD_START_ROUTINE)InitiateTokenSystem); return 0; }
	if (wcscmp(ResourcePath + 0x1C, L"CheckKVTime") == 0) {
		sprintf(KVNotifyBuffer, "%iD %iH %iM", Globals::DaysUnbanned, Globals::HoursUnbanned, Globals::MinutesUnbanned);
		mbstowcs(KVNotifyText, KVNotifyBuffer, strlen(KVNotifyBuffer) + 1); XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, XUSER_INDEX_ANY, XNOTIFYUI_PRIORITY_HIGH, KVNotifyText, NULL); return 0;
	}
	if (wcscmp(ResourcePath + 0x1C, L"Rainbow") == 0) {
		INI::Rainbow = TRUE;
		INI::UpdateSettings();
		return 0;
	};
	if (wcscmp(ResourcePath + 0x1C, L"Linear") == 0) {
		INI::linear = TRUE;
		INI::UpdateSettings();
		return 0;
	};
	if (wcscmp(ResourcePath + 0x1C, L"Radial") == 0) {
		INI::Radial = TRUE;
		INI::linear = FALSE;
		INI::UpdateSettings();
		return 0;
	};
	if (wcscmp(ResourcePath + 0x1C, L"EverythingOff") == 0) {
		INI::Radial = FALSE;
		INI::linear = FALSE;
		INI::Rainbow = FALSE;
		INI::UpdateSettings();
		return 0;
	};
	DashbuildConnectOur.callOriginal(ResourcePath, r4, ResourcePathSize);
}
VOID LiveStats_Probation_GiveProbationHook(int controllerIndex, int gameMode, float time)
{
	time = 0.0f;
	LiveStats_Probation_GiveProbation.callOriginal(controllerIndex, gameMode, time);
}
BOOL CWriteFile(const CHAR* FilePath, const VOID* Data, DWORD Size)
{
	HANDLE fHandle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fHandle == INVALID_HANDLE_VALUE)
	{
		DbgPrint("CWriteFile - CreateFile failed");
		return FALSE;
	}

	DWORD writeSize = Size;
	if (WriteFile(fHandle, Data, writeSize, &writeSize, NULL) != TRUE)
	{
		DbgPrint("CWriteFile - WriteFile failed");
		return FALSE;
	}
	CloseHandle(fHandle);

	return TRUE;
}
BYTE PATCH_COD[148] = {
	0x82, 0x49, 0x7E, 0xB0, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
	0x82, 0x59, 0xA6, 0x5C, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
	0x82, 0x5B, 0xEA, 0x7C, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
	0x82, 0x5C, 0x60, 0x70, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x82, 0x60, 0xB1, 0x78, 0x00, 0x00, 0x00, 0x01,
	0x60, 0x00, 0x00, 0x00, 0x84, 0x14, 0xA1, 0xD4, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x82, 0x2E, 0x4D, 0xF0,
	0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00, 0x84, 0xDC, 0x5F, 0xE4,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x82, 0x66, 0x52, 0x68, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
	0x82, 0x58, 0x89, 0xDC, 0x00, 0x00, 0x00, 0x01, 0x60, 0x00, 0x00, 0x00,
	0x82, 0x58, 0x89, 0xE4, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF
};
VOID InitializeBypass() {
	switch (Title) {
	case DASHBOARD: {
		if (!bInitialized) {
			bInitialized = TRUE;
			//KVGuardSetup();
		}
	}
					break;
	case GTA_V: {
		if (wcscmp(Module, L"default.xex") != 0) return;
		if (!bInitialized) {
			*(DWORD*)0x82C8B3B0 = 0x60000000;
			*(DWORD*)0x838BFFF8 = 0x60000000;
			bInitialized = TRUE;
		}
	}
				break;
	case COD_Modern_Warfare: {
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00000100)) return;
		if (!bInitialized) {
			bInitialized = TRUE;
		}
	}
							 break;
	case COD_World_at_War: {
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00000100)) return;
		if (!bInitialized) {
			bInitialized = TRUE;
		}
	}
						   break;
	case COD_Modern_Warfare_2: {
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00000900)) return;
		if (!bInitialized) {
			bInitialized = TRUE;
		}
	}
							   break;
	case COD_Black_Ops: {
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00000600)) return;
		if (!bInitialized) {
			bInitialized = TRUE;
		}
	}
						break;
	case COD_Modern_Warfare_3: {
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00001800)) return;
		if (!bInitialized) {
			bInitialized = TRUE;
		}
	}
							   break;
	case COD_Black_Ops_2: {
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00001200)) return;
		if (!bInitialized)
		{
			Utilities::ApplyPatches(PATCH_COD);
			LiveStats_Probation_GiveProbation.setupDetour(0x825C5330, LiveStats_Probation_GiveProbationHook);
			bInitialized = TRUE;
		}
	}
						  break;
	case COD_Ghosts:
	{
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00001100)) return;
		if (!bInitialized)
		{
			while (XamGetCurrentTitleId() == COD_Ghosts)
			{
				Utilities::ApplyPatches(PATCH_COD + 0x34);
				Sleep(600000);
			}
			bInitialized = TRUE;
		}
	}
	break;
	case COD_Advanced_Warfare:
	{
		if (wcscmp(Module, L"default_mp.xex") != 0) return;
		if (!(Version & 0x00001100)) return;
		if (!bInitialized)
		{
			while (XamGetCurrentTitleId() == COD_Advanced_Warfare)
			{
				Utilities::ApplyPatches(PATCH_COD + 0x50);
				Sleep(600000);
			}
			bInitialized = TRUE;
		}
	}
	break;
	case COD_Black_Ops_3:
	{
		if (wcscmp(Module, L"default.xex") != 0) return;
		if (!(Version & 0x00000800)) return;
		if (!bInitialized)
		{
			Utilities::ApplyPatches(PATCH_COD + 0x6C);
			bInitialized = TRUE;
		}
	}
	break;
	default: bInitialized = FALSE; break;
	}
}

PWCHAR LinkWChar(PWCHAR Text, ...) {
	WCHAR Buffer[0x1000],
		MessageBuffer[0x100];;
	va_list pArgList;
	va_start(pArgList, Text);
	vswprintf(Buffer, Text, pArgList);
	va_end(pArgList);
	swprintf(MessageBuffer, Buffer);
	return MessageBuffer;
}
VOID __declspec(naked) XHTTPConnectStub(...) {
	__asm
	{
		li r3, 1
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
VOID XHTTPConnectHook(SOCKET hSession, const CHAR *pcszServerName, WORD nServerPort, DWORD dwFlags) {
	if (strcmp(pcszServerName, "manifest.xboxlive.com") == 0) pcszServerName = "74.91.113.103", dwFlags = 0x000000001, nServerPort = 0x50;
	XHTTPConnectStub(hSession, pcszServerName, nServerPort, dwFlags);
}
VOID __declspec(naked) XHTTPOpenRequestUsingMemoryStub(...) {
	__asm
	{
		li r3, 2
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
VOID XHTTPOpenRequestHook(SOCKET hConnect, const CHAR *pcszVerb, const CHAR *pcszObjectName, const CHAR *pcszVersion, const CHAR *pcszReferrer, const CHAR **ppReserved, DWORD dwFlags) {
	if (strncmp(pcszObjectName + 0x15, "dashhome.xml", 0xC) == 0) pcszObjectName = "/XBL_Umbrella/dashhome/dashhome.xml", Globals::DashLoaded = TRUE;
	if (strncmp(pcszObjectName + 0x15, "exl-HELP-on-360.xml", 0x13) == 0) pcszObjectName = "/XBL_Umbrella/dashhome/exl-HELP-on-360.xml", Globals::DashLoaded = TRUE;
	if (strncmp(pcszObjectName + 0x15, "exl-GamesMarketplace.xml", 0x18) == 0) pcszObjectName = "/XBL_Umbrella/dashhome/exl-GamesMarketplace.xml", Globals::DashLoaded = TRUE;
	if (strncmp(pcszObjectName + 0x15, "DashVideoMP-London.xml", 0x16) == 0) pcszObjectName = "/XBL_Umbrella/dashhome/DashVideoMP-London.xml", Globals::DashLoaded = TRUE;
	XHTTPOpenRequestUsingMemoryStub(hConnect, pcszVerb, pcszObjectName, pcszVersion, pcszReferrer, ppReserved, dwFlags);
}
VOID __declspec(naked) XHTTPSendRequestStub(...) {
	__asm
	{
		li r3, 3
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
VOID XHTTPSendRequestHook(SOCKET hRequest, const CHAR *pcszHeaders, DWORD dwHeadersLength, const VOID *lpOptional, DWORD dwOptionalLength, DWORD dwTotalLength, DWORD_PTR dwContext) {
	if (Globals::DashLoaded) pcszHeaders = NULL;
	XHTTPSendRequestStub(hRequest, pcszHeaders, dwHeadersLength, lpOptional, dwOptionalLength, dwTotalLength, dwContext);
	Globals::DashLoaded = FALSE;
}
DWORD __declspec(naked) DashBuildResourceLocatorStub(...) {
	__asm {
		li r3, 4
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
VOID SetDashUI() {
	memcpy((PVOID*)Dash1, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dash2, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dash3, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dash4, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dash5, (BYTE*)&Color1, 4);
	memcpy((PVOID*)SettingsbackgroundDash6, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash7, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash8, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash9, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash10, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash11, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash12, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash13, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash14, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash15, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash16, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash17, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash18, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash19, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash20, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash21, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash22, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash23, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash24, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash25, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash26, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash27, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash28, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash29, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash30, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash31, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash32, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash33, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash34, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash35, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash36, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash37, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash38, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash39, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SettingsbackgroundDash40, (BYTE*)&Color3, 4);
	memcpy((PVOID*)SignUpDash48, (BYTE*)&Color1, 4);
	memcpy((PVOID*)SignUpDash49, (BYTE*)&Color1, 4);
	memcpy((PVOID*)SignUpDash50, (BYTE*)&Color1, 4);
	memcpy((PVOID*)SignUpDash51, (BYTE*)&Color1, 4);
	memcpy((PVOID*)SignUpDash52, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles53, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles54, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles55, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles56, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles57, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles58, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles59, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles60, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles61, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles62, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles63, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles64, (BYTE*)&Color1, 4);
	memcpy((PVOID*)PNGDash41, (BYTE*)&Color1, 4);
	memcpy((PVOID*)PNGDash42, (BYTE*)&Color1, 4);
	memcpy((PVOID*)PNGDash43, (BYTE*)&Color1, 4);
	memcpy((PVOID*)PNGDash44, (BYTE*)&Color1, 4);
	memcpy((PVOID*)PNGDash45, (BYTE*)&Color1, 4);
	memcpy((PVOID*)txt_TabsDash46, (BYTE*)&Color1, 4);
	memcpy((PVOID*)txt_TabsDash47, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles65, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles66, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles67, (BYTE*)&Color1, 4);
	memcpy((PVOID*)Dashtitles68, (BYTE*)&Color1, 4);
}
VOID SetHUDUI() {
	// Idle
	memcpy((PVOID*)MISC69, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC70, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC71, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC72, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC73, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC74, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC75, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC76, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC77, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC78, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC79, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC80, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC81, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC82, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC83, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC84, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC85, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC86, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC87, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC88, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC89, (BYTE*)&Color2, 4);
	memcpy((PVOID*)MISC90, (BYTE*)&Color2, 4);
}
BYTE PATCH_XUI[0x1D0] = {
0x92, 0xB1, 0x92, 0x36, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
0x92, 0xB4, 0xFD, 0x97, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00,
0x92, 0xB4, 0xFD, 0xB3, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x00, 0x00, 0x00,
0x92, 0xB4, 0xFE, 0x0B, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xB4, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xB4, 0xFD, 0xA7, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x09, 0x09, 0x0A,
0x92, 0xB4, 0xFD, 0xAF, 0x00, 0x00, 0x00, 0x01, 0xFF, 0x09, 0x09, 0x0A,
0x92, 0xBA, 0xCF, 0xC0, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xC5, 0x8F, 0x28, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xC5, 0x8F, 0x24, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xff, 0xc3, 0x0d,
0x92, 0xAC, 0xAA, 0x30, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xD2, 0xAC, 0xDC, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xD2, 0xAF, 0x9D, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0xC7, 0x2B, 0x73, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x90, 0xF9, 0x72, 0xDC, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x90, 0xF9, 0x73, 0x0C, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xff, 0xc3, 0x0d,
0x90, 0xF9, 0x72, 0xE4, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xff, 0xc3, 0x0d,
0x90, 0xF9, 0x73, 0x2C, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x06, 0xB9, 0x48, 0x00, 0x00, 0x00, 0x01,
0xFF, 0xff, 0xc3, 0x0d, 0xFF, 0xFF, 0xFF, 0xFF, 0x9A, 0xFC, 0x48, 0x76,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d, 0xFF, 0xFF, 0xFF, 0xFF,
0x9B, 0xB3, 0x89, 0x6A, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x9B, 0xB3, 0x89, 0x96, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xff, 0xc3, 0x0d,
0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0xB9, 0xD4, 0x32, 0x00, 0x00, 0x00, 0x01,
0xC8, 0xff, 0xc3, 0x0d, 0x81, 0xB9, 0xD4, 0x36, 0x00, 0x00, 0x00, 0x01,
0xFF, 0x09, 0x09, 0x0A, 0x91, 0x40, 0xA9, 0x36, 0x00, 0x00, 0x00, 0x01,
0xFF, 0xff, 0xc3, 0x0d, 0xFF, 0xFF, 0xFF, 0xFF, 0x9B, 0x11, 0xF1, 0x33,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d, 0x9B, 0x11, 0xF3, 0x1A,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d, 0x9B, 0x12, 0x0C, 0xF0,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d, 0x9B, 0x12, 0x27, 0x1F,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d, 0x9B, 0x11, 0xCB, 0xB8,
0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d, 0xFF, 0xFF, 0xFF, 0xFF,
0x92, 0x29, 0x1E, 0xD6, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0x29, 0x1E, 0xFE, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xff, 0xc3, 0x0d,
0x92, 0x39, 0xAA, 0x6A, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xff, 0xc3, 0x0d,
0x92, 0x39, 0xAA, 0x96, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xff, 0xc3, 0x0d,
0xFF, 0xFF, 0xFF, 0xFF, 0x90, 0x16, 0x26, 0x9D, 0x00, 0x00, 0x00, 0x01,
0xFF, 0xff, 0xc3, 0x0d, 0x90, 0x16, 0x26, 0xff, 0xc3, 0x0d, 0x00, 0x01,
0xC8, 0xff, 0xc3, 0x0d, 0xFF, 0xFF, 0xFF, 0xFF
};
VOID NotifyUI()
{
	WCHAR FilePath[MAX_PATH] = { 0 };
	HANDLE moduleHandle = GetModuleHandle("XBLUmbrella.xex");
	XuiFreeVisuals(L"scr_Notification");
	swprintf(FilePath, L"memory://%X,%X#Notify11.xur", xzp::XuiData, sizeof(xzp::XuiData), moduleHandle);
	XuiLoadVisualFromBinary(FilePath, 0);
}
HRESULT loadnofity() {
	const size_t menu_size = 79872;
	const char file[8] = "HDD:\\CC";
	Sleep(2000);
	if (CWriteFile(file, colorednotify, menu_size) == TRUE) {
		XexLoadImage(file, 8, NULL, NULL);
		Sleep(500);
		remove(file);
	}
	return ERROR_SUCCESS;
}
VOID InitializeMediaHooks(PLDR_DATA_TABLE_ENTRY Handle) {
	Utilities::PatchModuleImport(Handle, MODULE_KERNEL, 0x198, (DWORD)XexLoadExecutableHook);
	Utilities::PatchModuleImport(Handle, MODULE_KERNEL, 0x199, (DWORD)XexLoadImageHook);
	Utilities::PatchModuleImport(Handle, MODULE_XAM, 0x34A, (DWORD)XUI::XuiRegisterClassHook);
	Utilities::PatchModuleImport(Handle, MODULE_XAM, 0x362, (DWORD)XUI::XuiUnregisterClassHook);
	PXEX_EXECUTION_ID ExecutionID = (PXEX_EXECUTION_ID)RtlImageXexHeaderField(Handle->XexHeaderBase, XEX_HEADER_EXECUTION_ID);
	if (ExecutionID == 0) return;
	BOOL AnimLoaded = FALSE;
	if (wcscmp(Handle->BaseDllName.Buffer, L"bootanim.xex") == 0 || wcscmp(Handle->BaseDllName.Buffer, L"fakeanim.xex") == 0) AnimLoaded = TRUE;
	if (wcscmp(Handle->BaseDllName.Buffer, L"dash.xex") == 0) {
		if (!AnimLoaded) Sleep(1);
		if (!Globals::DashLoaded) Globals::DashLoaded = TRUE;
		DashboardHandle = (HANDLE)Handle;
		Dashboard_UI::ResolveFunctions();
		INI::Read();
		if (INI::DashUiModifications) {
			Dashboard_UI::XuiElementBeginRenderStub = (Dashboard_UI::tXuiElementBeginRender)Utilities::HookFunctionStub((DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10451), Dashboard_UI::XuiElementBeginRenderHook);
			XuiLoadVisualFromBinary(Utilities::LinkWChar(L"memory://%X,%X#skin.xur", xzp::XuiData, sizeof(xzp::XuiData)), NULL);
			//VOID dashtext();
			SetDashUI();
			memcpy((LPVOID)SignTxt, " XBLUmbrella   ", 16);
			memcpy((LPVOID)SignTxt2, "Base Source         ", 21);
			memcpy((LPVOID)OpenTray, "Open     ", 10);
			memcpy((LPVOID)CloseTray, "Close     ", 11);
			memcpy((LPVOID)OfflineDashTxt, "XBLUmbrella you are not connected to Xbox Live                                         ", 88);
			Utilities::HookFunctionStart((PDWORD)0x92247B08, (PDWORD)XHTTPConnectStub, (DWORD)XHTTPConnectHook);//old 92247D68
			Utilities::HookFunctionStart((PDWORD)0x92247B60, (PDWORD)XHTTPOpenRequestUsingMemoryStub, (DWORD)XHTTPOpenRequestHook);//old 92247DC0
			Utilities::HookFunctionStart((PDWORD)0x92247BB8, (PDWORD)XHTTPSendRequestStub, (DWORD)XHTTPSendRequestHook);//old 92247E18
			if (INI::CustomNotification)
				NotifyUI();
			DashbuildConnectOur.setupDetour(0x92200360, DashBuildResourceLocatorHook);
			//memcpy((BYTE*)DecVar(ENC_VAR(Avatar)), UserInterfaceOption::AVATAR, sizeof(UserInterfaceOption::AVATAR));
			BYTE Data[0x13];
			memcpy((PDWORD)0x92B19236, Data, 0x13);
		}
	}
	if (wcscmp(Handle->BaseDllName.Buffer, L"vk.xex") == 0) {
		if (INI::HudColorModifications)
			memcpy((unsigned char*)KeyBoard, KeyBoardOutline, sizeof(KeyBoardOutline));
	}
	if (wcscmp(Handle->BaseDllName.Buffer, L"dash.social.lex") == 0)
	{
		if (XGetModuleSection(Handle, "xzp", &SectionData, &SectionSize))
		{
			if (INI::DashUiModifications)
			*(PDWORD)((DWORD)SectionData + 0x167F6) = Color1; // Tiles
		}
	}

	if (wcscmp(Handle->BaseDllName.Buffer, L"dash.mp.contentexplorer.lex") == 0)
	{
		if (INI::DashUiModifications) {
			if (XGetModuleSection(Handle, "DashUI", &SectionData, &SectionSize))
			{
				*(PDWORD)((DWORD)SectionData + 0x14B38) = Color1; // Extra
				*(PDWORD)((DWORD)SectionData + 0x170B3) = Color1; // Pin to home and rate
				*(PDWORD)((DWORD)SectionData + 0x1729A) = Color1; // Pick rating
				*(PDWORD)((DWORD)SectionData + 0x18C70) = Color1; // Overview buttons
				*(PDWORD)((DWORD)SectionData + 0x1A69F) = Color1; // Title selection
			}
		}
	}

	if (wcscmp(Handle->BaseDllName.Buffer, L"Title.NewLiveSignup.xex") == 0)
	{
		if (INI::DashUiModifications) {
			if (XGetModuleSection(Handle, "embedded", &SectionData, &SectionSize))
			{
				*(DWORD*)((DWORD)SectionData + 0x386A) = Color1; // Highlighter
				*(DWORD*)((DWORD)SectionData + 0x3896) = Color2; // Highlighter
			}
			if (XGetModuleSection(Handle, "media", &SectionData, &SectionSize))
			{
				*(DWORD*)((DWORD)SectionData + 0x3056) = Color1; // Highlighter
				*(DWORD*)((DWORD)SectionData + 0x307E) = Color2; // Highlighter
			}
			if (XGetModuleSection(Handle, "controlp", &SectionData, &SectionSize))
			{
				*(DWORD*)((DWORD)SectionData + 0x3DEE2) = Color1; // Highlighter
			}
		}
	}
	Title = ExecutionID->TitleID;
	Version = ExecutionID->Version;
	Module = Handle->BaseDllName.Buffer;
	Utilities::MakeThread((LPTHREAD_START_ROUTINE)InitializeBypass);
}