#include "stdafx.h"
namespace XUI {
	HRESULT XuiRegisterClassHook(const XUIClass * pClass, HXUICLASS *phClass);
	HRESULT XuiUnregisterClassHook(LPCWSTR szClassName);
	class ScnGuideInfo : CXuiSceneImpl {
	public:
		ScnGuideInfo() {} ~ScnGuideInfo() {}
		static ScnGuideInfo& getInstance() {
			static ScnGuideInfo singleton;
			return singleton;
		}
		CXuiTextElement txt_Time, txt_KVDays;
		XUI_IMPLEMENT_CLASS(ScnGuideInfo, L"SyndicateText", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_RENDER(OnRender)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		HRESULT OnRender(XUIMessageRender *pRenderData, BOOL& bHandled);
		DWORD InitializeChildren();
	};
}
namespace HUDHooks {
	HRESULT XuiSceneCreateHook(LPCWSTR BasePath, LPCWSTR ScenePath, PVOID InitData, HXUIOBJ *Scene);
	VOID RefreshUIText_Hook();
}

namespace Dashhooks {
	VOID DashhookStart();
}

#pragma once
#ifndef HUD_H
#define HUD_H

#include "stdafx.h"
#include <stdio.h>
//#include "Utilities.h"
#define HUD_FamilySettings_String  "Reset INI"
#define HUD_FamilySettings_Len     15
#define HUD_XBOXGUIDE "RES Live  "
#define HUD_XBOXGUIDE_Len 10
#define HUD_XBOXGUIDE2 "RES Live  "
#define HUD_XBOXGUIDE_Len2 10
int HUD_PatchLabels(void);
typedef DWORD(*HUD_FILLLAUNCHDATA)(DWORD* XDashLaunchData, DWORD r4, DWORD selection);
typedef DWORD(*HUD_BOOTTODASHHELPER)(DWORD* _XUIOBJ, _XDASHLAUNCHDATA* LaunchData, DWORD* cstr, DWORD* r6, DWORD* r7);
DWORD HUD_BootToDashHelper_Hook(DWORD* _XUIOBJ, _XDASHLAUNCHDATA* LaunchData, DWORD* cstr, DWORD* r6, DWORD* r7);
typedef struct __HUDOffsets
{
	DWORD FamilySettings_LaunchStr;
	DWORD BootToDashHelper_Jump;
	DWORD LaunchData_FamilySettings;
	DWORD BootToDashHelper_Func;
	DWORD FamilySettings_Str1;
	DWORD FamilySettings_Str2;
	DWORD xuiLabel_Header_Label_Head;
} HUDOffsets;

static HUDOffsets hud_17544 = {//17559
	0x913F035C , //FamilySettings_LaunchStr 
	0x913E64A0 , //BootToDashHelper_Jump
	0x14, //LaunchData_FAMILYSETTINGS
	0x913E62C8, //HUD_BootToDashHelper_Func 
	0x11F02, //btnFamilySettings
	0x119B8, //btnFamilySettingsOnline
	0xA511, //XboxGuide
};
#endif

/*namespace XUI {
	HRESULT XuiRegisterClassHook(const XUIClass * pClass, HXUICLASS *phClass);
	HRESULT XuiUnregisterClassHook(LPCWSTR szClassName);
	class ScnGuideInfo : CXuiSceneImpl {
	public:
		ScnGuideInfo() {} ~ScnGuideInfo() {}
		static ScnGuideInfo& getInstance() {
			static ScnGuideInfo singleton;
			return singleton;
		}
		CXuiTextElement txt_Time, txt_KVDays;
		XUI_IMPLEMENT_CLASS(ScnGuideInfo, L"SyndicateText", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_RENDER(OnRender)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		HRESULT OnRender(XUIMessageRender *pRenderData, BOOL& bHandled);
		DWORD InitializeChildren();
	};
}
namespace HUDHooks {
	HRESULT XuiSceneCreateHook(LPCWSTR BasePath, LPCWSTR ScenePath, PVOID InitData, HXUIOBJ *Scene);
	VOID RefreshUIText_Hook();
}*/