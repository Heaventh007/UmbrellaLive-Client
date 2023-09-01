#pragma once
#include "stdafx.h"

namespace Bypasses
{
	extern NTSTATUS GhostsXeKeysGetKeyHook(WORD KeyId, PVOID KeyBuffer, PDWORD keyLength);
	extern unsigned long XeXGetModuleHandleHook(char* ModuleName);
	extern DWORD XeKeysGetKeyHook(WORD KeyId, PVOID KeyBuffer, PDWORD KeyLength);
}
