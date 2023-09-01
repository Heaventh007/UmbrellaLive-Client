#include "stdafx.h"
DWORD HUD_UI::Tab_Cheats::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();
	this->GetChildById(L"lblDesc", &labeldesc);
	this->GetChildById(L"checkboxCheats", &checkboxCheats);
	this->GetChildById(L"checkboxAllCodsOffhost", &checkboxGTAV);
	this->GetChildById(L"btnSave", &btnSave); 
	btnSave.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#Smoke.png", xzp::XuiData, sizeof(xzp::XuiData)));
	checkboxCheats.SetCheck(INI::cheats);
	checkboxGTAV.SetCheck(INI::GTAV);
	this->GetChildById(L"lblDesc", &labeldesc);
	bHandled = TRUE;
	return ERROR_SUCCESS;
}
VOID halo3Cheats() {
	if (XamGetCurrentTitleId() == 0x4D5307E6) {
		//Aimbot
		byte bytes10[1] = { 0x58 };
		memcpy((PVOID)0x820C5CA3, bytes10, 1);
		memcpy((PVOID)0x820C5CAF, bytes10, 1);
		memcpy((PVOID)0x8224C7FF, bytes10, 1);
		memcpy((PVOID)0x8228A1AB, bytes10, 1);
		memcpy((PVOID)0x8228A1C7, bytes10, 1);
		memcpy((PVOID)0x8228A1D3, bytes10, 1);
		memcpy((PVOID)0x8228A1DF, bytes10, 1);
		memcpy((PVOID)0x8228A1F7, bytes10, 1);
		memcpy((PVOID)0x8228A22B, bytes10, 1);
		memcpy((PVOID)0x823814BF, bytes10, 1);
		memcpy((PVOID)0x823A32BB, bytes10, 1);
		memcpy((PVOID)0x823A6B47, bytes10, 1);
		memcpy((PVOID)0x8254FD9B, bytes10, 1);
		memcpy((PVOID)0x8254FDA7, bytes10, 1);
		memcpy((PVOID)0x8254FE77, bytes10, 1);
		memcpy((PVOID)0x8255F547, bytes10, 1);
		memcpy((PVOID)0x82563BE3, bytes10, 1);
		memcpy((PVOID)0x82563C43, bytes10, 1);
		//XNotifyUI(L"XBL - Loaded Aimbot Only");
	}
	else
	{
		//XNotifyUI(L"Load in Game, WTF u Doin?");
	}
}
VOID reachCheats() {
	if (XamGetCurrentTitleId() == 0x4D53085B) {
		byte buffer4[] = { 0x2 };
		//SetMemory((PVOID)0x820D49B3, buffer4, 1); //show coordinates
		byte buffer5[] = { 0xc3, 0xfd, 0x05, 0x58 };
		memcpy((PVOID)0x82419a54, buffer5, 4);//speed
		byte buffer6[] = { 0x20 };
		memcpy((PVOID)0x8213d85f, buffer6, 1);//jump
		byte buffer7[] = { 0xc1, 0x6b, 0x00, 0xa8 };
		memcpy((PVOID)0x823b9d60, buffer7, 4);//rapid fire
		*(int*)0x824dcd80 = 0xc01e030c; // Aimbot
		*(int*)0x824dcdb0 = 0xed090032; // Aimbot
		*(int*)0x824dcd7c = 0x3d608201; // Aimbot
		byte buffer2[] = { 0x21 };
		memcpy((PVOID)0x824fb884, buffer2, 1);//3rd Person
		//XNotifyUI(L" - Public Cheater Loaded!");
	}
	else
	{
		//XNotifyUI(L"Load in Game, WTF u Doin?");
	}
}
VOID BF4Cheats() {
	if (XamGetCurrentTitleId() == 0x454109BA) {
		*(int*)0x89167EF8 = 0x60000000; //Unlimited Breath
		*(int*)0x8910D704 = 0x409A000C; //Chams
		*(int*)0x88C06B54 = 0x48000040; //No Recoil
		*(int*)0x897C1C18 = 0x45990000; //Aimbot
		*(int*)0x897C1C1C = 0x47AFC800; //Aimbot
		*(int*)0x897C1C20 = 0x35719787; //Aimbot
		BYTE bytes777[] =
		{ 0x7D,0x88,0x02,0xA6,0x91,0x81,0x00,0x08,0x94,0x21,0xFF,0x90,0x7F,0x47,0xD3,0x78,0xFC,0x20,0xF0,0x90,0x7F,0x66,0xDB,0x78,0x7F,0xA4,0xEB,0x78,0x3D,0xC0,0x89,0x7C,0x61,0xCE,0x1C,0x10,0x90,0x6E,0x00,0x00,0x81,0xCE,0x00,0x00,0x81,0xCE,0x05,0x44,0x81,0xCE,0x00,0x00,0x81,0xCE,0x00,0x0C,0x3D,0xE0,0x89,0x7C,0x61,0xEF,0x1C,0x18,0x81,0xEF,0x00,0x00,0x91,0xEE,0x00,0x30,0x91,0xEE,0x00,0x34,0x91,0xEE,0x00,0x38,0x91,0xEE,0x00,0x40,0x91,0xEE,0x00,0x44,0x91,0xEE,0x00,0x48,0x3D,0xE0,0x89,0x7C,0x61,0xEF,0x1C,0x1C,0x81,0xEF,0x00,0x00,0x91,0xEE,0x00,0xF4,0x91,0xEE,0x00,0xF8,0x91,0xEE,0x00,0xFC,0x3D,0xE0,0x89,0x7C,0x61,0xEF,0x1C,0x20,0x81,0xEF,0x00,0x00,0x91,0xEE,0x00,0xD4,0x39,0xE0,0x00,0x00,0x39,0xC0,0x00,0x00,0x38,0x21,0x00,0x70,0x81,0x81,0x00,0x08,0x7D,0x88,0x03,0xA6,0x4E,0x80,0x00,0x20,0x00,0x00,0x00,0x00 };
		memcpy((void *)0x897C1A10, &bytes777, 160);//Aimbot ??
		BYTE bytes7777[] = { 0x3D,0xC0,0x89,0x7C,0x61,0xCE,0x1A,0x10,0x7D,0xC9,0x03,0xA6,0x4E,0x80,0x04,0x21 };
		memcpy((void *)0x89103870, &bytes7777, 16);
		Sleep(50);//Aimbot
		*(int*)0x88BE3500 = 0xED8701F2; //BULLET DAMAGE MAX
		*(int*)0x88BE3524 = 0x60000000; // BULLET DISTANCE NOP
		*(int*)0x8852C634 = 0x60000000;//BULLET DAMAGE MAX
		*(int*)0x8852C11C = 0x60000000;//BULLET DAMAGE MAX
		*(int*)0x886E7174 = 0x60000000;//BULLET DAMAGE MAX
		*(int*)0x886E6568 = 0x409A0028; // Enemy box head
		*(int*)0x886E6574 = 0x409A0014; // Enemy box head
		*(int*)0x886E71D4 = 0x39600001;
		*(int*)0x886E6440 = 0x3BA00004; // Team color
		*(int*)0x886E6508 = 0x3BC00008; // Enemy color
		Sleep(1000);
		//XNotifyUI(L" - BF4 Mods Enabled!");
		Sleep(5000);
	}
	else
	{
		//XNotifyUI(L"Load in Game, WTF u Doin?");
	}
}
VOID BF3Cheats() {
	if (XamGetCurrentTitleId() == 0x45410950) {//BF3
		*(int*)0x836FDAC8 = 0x60000000; // No Recoil
		*(int*)0x832291E0 = 0x60000000; // No Sway
		*(int*)0x83505268 = 0x60000000; // Name Tags
		*(int*)0x833E02A8 = 0x39600000; // Name Tags
		*(int*)0x835054C0 = 0x60000000; // Name Tags
		*(int*)0x835054FC = 0x4800003C; // Name Tags
		*(int*)0x831F7430 = 0x60000000; // Wall Hax
		*(int*)0x83505268 = 0x60000000; // Esp Hax
		*(int*)0x835054C0 = 0x60000000; // Esp Hax
		*(int*)0x835054FC = 0x4800003C; // Esp Hax
		*(int*)0x835056B0 = 0x4BD1FE59; // Esp Hax
		*(int*)0x83225508 = 0x7D8802A6; // Esp Hax
		*(int*)0x8322550C = 0x2F1C0002; // Esp Hax
		*(int*)0x83225510 = 0x41980010; // Esp Hax
		*(int*)0x83225514 = 0x3BE0005F; // Esp Hax
		*(int*)0x83225518 = 0x7D8803A6; // Esp Hax
		*(int*)0x8322551C = 0x4E000020; // Esp Hax
		*(int*)0x83225520 = 0x3BE00012; // Esp Hax
		*(int*)0x83225524 = 0x7D8803A6; // Esp Hax
		*(int*)0x83225528 = 0x4E000020; // Esp Hax
		*(int*)0x83504BB4 = 0x419A0010; // Esp Hax
		*(int*)0x836D7C60 = 0x60000000; // Max Damage
		*(int*)0x834FB424 = 0x39600001; // Enable UAV
		*(int*)0x834FB3D0 = 0x39600001; // Enable UAV
		*(int*)0x836D7C60 = 0x60000000; // 1 Hit Kill
		*(int*)0x836D7C34 = 0xED400032; // 1 Hit Kill
		*(int*)0x836D7C38 = 0xED200032; // 1 Hit Kill
		*(int*)0x83225534 = 0x7D8802A6; // Speed / Gravity Hax
		*(int*)0x83225538 = 0x2B0A0000; // Speed / Gravity Hax
		*(int*)0x8322553C = 0x419A0010; // Speed / Gravity Hax
		*(int*)0x83225540 = 0x8976011C; // Speed / Gravity Hax
		*(int*)0x83225544 = 0x7D8803A6; // Speed / Gravity Hax
		*(int*)0x83225548 = 0x4E000020; // Speed / Gravity Hax
		*(int*)0x8322554C = 0xC0090018; // Speed / Gravity Hax
		*(int*)0x83225550 = 0xC0290018; // Speed / Gravity Hax
		*(int*)0x83225554 = 0xEC210032; // Speed / Gravity Hax
		*(int*)0x83225558 = 0xEC210032; // Speed / Gravity Hax
		*(int*)0x8322555C = 0xEC210032; // Speed / Gravity Hax
		*(int*)0x83225560 = 0xEC210032; // Speed / Gravity Hax
		*(int*)0x83225564 = 0xD0290088; // Speed / Gravity Hax
		*(int*)0x83225568 = 0x100019C3; // Speed / Gravity Hax
		*(int*)0x83225570 = 0x100019C3; // Speed / Gravity Hax
		*(int*)0x83225574 = 0x8976011C; // Speed / Gravity Hax
		*(int*)0x83225578 = 0x7D8803A6; // Speed / Gravity Hax
		*(int*)0x8322557C = 0x4E000020; // Speed / Gravity Hax
		*(int*)0x832241B0 = 0xFC007890; // Speed / Gravity Hax
		*(int*)0x8328B1E8 = 0x4BF9A34D; // Speed / Gravity Hax
		*(int*)0x83504994 = 0x60000000; // Name Above Player
		*(int*)0x835049A8 = 0x60000000; // Name Above Player
		*(int*)0x835049B8 = 0x60000000; // Name Above Player 
		*(int*)0x83504938 = 0x60000000; // Name Above Player
		*(int*)0x83504944 = 0x60000000; // Name Above Player
		Sleep(1000);// No Freeze
		//XNotifyUI(L"XBLUmbrella - BF3 Mods Enabled!");
	}
	else
	{
		//XNotifyUI(L"Load in Game, WTF u Doin?");
	}
	return;
}
DWORD HUD_UI::Tab_Cheats::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	 if (hObjPressed == checkboxCheats)
		INI::cheats = checkboxCheats.IsChecked();
	else if (hObjPressed == checkboxGTAV)
		INI::GTAV = checkboxGTAV.IsChecked(); 
	else if (hObjPressed == btnSave) {
		INI::UpdateSettings();
		Utilities::NotifyPopup(L"XBLUmbrella - Settings saved and updated!");
	}
	bHandled = TRUE;
	return ERROR_SUCCESS;
}