#include "stdafx.h"
VOID DisplayLiveBlockMessage() {
	PCWSTR Buttons[0x2] = { L"Enable LiveBlock", L"Disable LiveBlock" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"XBLUmbrella | Are you sure?", L"You are about to block XboxLives Dns which will result in a disconnection from XboxLive.\nPressing yes will Enable the block feature, while pressing Disable will disable the block feature.\nNote LiveBlock will be Enabled until you disable it here again!", ARRAYSIZE(Buttons), Buttons, NULL, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Sleep(500);

	if (Result.dwButtonPressed == 0) {
		INI::EnableBlockXblDns = TRUE;
		INI::UpdateSettings();
		Utilities::SetLiveBlock(INI::EnableBlockXblDns);
		Utilities::NotifyPopup(L"AWWW Man You Need To Fix This  - The LiveDns has been [blocked]!");
	}
	if (Result.dwButtonPressed == 1) {
		INI::EnableBlockXblDns = FALSE;
		INI::UpdateSettings();
		Utilities::SetLiveBlock(INI::EnableBlockXblDns);
		Utilities::NotifyPopup(L"XBLUmbrella  - The LiveDns has been [Unblocked]!");
	}
}
VOID DisplayLiveHelpMenu() {
	PCWSTR Buttons[0x1] = { L"Got it Daddy!"};
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"XBLUmbrella | Help", L"Join Our Discord For Support... TBH THIS IS TBHA IN NEXT UPDATE S1!", ARRAYSIZE(Buttons), Buttons, NULL, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Sleep(500);

	if (Result.dwButtonPressed == 0) {

	}
}
DWORD HUD_UI::Tab_Stealth::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	this->GetChildById(L"btntoken", &btntoken);
	this->GetChildById(L"btnHelp", &btnHelp);
	this->GetChildById(L"btnResetXbdm", &btnResetXbdm);
	this->GetChildById(L"btnGurdian", &btnGurdian);
	this->GetChildById(L"btnBlockLiveDns", &btnBlockLiveDns);
	btntoken.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#Tokens.png", xzp::XuiData, sizeof(xzp::XuiData)));
	btnHelp.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#help.png", xzp::XuiData, sizeof(xzp::XuiData)));
	btnResetXbdm.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#XbdmRefresh.png", xzp::XuiData, sizeof(xzp::XuiData)));
	btnGurdian.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#Weed.png", xzp::XuiData, sizeof(xzp::XuiData)));
	btnBlockLiveDns.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#entherneticon.png", xzp::XuiData, sizeof(xzp::XuiData)));
	bHandled = TRUE;
	return ERROR_SUCCESS;
}
DWORD HUD_UI::Tab_Stealth::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == btntoken)
		if (Globals::Status >= 0x7A000000)
			Utilities::NotifyPopup(L"XBLUmbrella - Free Mode, no redeeming tokens");
		else
		Utilities::MakeThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(InitiateTokenSystem));
	else if (hObjPressed == btnHelp)//
		Utilities::MakeThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayLiveHelpMenu));
	else if (hObjPressed == btnResetXbdm) {
		XexUnloadImage(GetModuleHandle("xbdm.xex"));
		if (SUCCEEDED(XexLoadImage("Hdd:\\xbdm.xex", XEX_LOADIMG_FLAG_DLL, NULL, nullptr)))
			Utilities::NotifyPopup(L"XBLUmbrella Reset - Successfully reset xbdm!");
	}
	else if (hObjPressed == btnGurdian)
		KVGuardSetup();
	else if (hObjPressed == btnBlockLiveDns)
		Utilities::MakeThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayLiveBlockMessage));

	bHandled = TRUE;
	return ERROR_SUCCESS;
}