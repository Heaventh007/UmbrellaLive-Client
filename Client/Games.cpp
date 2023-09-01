#include "stdafx.h"
DWORD nop = 0x60000000;
DWORD other = 0x38600001;
extern DWORD Title, Version; 
extern PWCHAR Module; 
extern BOOL bInitialized; 
//detour<VOID> LiveStats_Probation_GiveProbation;
DWORD WINAPI Games::B03::B03Start() {
	if (INI::cheats) {
		BO3_Start();
		XNotifyQueueUI(XNOTIFYUI_TYPE_GENERIC, 0, 2, L"XBLUmbrella - [B03] Engine Loaded!", 0);
		return ERROR_SUCCESS;
	}
	return E_FAIL;
}
DWORD WINAPI Games::BO2::B02Start() {
	if (INI::cheats) {
		BO2_Startload();
		*(int*)0x821f5b7c = 0x38600001; //Redboxes
		*(int*)0x82259BC8 = 0x38600001; //No Recoil
		*(int*)0x821B8FD0 = 0x2b110001; //UAV
		*(int*)0x821FC04C = 0x38c0ffff; //Wallhack
		*(int*)0x821B8FD3 = other;      //V-SAT<<new
		*(int*)0x826C6E6C = nop; //No Sway 
		*(int*)0x826BB50C = nop; //No Spread
		*(int*)0x82255E1C = 0x2b110001; //Laser
		*(int*)2181054320U = 2210767803U; //Show Host
		XNotifyQueueUI(XNOTIFYUI_TYPE_GENERIC, 0, 2, L"XBLUmbrella - [B02] Engine Loaded!", 0);
		return ERROR_SUCCESS;
	}
	return E_FAIL;
}
DWORD WINAPI Games::B01::B01Start() {
	if (INI::cheats) {
		BO1_Startload();
		XNotifyQueueUI(XNOTIFYUI_TYPE_GENERIC, 0, 2, L"XBLUmbrella - [B01] Engine Loaded!", 0);
		return ERROR_SUCCESS;
	}
	return E_FAIL;
}
DWORD WINAPI Games::Ghosts::GhostsStart() {
	if (INI::cheats) {
		GO_Startload();
		*(int*)0x826276D4 = 0x48000010;
		*(int*)0x826276CC = 0x38600000;
		*(int*)0x82627670 = 0x3920331C;
		BYTE ghosts[4] = { 0x38, 0xc0, 0, 5 };
		memcpy((void*)0x82290494, ghosts, 4);
		*(int*)0x8226D2B4 = nop; //RedBox
		*(int*)0x8227F198 = other; //Lazer
		*(int*)0x8226539B = other; //UAV1
		*(int*)0x82265793 = other; //UAV2
		*(int*)0x822657FF = other; //UAV3
		//XNotifyQueueUI(XNOTIFYUI_TYPE_GENERIC, 0, 2, L"XBLUmbrella - [Ghosts] Engine Loaded!", 0);
		return ERROR_SUCCESS;
	}
	return E_FAIL;
}
DWORD WINAPI Games::MW3::MW3Start() {
	if (INI::cheats) {
		MW3_StartLoad();
		XNotifyQueueUI(XNOTIFYUI_TYPE_GENERIC, 0, 2, L"XBLUmbrella - [MW3] Engine Loaded!", 0);
		return ERROR_SUCCESS;
	}
	return E_FAIL;
}
DWORD WINAPI Games::DestinyClass::DestinyBypass() {
	DestinyBypass();
	return ERROR_SUCCESS;
}
VOID AWCheats() {
	*(int*)0x82648CEC = nop; //no recoil
	*(int*)0x8262FB0C = nop; //Redbox
	*(int*)0x826352A4 = other; //UAV1
	*(int*)0x826352A8 = other; //UAV2
	*(int*)0x826352AC = other; //UAV3
	*(int*)0x826352B0 = other; //UAV4
	*(int*)0x824E84E4 = nop;//Run before game
	*(int*)0x82626F6C = nop;//Wepon Out Line
	*(int*)0x820659D8 = nop;//Croshairs
	*(int*)0x826476A0 = nop;//Croshairs2
	*(PDWORD)0x826476A0 = 0x60000000; // Crosshair 1   <---TU17||
	*(PDWORD)(0x826476A0 + 4) = 0x39600000; // Crosshair 2 <---TU17||
	*(PDWORD)0x82626F6C = 0x38C0000F; // Weapons outline <---TU17||
	*(int*)0x8262DF68 = other; //ESP1
	*(int*)0x8262E638 = other; //ESP2
	*(int*)0x826347C0 = other; //ESP3
	*(PDWORD)0x8260659C = 0x38C00012; // Players Body <---TU17|| Red
	Sleep(10000);
}