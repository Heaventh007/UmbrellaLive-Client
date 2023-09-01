#include "stdafx.h"
XEX_EXECUTION_ID SpoofedExecutionID = { 0,
	((XboxKrnlVersion->Major & 0xF) << 28) | ((XboxKrnlVersion->Minor & 0xF) << 24) | (XboxKrnlVersion->Build << 8) | (XboxKrnlVersion->Qfe),
	((XboxKrnlVersion->Major & 0xF) << 28) | ((XboxKrnlVersion->Minor & 0xF) << 24) | (XboxKrnlVersion->Build << 8) | (XboxKrnlVersion->Qfe),
	0xFFFE07D1, 0, 0, 0, 0, 0
};
PVOID RtlImageXexHeaderFieldHook(PVOID XexHeaderBase, DWORD ImageField)
{
	PVOID Result = RtlImageXexHeaderField(XexHeaderBase, ImageField);
	if (ImageField == XEX_HEADER_EXECUTION_ID)
	{
		if (Result)
		{
			switch (((PXEX_EXECUTION_ID)Result)->TitleID)
			{
			case 0xC0DE9999: // XeXMenu
			case 0xFFFF0055: // XeXMenu [1]
			case 0xFFFF011D: // Dashlaunch
			case 0xFFFE07FF: // XShellXDK
			case 0xF5D20000: // Freestyle Dash
			case 0x00000166: // Aurora
			case 0x00000189: // Simple 360 NandFlasher
			case 0x00000188: // Flash 360
			case 0x00000176: // XM360
			case 0x00000167: // Freestyle Dash 3
			case 0x00000177: // NXE2GOD
			case 0x00000170: // XeXMenu 2.0
			case 0xFFFEFF43: // XellLaunch [GOD]
			case 0xFEEDC0DE: // XYZ Project
			case 0x58480880: // IE Homebrew
			case 0x00000001: // FX Menu
			case 0x00000171: // FCEUX
			case 0xFFED0707: // SNES 360
			case 0x1CED2911: // PCSXR
			case 0xFFED7300: // FCE 360
			case 0x00FBAFBA: // DSON 360
			case 0x000003D0: // 3DOX
				memcpy(Result, &SpoofedExecutionID, sizeof(XEX_EXECUTION_ID));
				break;
			}
		}
		else Result = &SpoofedExecutionID;
	}
	return Result;
}
//PVOID RtlImageXexHeaderFieldHook(PVOID XexHeaderBase, DWORD ImageField) {
//	if (ImageField != XEX_HEADER_EXECUTION_ID) { return RtlImageXexHeaderField(XexHeaderBase, ImageField); }
//	DWORD HomebrewTitleIds[] = { 0xC0DE9999, 0xFFFF0055, 0xFFFF011D, 0xFFFE07FF };
//	PXEX_EXECUTION_ID ExecutionID2 = (PXEX_EXECUTION_ID)RtlImageXexHeaderField(XexHeaderBase, ImageField);
//	for (DWORD i = 0x00; i < ARRAYSIZE(HomebrewTitleIds); i++) {
//		if (ExecutionID2->TitleID == HomebrewTitleIds[i]) {
//			RtlZeroMemory(ExecutionID2, sizeof(XEX_EXECUTION_ID));
//			ExecutionID2->BaseVersion = XboxKrnlVersion->Major << 0x1C | XboxKrnlVersion->Build << 0x08 | XboxKrnlVersion->Qfe;
//			ExecutionID2->Version = XboxKrnlVersion->Major << 0x1C | XboxKrnlVersion->Build << 0x08 | XboxKrnlVersion->Qfe;
//			ExecutionID2->TitleID = 0xFFFE07D1;
//		}
//	}
//	return ExecutionID2;
//}
NTSTATUS XexLoadExecutableHook(PCHAR Name, PHANDLE Handle, DWORD TypeFlags, DWORD Version) {
	HANDLE Module = 0;
	NTSTATUS Result = XexLoadExecutable(Name, &Module, TypeFlags, Version);
	if (Handle != 0) *Handle = Module;
	if (NT_SUCCESS(Result)) InitializeMediaHooks((PLDR_DATA_TABLE_ENTRY)*XexExecutableModuleHandle);
	return Result;
}
NTSTATUS XexLoadImageHook(CONST PCHAR Name, DWORD TypeFlags, DWORD Version, PHANDLE Handle)
{
	HANDLE Module = 0;
	NTSTATUS Result = XexLoadImage(Name, TypeFlags, Version, &Module);
	if (Handle != 0) *Handle = Module;
	if (NT_SUCCESS(Result)) InitializeMediaHooks((PLDR_DATA_TABLE_ENTRY)Module);
	return Result;
}
BOOL BufferIsNull(PBYTE Buffer, int Size) {
	BOOL BufferIsNull = TRUE;
	for (int i = 0; i < Size; i++) {
		if (Buffer[i] != 0) {
			BufferIsNull = FALSE;
		}
	}
	return BufferIsNull;
}
DWORD XeKeysExecuteHook(PBYTE Buffer, DWORD Size, PBYTE HVSalt, PVOID r6, PVOID r7, PVOID r8) {
	XKEC_REQUEST Request = { 0 }; XKEC_RESPONSE Response = { 0 }; BYTE kv[2];
	QWORD kvAddress = Utilities::HvxPeekQWORD(0x80000102000163C0);
	Utilities::HvxPeekBytes(kvAddress + 0x9D1, kv, 0x2);
	memcpy(Request.CPUKey, Utilities::GetCPUKey(), 0x10);
	memcpy(Request.HVSalt, HVSalt, 0x10);
	Request.Crl = Globals::Crl;
	Request.Fcrt = Globals::Fcrt;
	Request.KVType = Globals::kvtype;
	Request.SerialByte = ((kv[0] << 4) & 0xF0) | (kv[1] & 0x0F);
	if (ProcessCommand(COMMAND_XKEC, &Request, sizeof(XKEC_REQUEST), &Response, sizeof(XKEC_RESPONSE), FALSE)) {
		if (Globals::Authenticated = ((Response.Status == 0x5A000000) || (Response.Status == 0x10000000))) {
			memset(Buffer, 0, Size);
			if (!ReceiveData(Buffer, 0x100)) { Utilities::NotifyPopup(L"NOOOO XKE  - Failed to Request XKE Response Rebooting!"); Sleep(2000); HalReturnToFirmware(HalRebootQuiesceRoutine); }
			if (BufferIsNull(Buffer, 0x100)) { Utilities::NotifyPopup(L"FUCK - Bad XKE Response, Rebooting!"); Sleep(2000); HalReturnToFirmware(HalRebootQuiesceRoutine); }
		}
		else {
			Sleep(3000);
			HalReturnToFirmware(HalRebootQuiesceRoutine);
		}
	}
	else {
		Sleep(3000);
		HalReturnToFirmware(HalRebootQuiesceRoutine); 
	} FreeSession();
	!Globals::Crl ? Utilities::NotifyPopup(L"Umbrella Connected And Passed XKE"), Globals::Crl = TRUE : NULL;
	XNotifyQueueUI(XNOTIFYUI_TYPE_GENERIC, 0, 2, L"Welcome to UmbrellaLive", 0);
	return S_OK;
	ServerPresence();
}
DWORD XamLoaderExecuteAsyncChallengeHook(DWORD Address, DWORD Task, PBYTE r5, DWORD r6, PBYTE Buffer, DWORD Size) {
	XOSC_REQUEST Request; XOSC_RESPONSE Response;
	memcpy(Request.xoscbuffer, Buffer, 0x2E0);
	Request.Title = XamGetCurrentTitleId();
	memcpy(Request.CPUKey, Utilities::GetCPUKey(), 0x10);
	memcpy(Request.KVDigest, Globals::KVDigest, 0x14);
	memcpy(Request.Final1, (PDWORD)0x90016700, 0x10);
	memcpy(Request.Final2, (PDWORD)0x90017063, 0x8);
	if (ProcessCommand(COMMAND_XOSC, &Request, sizeof(XOSC_REQUEST), &Response, sizeof(XOSC_RESPONSE), FALSE)) {
		if (Globals::Authenticated = (Response.Status == 0x5A000000)) {
			memset(Buffer, 0, Size);
			if (!ReceiveData(Buffer, 0x2E0)) { Sleep(3000); Utilities::NotifyPopup(L" - Failed XOSC Response Rebooting!"); HalReturnToFirmware(HalRebootQuiesceRoutine);
			}
			if (BufferIsNull(Buffer, 0x2E0)) { Utilities::NotifyPopup(L" - Bad XOSC Response, Rebooting!"); HalReturnToFirmware(HalRebootQuiesceRoutine);
			}
			memcpy(Buffer + 0x50, Globals::CPUKeyDigest, 0x10);
		}
		else {
			Sleep(3000);
			HalReturnToFirmware(HalRebootQuiesceRoutine);
		}
	}
	else {
		Sleep(3000);
		HalReturnToFirmware(HalRebootQuiesceRoutine);
	} FreeSession();
	return S_OK;
}
BOOL InitializeSystemHooks() {
	if (Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x12B, (DWORD)RtlImageXexHeaderFieldHook) != S_OK) return FALSE;
	if (Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x198, (DWORD)XexLoadExecutableHook) != S_OK) return FALSE;
	if (Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x199, (DWORD)XexLoadImageHook) != S_OK) return FALSE;
	if (Utilities::PatchModuleImport(MODULE_XAM, MODULE_KERNEL, 0x25F, (DWORD)XeKeysExecuteHook) != S_OK) return FALSE;
	Utilities::PatchInJump((PDWORD)0x8169CD98, (DWORD)XamLoaderExecuteAsyncChallengeHook, FALSE);
	return TRUE;
}