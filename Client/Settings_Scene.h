#pragma once
#include "stdafx.h"
namespace StealthTab {
	class SettingsScnTab1 : CXuiSceneImpl
	{
	public:
		SettingsScnTab1() {}
		~SettingsScnTab1() {}
		static SettingsScnTab1& getInstance() {
			static SettingsScnTab1 singleton;
			return singleton;
		}
		CXuiControl btnCheckTime, btnClaimToken, btnChStealthog;
		XUI_IMPLEMENT_CLASS(SettingsScnTab1, L"iniTab1", XUI_CLASS_SCENE)
		XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT(OnInit)
		XUI_ON_XM_NOTIFY_PRESS(OnNotifyPress)
		XUI_END_MSG_MAP()
		HRESULT OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		HRESULT OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled);
		HRESULT InitializeChildren();
	};
}
namespace ChStealthogTab {
	class SettingsScnTab2 : CXuiSceneImpl {
	public:
		SettingsScnTab2() {}
		~SettingsScnTab2() {}
		static SettingsScnTab2& getInstance() {
			static SettingsScnTab2 singleton;
			return singleton;
		}
		CXuiControl btnApply;
		CXuiCheckbox btnBO1Cheats, btnBO2Cheats, btnBO3Cheats;
		CXuiTextElement txt_ChStealthog;
		XUI_IMPLEMENT_CLASS(SettingsScnTab2, L"iniTab2", XUI_CLASS_SCENE)
		XUI_BEGIN_MSG_MAP()
		XUI_ON_XM_INIT(OnInit)
		XUI_ON_XM_NOTIFY_PRESS(OnNotifyPress)
		XUI_END_MSG_MAP()
		HRESULT OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		HRESULT OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

}
namespace HUDTab {
	class SettingsScnTab3 : CXuiSceneImpl {
	public:
		SettingsScnTab3() {}
		~SettingsScnTab3() {}
		static SettingsScnTab3& getInstance() {
			static SettingsScnTab3 singleton;
			return singleton;
		}
		CXuiControl btnApply;
		CXuiCheckbox btnGradient, btnGradientBG;
		XUI_IMPLEMENT_CLASS(SettingsScnTab3, L"iniTab3", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()

			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_NOTIFY_PRESS(OnNotifyPress)

			XUI_END_MSG_MAP()

		HRESULT OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		HRESULT OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled);
	};
}