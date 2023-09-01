#include "stdafx.h"

namespace Dashboard_Calls {
	/*For Rotating Rainbow*/
	struct RGB {
		int R, G, B;
	};
	RGB MenuColour2 = { 0, 0, 255 };
	RGB MenuColour = { 255, 0, 0 };
	RGB Rotation = { 50 };
	DWORD Rotations = 50;
	int ToggleRotations = 0;
	int Rainbow = 0;
	int Rainbow2 = 0;
	BOOL r = 0, g = 0, b = 255;
	BOOL r2 = 255, g2 = 0, b2 = 0;
	/*For Rotating Rainbow*/
	HRESULT(*XuiElementGetId)(HXUIOBJ hObj, LPCWSTR *pszId);
	HRESULT(*XuiElementGetChildById)(HXUIOBJ hObj, LPCWSTR szId, HXUIOBJ *phChild);
	HRESULT(*XuiElementAddChild)(HXUIOBJ hObj, HXUIOBJ hChild);
	HRESULT(*XuiElementGetParent)(HXUIOBJ hObj, HXUIOBJ* phParent);
	HRESULT(*XuiFigureSetFill)(HXUIOBJ hObj, XUI_FILL_TYPE nFillType, DWORD dwFillColor, XUIGradientStop *pStops, int nNumStops, float fGradientAngle, const D3DXVECTOR2 *pvScale, const D3DXVECTOR2 *pvTrans);
	HRESULT(*XuiCreateObject)(LPCWSTR szClassName, HXUIOBJ *phObj);
	HRESULT(*XuiElementSetBounds)(HXUIOBJ hObj, float fWidth, float fHeight);
	HRESULT(*XuiElementSetPosition)(HXUIOBJ hObj, const D3DXVECTOR3 *pvPos);
	HRESULT(*XUIElementPropVal_SetString)(Dashboard_Structures::XUIElementPropVal* pThis, LPCWSTR val);
	HRESULT(*XuiObjectGetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, Dashboard_Structures::XUIElementPropVal *pVal);
	HRESULT(*XuiObjectSetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, const Dashboard_Structures::XUIElementPropVal *pVal);
	HRESULT(*XuiObjectGetPropertyId)(HXUIOBJ hObj, LPCWSTR szPropName, DWORD *pdwId);
	HRESULT RotatingSetFillDash(HXUIOBJ hObj, DWORD Color1, DWORD Color2, BOOL Gradient, BOOL RainbowMode) {
		XUI_FILL_TYPE fillType = XUI_FILL_TYPE::XUI_FILL_SOLID;
		if (INI::front)
		fillType = (Gradient) ? XUI_FILL_TYPE::XUI_FILL_LINEAR_GRADIENT : XUI_FILL_TYPE::XUI_FILL_SOLID;
		D3DXVECTOR2 pvScalingFactor(1.0f, 1.0f), pTrnas(0.0f, 0.0f);
		XUIGradientStop gs[2];
		gs[0].dwColor = Color1;
		gs[0].fPos = 0.0f;
		gs[1].dwColor = Color2;
		gs[1].fPos = 1.0f;
		if (RainbowMode) {
			if (Rainbow < GetTickCount() - 15) {
				if (MenuColour.R > 0 && MenuColour.B == 0) {
					MenuColour.R--;
					MenuColour.G++;
				}
				if (MenuColour.G > 0 && MenuColour.R == 0) {
					MenuColour.G--;
					MenuColour.B++;
				}
				if (MenuColour.B > 0 && MenuColour.G == 0) {
					MenuColour.R++;
					MenuColour.B--;
				}
				if (MenuColour2.R > 0 && MenuColour2.B == 0) {
					MenuColour2.R--;
					MenuColour2.G++;
				}
				if (MenuColour2.G > 0 && MenuColour2.R == 0) {
					MenuColour2.G--;
					MenuColour2.B++;
				}
				if (MenuColour2.B > 0 && MenuColour2.G == 0) {
					MenuColour2.R++;
					MenuColour2.B--;
				}
				Color5 = D3DCOLOR_ARGB(0xFF, MenuColour.R, MenuColour.G, MenuColour.B);
				Color6 = D3DCOLOR_ARGB(0xFF, MenuColour2.R, MenuColour2.G, MenuColour2.B);
				Rainbow = GetTickCount();
			}
			if (ToggleRotations < GetTickCount() - 25) {
				Rotations++;
				ToggleRotations = GetTickCount();
			}
		}
		return Dashboard_Calls::XuiFigureSetFill(hObj, fillType, Color1, gs, 2, Rotations, &pvScalingFactor, &pTrnas);
	}
	HRESULT SetFillRADIAL(HXUIOBJ hObj, DWORD Color1, DWORD Color2, BOOL Gradient) {
		XUI_FILL_TYPE fillType = XUI_FILL_TYPE::XUI_FILL_SOLID;
		fillType = (Gradient) ? XUI_FILL_TYPE::XUI_FILL_RADIAL_GRADIENT : XUI_FILL_TYPE::XUI_FILL_SOLID;
		{
			D3DXVECTOR2 pvScalingFactor(1.0f, 1.0f), pTrnas(0.0f, 0.0f);
			XUIGradientStop gs[2];
			gs[0].dwColor = Color1;
			gs[0].fPos = 0.0f;
			gs[1].dwColor = Color2;
			gs[1].fPos = 1.0f;
			return Dashboard_Calls::XuiFigureSetFill(hObj, fillType, Color1, gs, 2, 0, &pvScalingFactor, &pTrnas);
		}
	}
	HRESULT SetFillLINEAR(HXUIOBJ hObj, DWORD Color3, DWORD Color4, BOOL Gradient) {
		XUI_FILL_TYPE fillType = XUI_FILL_TYPE::XUI_FILL_SOLID;
		fillType = (Gradient) ? XUI_FILL_TYPE::XUI_FILL_LINEAR_GRADIENT : XUI_FILL_TYPE::XUI_FILL_SOLID;
		D3DXVECTOR2 pvScalingFactor(1.0f, 1.0f), pTrnas(0.08f, 0.08f);//pTrans left to right & pvScalingFactor up/down
		XUIGradientStop gs[2];
		gs[0].dwColor = Color3;
		gs[0].fPos = 0.20f;//0.0f
		gs[1].dwColor = Color4;
		gs[1].fPos = 1.0f;//1.0
		return Dashboard_Calls::XuiFigureSetFill(hObj, fillType, Color3, gs, 2, 0, &pvScalingFactor, &pTrnas);
	}
	HRESULT SetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectSetProperty(Obj, propID, 0, propVal);

		return result;
	}

	HRESULT GetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectGetProperty(Obj, propID, 0, propVal);

		return result;
	}
}