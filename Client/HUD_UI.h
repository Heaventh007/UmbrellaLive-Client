#pragma once
#include "stdafx.h"
namespace HUD_UI {
	typedef INT(*tSendNotifyPress)(HXUIOBJ r3, WORD r4, WORD r5);
	typedef HRESULT(*tXuiElementBeginRender)(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);
	extern HXUIOBJ hGuideMain;
	extern tSendNotifyPress SendNotifyPressStub;
	extern tXuiElementBeginRender HUD_XuiElementBeginRenderStub;
	extern HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2, BOOL Gradient);
	extern HXUIOBJ newScene; 
	extern HXUIOBJ hChild;
	extern HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct, PWCHAR base, PWCHAR scene, void* init, HXUIOBJ* hscene);
	extern INT SendNotifyPressHook(HXUIOBJ r3, WORD r4, WORD r5);

	class Tab_Stealth : CXuiSceneImpl {
	public:
		CXuiControl btntoken, btnHelp, btnResetXbdm, btnGurdian, btnBlockLiveDns;

		XUI_IMPLEMENT_CLASS(Tab_Stealth, L"iniTab1", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()
			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class Tab_UI : CXuiSceneImpl {
	public:
		CXuiSlider slider_R, slider_G, slider_B, colorpreview, Scroller, slider_R1, slider_G1, slider_B1;
		HXUIOBJ hbtnPreview;

		CXuiTextElement labeldesc;
		CXuiCheckbox checkboxCustomNotification, checkboxDashUiModifications, checkboxHudColorModifications, checkboxDrawGuideInformation, checkcolor1,  PreviewCheckbox1;
		CXuiControl btnSave;

		XUI_IMPLEMENT_CLASS(Tab_UI, L"iniTab2", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_ON_XM_NOTIFY_VALUE_CHANGED(OnNotifyValueChanged)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
		HRESULT OnNotifyValueChanged(HXUIOBJ hObjSource, XUINotifyValueChanged *pNotifyValueChangedData, BOOL& bHandled);
	};

	class Tab_Bypasses : CXuiSceneImpl {
	public:
		CXuiTextElement labeldesc;
		CXuiCheckbox checkboxGhosts, checkboxAw, checkboxBO2, checkboxBO3;
		CXuiControl btnSave;

		XUI_IMPLEMENT_CLASS(Tab_Bypasses, L"iniTab3", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class Tab_Cheats : CXuiSceneImpl {
	public:
		CXuiTextElement labeldesc;
		CXuiCheckbox checkboxCheats, checkboxGTAV;
		CXuiControl btnSave;

		XUI_IMPLEMENT_CLASS(Tab_Cheats, L"iniTab4", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

	class Tab_Changes : CXuiSceneImpl {
	public:
		CXuiListItem ListChanges;
		CXuiTextElement labeldesc;

		XUI_IMPLEMENT_CLASS(Tab_Changes, L"iniTab5", XUI_CLASS_SCENE)
			XUI_BEGIN_MSG_MAP()
			XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
			XUI_ON_XM_INIT(OnInit)
			XUI_END_MSG_MAP()

			DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
		DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	};

}	
typedef struct
{/*Cant remember what this was for?*/
	string ContentDisplayText;
	int ContentBkgIndex;
	int ContentBkgVisual;
	int ContentIconVisual;
	int ContentIconIndex;
	int ContentReflectVisual;
	int ContentReflectIndex;
	bool ContentAnimState;
	bool ContentGradientState;
	bool ContentIconState;
	DWORD ContentMainTextColor;
	DWORD ContentReflectTextColor;
} TabDataPackage;
#pragma once