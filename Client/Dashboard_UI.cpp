#include "stdafx.h"
HANDLE DashboardHandle;
DWORD Color1 = 0xFFFF00;// tiles // black 000000
DWORD Color2 = 0xFFFF00;// background // Platinum color is E5E4E2
DWORD Color3 = 0xFFFF00;//
DWORD Color4 = 0xFFFF00;//
//For Rainbow Off
DWORD Color5 = 0xFF000000;//
DWORD Color6 = 0xFF000000;//
DWORD Color7 = 0xFF000000;//
DWORD Color8 = 0xFF000000;//


DWORD Color9  = 0xFF000000;
DWORD Color10 = 0xFF000000;
DWORD Color11 = 0xFF000000;
DWORD Color12 = 0xFF000000;

namespace Dashboard_UI {
	tXuiElementBeginRender XuiElementBeginRenderStub;

	HXUIOBJ objLEImage;
	HRESULT AddImage(HXUIOBJ ObjToAttatchTo, HXUIOBJ* Obj, LPCWSTR Id, FLOAT x, FLOAT y, LPCWSTR ImagePath) {
		D3DXVECTOR3 pos;
		Dashboard_Structures::XUIElementPropVal propVal;

		Dashboard_Calls::XuiCreateObject(XUI_CLASS_IMAGE, Obj);

		Dashboard_Calls::XuiElementSetBounds(*Obj, 255.0, 28.0);

		pos.x = x; pos.y = y; pos.z = 0;
		Dashboard_Calls::XuiElementSetPosition(*Obj, &pos);

		Dashboard_Calls::XUIElementPropVal_SetString(&propVal, Id);
		Dashboard_Calls::SetProperty(*Obj, L"Id", &propVal);

		Dashboard_Calls::XUIElementPropVal_SetString(&propVal, ImagePath);
		Dashboard_Calls::SetProperty(*Obj, L"ImagePath", &propVal);

		return Dashboard_Calls::XuiElementAddChild(ObjToAttatchTo, *Obj);
	}

	HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct) {
		HXUIOBJ hParentObj; LPCWSTR ObjID, ParentText;
		Dashboard_Calls::XuiElementGetId(hObj, &ObjID);
		Dashboard_Calls::XuiElementGetParent(hObj, &hParentObj);
		Dashboard_Calls::XuiElementGetId(hParentObj, &ParentText);
		if (NT_SUCCESS(XuiElementBeginRenderStub(hObj, pRenderData, pRenderStruct))){
			HXUIOBJ randomasshandlefunction = ((HXUIOBJ(*)(...))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10681))();			
			if (INI::DashUiModifications)
			{
				if (INI::linear) {
					if (lstrcmpW(ObjID, L"Front") == 0)
						Dashboard_Calls::SetFillLINEAR(hObj, Color1, 0xFF000000, INI::front);
					else if (lstrcmpW(ObjID, L"Background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"_Background") == 0)
						Dashboard_Calls::SetFillLINEAR(hObj, Color1, 0xFF000000, INI::front);
					else if (lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0 ||
						lstrcmpW(ParentText, L"Back") == 0 || lstrcmpW(ParentText, L"Color") == 0 || lstrcmpW(ParentText, L"_SceneHost") == 0)
						Dashboard_Calls::SetFillLINEAR(hObj, Color3, 0xFF000000, INI::front);
				}
				else if (INI::Rainbow) {
					if (lstrcmpW(ObjID, L"Front") == 0)
						Dashboard_Calls::RotatingSetFillDash(hObj, Color5, Color6, INI::front, TRUE);
					else if (lstrcmpW(ObjID, L"Background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"_Background") == 0)
						Dashboard_Calls::RotatingSetFillDash(hObj, Color6, Color6, INI::front, TRUE);
					else if (lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0 ||
						lstrcmpW(ParentText, L"Back") == 0 || lstrcmpW(ParentText, L"Color") == 0 || lstrcmpW(ParentText, L"_SceneHost") == 0)
						Dashboard_Calls::RotatingSetFillDash(hObj, Color6, Color5, INI::front, TRUE);
				}
				else if (INI::Radial)
				{
					if (lstrcmpW(ObjID, L"Front") == 0)
						Dashboard_Calls::SetFillRADIAL(hObj, Color1, 0xFF000000, INI::front);
					else if (lstrcmpW(ObjID, L"Background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"_Background") == 0)
						Dashboard_Calls::SetFillRADIAL(hObj, Color1, 0xFF000000, INI::front);
					else if (lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0 ||
						lstrcmpW(ParentText, L"Back") == 0 || lstrcmpW(ParentText, L"Color") == 0 || lstrcmpW(ParentText, L"_SceneHost") == 0)
						Dashboard_Calls::SetFillRADIAL(hObj, Color3, 0xFF000000, INI::front);
					return ERROR_SUCCESS;
				}
				else {
					if (lstrcmpW(ObjID, L"Front") == 0)
						Dashboard_Calls::SetFillLINEAR(hObj, Color1, 0xFF000000, INI::front);
					else if (lstrcmpW(ObjID, L"Background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"_Background") == 0)
						Dashboard_Calls::SetFillLINEAR(hObj, Color1, 0xFF000000, INI::front);
					else if (lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0 ||
						lstrcmpW(ParentText, L"Back") == 0 || lstrcmpW(ParentText, L"Color") == 0 || lstrcmpW(ParentText, L"_SceneHost") == 0)
						Dashboard_Calls::SetFillLINEAR(hObj, Color3, 0xFF000000, INI::front);
					return ERROR_SUCCESS;
				}

			}
			
		}

		return E_FAIL;
	}

	VOID ResolveFunctions() {
		Dashboard_Calls::XuiElementGetId = (HRESULT(*)(HXUIOBJ, LPCWSTR*))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10176);
		Dashboard_Calls::XuiElementGetChildById = (HRESULT(*)(HXUIOBJ, LPCWSTR, HXUIOBJ*))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10177);
		Dashboard_Calls::XuiElementAddChild = (HRESULT(*)(HXUIOBJ, HXUIOBJ))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10155);
		Dashboard_Calls::XuiElementGetParent = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10157);
		Dashboard_Calls::XuiFigureSetFill = (HRESULT(*)(HXUIOBJ, XUI_FILL_TYPE, DWORD, XUIGradientStop*, int, float, const D3DXVECTOR2*, const D3DXVECTOR2*))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10210);
		Dashboard_Calls::XuiCreateObject = (HRESULT(*)(LPCWSTR, HXUIOBJ *))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10028);
		Dashboard_Calls::XuiElementSetBounds = (HRESULT(*)(HXUIOBJ, float, float))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10165);
		Dashboard_Calls::XuiElementSetPosition = (HRESULT(*)(HXUIOBJ, const D3DXVECTOR3*))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10167);
		Dashboard_Calls::XUIElementPropVal_SetString = (HRESULT(*)(Dashboard_Structures::XUIElementPropVal*, LPCWSTR))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10094);
		Dashboard_Calls::XuiObjectGetProperty = (HRESULT(*)(HXUIOBJ, DWORD, DWORD, Dashboard_Structures::XUIElementPropVal*))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10047);
		Dashboard_Calls::XuiObjectSetProperty = (HRESULT(*)(HXUIOBJ, DWORD, DWORD, const Dashboard_Structures::XUIElementPropVal *))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10048);
		Dashboard_Calls::XuiObjectGetPropertyId = (HRESULT(*)(HXUIOBJ, LPCWSTR, DWORD *))(DWORD)Utilities::ResolveFunction((HMODULE)DashboardHandle, 10044);
	}
}


