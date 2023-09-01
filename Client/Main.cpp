#include "stdafx.h"
#include "INIReader.h"
HANDLE dllHandle = NULL;
typedef VOID(*XAMBUILDRESOURCELOCATORV2)(HANDLE hModule, PWCHAR wModuleName, PWCHAR const cdModule, PWCHAR pBuffer, ULONG ddSize);
XAMBUILDRESOURCELOCATORV2 XamBuildResourceLocator = (XAMBUILDRESOURCELOCATORV2)Utilities::ResolveFunction("xam.xex", 0x31B);
BOOL Initialize() {
	INI::Init();
	INI::Read();
	if (!Utilities::FindPaths()) return FALSE;
	if (!Utilities::InitializeHvxPeekPoke()) return FALSE;
	if (!Utilities::SetKeyVault()) return FALSE;
	if (!GetKVSetDate(Globals::pathKeyVault)) return FALSE;
	Utilities::SetLiveBlock(TRUE);
	if (!InitializeSystemHooks()) return FALSE;
	HANDLE xam;
	xam = GetModuleHandle("xam.xex");
	HUD_UI::HUD_XuiElementBeginRenderStub = (HUD_UI::tXuiElementBeginRender)Utilities::HookFunctionStub((DWORD)Utilities::ResolveFunction((HMODULE)xam, 936), HUD_UI::XuiElementBeginRenderHook),HUD_UI::SendNotifyPressStub = (HUD_UI::tSendNotifyPress)Utilities::HookFunctionStub(0x817CA3A0, HUD_UI::SendNotifyPressHook);
	InitializeConnection();
	return TRUE;
}
BOOL APIENTRY DllMain(HANDLE Handle, DWORD Reason, PVOID Reserved) {
	dllHandle = Handle;
	if (Reason == DLL_PROCESS_ATTACH) {
		//ProtectMyMemorySpace();
		if (Utilities::TrayOpen()) return 0xC0000001;
		else if (!Initialize()) HalReturnToFirmware(HalResetSMCRoutine);
	} return TRUE;
}
VOID XamLivelogo(PWCHAR pResource, PWCHAR pBuffer, DWORD dwSize) {
	HANDLE hXam = NULL;
	if (lstrcmpW(pResource, L"loadingRing.png") == 0) {
		swprintf(pBuffer, L"memory://%X,%X#loadingRing.png", xzp::XuiData, sizeof((xzp::XuiData)), dllHandle);
	}
	else if (lstrcmpW(pResource, L"livelogo.png") == 0) {
		swprintf(pBuffer, L"memory://%X,%X#livelogo.png", xzp::XuiData, sizeof((xzp::XuiData)), dllHandle);
	}
	else XamBuildResourceLocator(dllHandle, L"shrdres", pResource, pBuffer, dwSize);
}