#include "stdafx.h"

DWORD HUD_UI::Tab_Changes::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	this->GetChildById(L"lblDesc", &labeldesc);
	this->GetChildById(L"ListChanges", &ListChanges);
	
	ListChanges.SetText(L"Pre Installment of XBLUmbrella Enjoy Guys!\n ");

	bHandled = TRUE;
	return ERROR_SUCCESS;
}

DWORD HUD_UI::Tab_Changes::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	bHandled = TRUE;
	return ERROR_SUCCESS;
}