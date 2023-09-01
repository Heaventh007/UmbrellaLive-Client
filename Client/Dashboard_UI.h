#pragma once
#include "stdafx.h"

extern DWORD Color1;
extern DWORD Color2;
extern DWORD Color3;
extern DWORD Color4;
extern DWORD Color5;
extern DWORD Color6;
extern DWORD Color7;
extern DWORD Color8;

namespace Dashboard_UI {
	typedef HRESULT(*tXuiElementBeginRender)(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);
	extern tXuiElementBeginRender XuiElementBeginRenderStub;

	extern HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);

	extern VOID ResolveFunctions();
}