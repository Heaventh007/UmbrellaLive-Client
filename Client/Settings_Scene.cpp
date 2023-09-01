#include "stdafx.h"
namespace StealthTab {
	HRESULT SettingsScnTab1::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
		GetChildById(L"btnClaimToken", &btnClaimToken);
		GetChildById(L"btnCheckTime", &btnCheckTime);
		return ERROR_SUCCESS;
	}
	HRESULT SettingsScnTab1::OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled) {
		if (hObjPressed == btnCheckTime) {
			if (Globals::Status >= 0x7A000000) { sprintf(NotifyBuffer, "Freemode is currently active this feature is unavailable."); }
			else if (Globals::Hours < 0 || Globals::Minutes < 0 || (Globals::Hours == 0 && Globals::Minutes == 0)) { sprintf(NotifyBuffer, "0D 0H 0M"); }
			else if (Globals::Days >= 500) { sprintf(NotifyBuffer, "Lifetime!"); }
			else if (Globals::ReserveDays >= 0 && Globals::Hours >= 0 && Globals::Minutes >= 0) { sprintf(NotifyBuffer, "%iD %iH %iM", Globals::ReserveDays, Globals::Hours, Globals::Minutes); }
			mbstowcs(NotifyText, NotifyBuffer, strlen(NotifyBuffer) + 1);
			XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, XUSER_INDEX_ANY, XNOTIFYUI_PRIORITY_HIGH, NotifyText, NULL);
		}
		else if (hObjPressed == btnClaimToken) {
			HANDLE hThread; DWORD threadId;
			ExCreateThread(&hThread, 0, &threadId, (VOID*)XapiThreadStartup, (LPTHREAD_START_ROUTINE)InitiateTokenSystem, NULL, 0x2 | CREATE_SUSPENDED);
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_ABOVE_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
		bHandled = TRUE;
		return S_OK;
	}
	HRESULT SettingsScnTab1::InitializeChildren() { return ERROR_SUCCESS; }
}
namespace ChStealthogTab {
	HRESULT SettingsScnTab2::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
		txt_ChStealthog.SetText(L"UmbrellaLive Base R1.0\nOfficial Release");
		return ERROR_SUCCESS;
	}
	HRESULT SettingsScnTab2::OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled)
	{
		GetChildById(L"txt_ChStealthog", &txt_ChStealthog);
		return S_OK;
	}
}
namespace HUDTab {
	HRESULT SettingsScnTab3::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
		GetChildById(L"btnCustomGradient", &btnGradient);
		GetChildById(L"btnCustomBG", &btnGradientBG);
		GetChildById(L"btnApply", &btnApply);
		return ERROR_SUCCESS;
	}
	HRESULT SettingsScnTab3::OnNotifyPress(HXUIOBJ hObjPressed, BOOL& bHandled) {
		if (btnGradient.IsChecked()) {
			if (hObjPressed == btnApply) {
				Utilities::NotifyPopup(L"Custom Gradient Enabled");
			}
		}
		else if (btnGradient.IsChecked()) {
			if (hObjPressed == btnApply) {
				Utilities::NotifyPopup(L"Custom Gradient Disabled");
			}
		}
		return S_OK;
	}
}