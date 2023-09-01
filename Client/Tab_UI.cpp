#include "stdafx.h"
bool SettingsTabSceneInit = false;
DWORD HUD_UI::Tab_UI::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	SettingsTabSceneInit = false;
	INI::Read();
	this->GetChildById(L"slider_R", &slider_R);
	this->GetChildById(L"slider_G", &slider_G);
	this->GetChildById(L"slider_B", &slider_B);
	this->GetChildById(L"colorpreview", &colorpreview);
	this->GetChildById(L"slider_R1", &slider_R1);
	this->GetChildById(L"slider_G1", &slider_G1);
	this->GetChildById(L"slider_B1", &slider_B1);
	//Start OF UI
	this->GetChildById(L"lblDesc", &labeldesc);
	this->GetChildById(L"checkboxCustomNotification", &checkboxCustomNotification);
	this->GetChildById(L"checkboxDashUiModifications", &checkboxDashUiModifications);
	this->GetChildById(L"checkboxHudColorModifications", &checkboxHudColorModifications);
	this->GetChildById(L"checkboxDrawGuideInformation", &checkboxDrawGuideInformation);
	this->GetChildById(L"checkcolor1", &checkcolor1);
	this->GetChildById(L"PreviewCheckbox1", &PreviewCheckbox1);
	this->GetChildById(L"btnPreview", &hbtnPreview);
	this->GetChildById(L"btnSave", &btnSave);
	this->GetChildById(L"Scroller", &Scroller);
	btnSave.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#Weed.png", xzp::XuiData, sizeof(xzp::XuiData)));
	checkboxCustomNotification.SetCheck(INI::CustomNotification);
	checkboxDashUiModifications.SetCheck(INI::DashUiModifications);
	checkboxDrawGuideInformation.SetCheck(INI::DrawGuideInformation);
	checkboxHudColorModifications.SetCheck(INI::HudColorModifications);
	checkcolor1.SetCheck(INI::front);
	PreviewCheckbox1.SetCheck(INI::front);
	slider_R.SetValue((int)((Color1 >> 16) & 0xFF));
	slider_G.SetValue((int)((Color1 >> 8) & 0xFF));
	slider_B.SetValue((int)((Color1) & 0xFF));
	slider_R.SetValue((int)((Color2 >> 16) & 0xFF));
	slider_G.SetValue((int)((Color2 >> 8) & 0xFF));
	slider_B.SetValue((int)((Color2) & 0xFF));
	slider_R1.SetValue((int)((Color3 >> 16) & 0xFF));
	slider_G1.SetValue((int)((Color3 >> 8) & 0xFF));
	slider_B1.SetValue((int)((Color3) & 0xFF));
	slider_R1.SetValue((int)((Color4 >> 16) & 0xFF));
	slider_G1.SetValue((int)((Color4 >> 8) & 0xFF));
	slider_B1.SetValue((int)((Color4) & 0xFF));
	HUD_UI::SetFill(Scroller, Color1, Color2, INI::front);
	SettingsTabSceneInit = true;
	bHandled = TRUE;
	return ERROR_SUCCESS;
}
DWORD HUD_UI::Tab_UI::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == checkboxCustomNotification)
		INI::CustomNotification = checkboxCustomNotification.IsChecked();
	else if (hObjPressed == checkboxDashUiModifications)
		INI::DashUiModifications = checkboxDashUiModifications.IsChecked();
	else if (hObjPressed == checkboxHudColorModifications)
		INI::HudColorModifications = checkboxHudColorModifications.IsChecked();
	else if (hObjPressed == checkboxDrawGuideInformation)
		INI::DrawGuideInformation = checkboxDrawGuideInformation.IsChecked();
	else if (hObjPressed == checkcolor1)
		INI::front = checkcolor1.IsChecked();
	else if (hObjPressed == btnSave) {
		INI::UpdateSettings();
		Utilities::NotifyPopup(L"XBLUmbrella - Settings saved and updated!");
	}
	bHandled = TRUE;
	return ERROR_SUCCESS;
}
HRESULT HUD_UI::Tab_UI::OnNotifyValueChanged(HXUIOBJ hObjSource, XUINotifyValueChanged *pNotifyValueChangedData, BOOL& bHandled) {
	int R, G, B;
	int R2, G2, B2;
	if (SettingsTabSceneInit) {
		if ( hObjSource == slider_R.m_hObj || hObjSource == slider_G.m_hObj || hObjSource == slider_B.m_hObj) {
			slider_R.GetValue(&R);
			slider_G.GetValue(&G);
			slider_B.GetValue(&B);
			Color1 = D3DCOLOR_RGBA(R, G, B, 0xFF);
			Color2 = D3DCOLOR_RGBA(R, G, B, 0xFF);
		}
		else if (hObjSource == slider_R1.m_hObj || hObjSource == slider_G1.m_hObj || hObjSource == slider_B1.m_hObj)
		{		
			slider_R1.GetValue(&R);
			slider_G1.GetValue(&G);
			slider_B1.GetValue(&B);
			Color3 = D3DCOLOR_RGBA(R, G, B, 0xFF);
			Color4 = D3DCOLOR_RGBA(R, G, B, 0xFF);
		}
		else if (hObjSource == colorpreview.m_hObj || hObjSource == slider_R1.m_hObj || hObjSource == slider_G1.m_hObj || hObjSource == slider_B1.m_hObj) {
			slider_R1.GetValue(&R2);
			slider_G1.GetValue(&G2);
			slider_B1.GetValue(&B2);
			Color1 = D3DCOLOR_RGBA(R2, G2, B2, 0xFF);
			Color3 = D3DCOLOR_RGBA(R2, G2, B2, 0xFF);
		}
		HUD_UI::SetFill(colorpreview, Color1, Color3, INI::front);
		//HUD_UI::SetFill(colorpreview, Color2, Color4, INI::front);
	}
	bHandled = TRUE;
	return ERROR_SUCCESS;
}