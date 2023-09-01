#include "stdafx.h"
//typedef VOID(*XAMBUILDRESOURCELOCATORV2)(HANDLE hModule, PWCHAR wModuleName, PWCHAR const cdModule, PWCHAR pBuffer, ULONG ddSize);
//XAMBUILDRESOURCELOCATORV2 XamBuildResourceLocator = (XAMBUILDRESOURCELOCATORV2)Utilities::ResolveFunction("xam.xex", 0x31B);
DWORD hThreadId; extern HANDLE dllHandle; CHAR Buffer[0x1000]; CHAR MessageBuffer[0x100]; WCHAR Message[0x100];
PWCHAR toWCHAR(PCHAR Text, ...) {
	va_list pArgList;
	va_start(pArgList, Text);
	vsprintf(Buffer, Text, pArgList);
	va_end(pArgList);
	sprintf(MessageBuffer, Buffer);
	mbstowcs(Message, MessageBuffer, strlen(MessageBuffer) + 1);
	ZeroMemory(Buffer, sizeof(Buffer));
	ZeroMemory(MessageBuffer, sizeof(MessageBuffer));
	return Message;
}
namespace XUI {
	HRESULT ScnGuideInfo::OnRender(XUIMessageRender *pRenderData, BOOL& bHandled) {
		InitializeChildren();
		wchar_t Text[64];
		wchar_t Unbanned[64];
		if (Globals::Status >= 0x7A000000) {
			txt_Time.SetText(Utilities::PCHARtoWCHAR("Free Mode"));
		}
		else if (Globals::Days >= 500) { txt_Time.SetText(Utilities::PCHARtoWCHAR("LifeTime")); }
		else if (Globals::ReserveDays >= 0 && Globals::Hours >= 0 && Globals::Minutes >= 0){
			txt_Time.SetText(Utilities::PCHARtoWCHAR("%iD %iH %iM", Globals::ReserveDays, Globals::Hours, Globals::Minutes));
		
		}
		
		txt_KVDays.SetText(Utilities::PCHARtoWCHAR("%iD %iH %iM", Globals::DaysUnbanned, Globals::HoursUnbanned, Globals::MinutesUnbanned));
		Sleep(1000);

		//HUDHooks::RefreshUIText_Hook();
		return ERROR_SUCCESS;
	}
	DWORD ScnGuideInfo::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
		return ERROR_SUCCESS;
	}
	DWORD ScnGuideInfo::InitializeChildren() {
		GetChildById(L"Time", &txt_Time);
		GetChildById(L"Unbanned", &txt_KVDays);
		return ERROR_SUCCESS;
	}
	HRESULT XuiRegisterClassHook(const XUIClass * pClass, HXUICLASS *phClass)
	{
		HRESULT ret = XuiRegisterClass(pClass, phClass);
		HUD_UI::Tab_Stealth::Register();
		HUD_UI::Tab_UI::Register();
		HUD_UI::Tab_Bypasses::Register();
		HUD_UI::Tab_Cheats::Register();
		HUD_UI::Tab_Changes::Register();
		ScnGuideInfo::Register();
		return ret;
	}
	HRESULT XuiUnregisterClassHook(LPCWSTR szClassName)
	{
		HRESULT ret = XuiUnregisterClass(szClassName);
		ScnGuideInfo::Unregister();
		HUD_UI::Tab_Stealth::Unregister();
		HUD_UI::Tab_UI::Unregister();
		HUD_UI::Tab_Bypasses::Unregister();
		HUD_UI::Tab_Cheats::Unregister();
		HUD_UI::Tab_Changes::Unregister();
		return ret;
	}
}
namespace HUDHooks {
	HXUIOBJ ChildScene;
	VOID SetUIText(LPCWSTR Header, LPCWSTR Text) {
		HXUIOBJ childHeader;
		if (XuiElementGetChildById(ChildScene, Header, &childHeader) == 0)
			XuiTextElementSetText(childHeader, Text);
	}

	HRESULT XuiSceneCreateCallbackPostRef(LPCWSTR szBasePath, LPCWSTR szScenePath, PVOID pvInitData, HXUIOBJ *phScene) {
		WCHAR cmp[64];
		if (lstrcmpW(szScenePath, L"Menu11.xur") == 0) {
			HXUIOBJ m_hRootScene = *phScene;
			HXUIOBJ newScene;
			//swprintf(cmp, L"memory://%X,%X#PreferencesScene.xur", xzp::XuiData, sizeof((xzp::XuiData)), dllHandle);
			XuiSceneCreate(NULL, cmp, pvInitData, phScene);
			XuiElementAddChild(m_hRootScene, newScene);
		}
		return S_OK;
	}
	HRESULT XuiSceneCreateHook(LPCWSTR BasePath, LPCWSTR ScenePath, PVOID InitData, HXUIOBJ *hScene) {
		WCHAR cmp[64];
		HRESULT ret = E_FAIL;
		swprintf(cmp, L"memory://%X,%X#GuideInfo11.xur", xzp::XuiData, sizeof((xzp::XuiData)), dllHandle);
		if (lstrcmpW(ScenePath, L"GuideInfo11.xur") == 0) {
			XuiSceneCreate(NULL, cmp, InitData, hScene);
			HXUIOBJ hChild;
			XuiElementAddChild(*hScene, hChild);
		}

		else if (lstrcmpW(BasePath, cmp) == 0) {
			XuiSceneCreate(L"section://@0,hud#", ScenePath, InitData, hScene);
			if (lstrcmpW(ScenePath, L"SettingsTabSignedIn.xur") == 0 || lstrcmpW(ScenePath, L"SettingsTabSignedOut.xur") == 0) {
			}
		}
		else {
			XuiSceneCreate(BasePath, ScenePath, InitData, hScene);
		}

		ret = XuiSceneCreateCallbackPostRef(BasePath, ScenePath, InitData, hScene);
		return ret;
	}
	VOID UpdateUIText() {
		wchar_t Text[64];
		wchar_t Unbanned[64];
		if (XuiHandleIsValid(ChildScene)) {
			if (Globals::Status >= 0x7A000000) { swprintf(Text, L"Freemode"); }
			else if (Globals::Days >= 500) { swprintf(Text, L"Lifetime!"); }
			else if (Globals::Hours < 0 || Globals::Minutes < 0 || (Globals::Hours == 0 && Globals::Minutes == 0)) { swprintf(Text, L"0D 0H 0M"); }
			else if (Globals::ReserveDays >= 0 && Globals::Hours >= 0 && Globals::Minutes >= 0) { swprintf(Text, L"%iD %iH %iM", Globals::ReserveDays, Globals::Hours, Globals::Minutes); }
			swprintf(Unbanned, L"%i", Globals::DaysUnbanned);
			SetUIText(L"Time", Text);
			SetUIText(L"Unbanned", Unbanned);
		}
		Sleep(1000);
	}
	VOID RefreshUIText() {
		while (true) {
			UpdateUIText();
		}
	}
	VOID RefreshUIText_Hook(){
		//Utilities::MakeThread((LPTHREAD_START_ROUTINE)RefreshUIText);
	}

}

