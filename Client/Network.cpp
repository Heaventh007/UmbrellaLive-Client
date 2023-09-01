#include "stdafx.h"
#include "IntegrityManager.h"
vector<pair<DWORD, DWORD>> hReadProtection;
vector<pair<DWORD, DWORD>> hReadProtectionWhitelist;
DWORD dwMyModuleSize = NULL;
CHAR KVSetDate[15];
char CurrentDate[25];
extern "C" {
	NTSYSAPI
		EXPORTNUM(315)
		VOID
		NTAPI
		XNetLogonGetExtendedStatus(
			OUT DWORD * pdwStatus,
			OUT DWORD * pdwStatusError
		);
}
BOOL banned() {
	DWORD x, Status;
	XNetLogonGetExtendedStatus(&x, &Status);
	return (Status == 0x8015190DL) ? TRUE : FALSE;
}
BOOL GetKVSetDate(string File)
{
	if (banned() == 1) return FALSE;
	struct stat Result;
	time_t Now = time(0);
	if (stat(File.c_str(), &Result) == 0)
	{
		strftime(KVSetDate, sizeof(KVSetDate), "%Y-%m-%d", localtime(&Result.st_ctime));
		strftime(CurrentDate, sizeof(CurrentDate), "%Y-%m-%d", localtime(&Now));
	}
	return TRUE;
}
BOOL Connect()
{
	XNetStartupParams XNSP;
	memset(&XNSP, NULL, sizeof(XNSP));
	XNSP.cfgSizeOfStruct = sizeof(XNetStartupParams);
	XNSP.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
	if (NetDll_XNetStartup(XNCALLER_SYSAPP, &XNSP) != NULL) return FALSE;
	if ((Globals::Socket = NetDll_socket(XNCALLER_SYSAPP, AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) return FALSE;
	BOOL SockOpt = TRUE;
	if (NetDll_setsockopt(XNCALLER_SYSAPP, Globals::Socket, SOL_SOCKET, SO_MARKINSECURE, (CONST PCHAR)&SockOpt, sizeof(BOOL)) != NULL) return FALSE;
	DWORD MaxPacketSize = 0x800;
	if (NetDll_setsockopt(XNCALLER_SYSAPP, Globals::Socket, SOL_SOCKET, SO_SNDBUF, (CONST PCHAR)&MaxPacketSize, sizeof(DWORD)) != NULL) return FALSE;
	if (NetDll_setsockopt(XNCALLER_SYSAPP, Globals::Socket, SOL_SOCKET, SO_RCVBUF, (CONST PCHAR)&MaxPacketSize, sizeof(DWORD)) != NULL) return FALSE;
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.S_un.S_addr = *(PDWORD)Globals::IPAddress;
	sockAddr.sin_port = htons(Globals::Port);
	if (NetDll_connect(XNCALLER_SYSAPP, Globals::Socket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) return FALSE, Globals::InitialConnected = FALSE;
	return Globals::Connected = TRUE, Globals::InitialConnected = TRUE;
}
VOID FreeSession() {
	if ((Globals::Socket != INVALID_SOCKET) && Globals::Connected) {
		NetDll_closesocket(XNCALLER_SYSAPP, Globals::Socket);
		Globals::Socket = INVALID_SOCKET;
		Globals::Connected = FALSE;
	}
}
BOOL SendServerCommands(DWORD Command, PVOID Buffer, DWORD Size) {
	memset(Globals::TempBuffer, NULL, sizeof(Globals::TempBuffer));
	memcpy(Globals::TempBuffer, &Command, sizeof(DWORD));
	memcpy(Globals::TempBuffer + 0x4, &Size, sizeof(DWORD));
	memcpy(Globals::TempBuffer + 0x8, Buffer, Size);
	DWORD Remaining = (Size + 0x8);
	PCHAR Current = (PCHAR)Globals::TempBuffer;
	while (Remaining > NULL) {
		DWORD SendSize = min(0x800, Remaining);
		DWORD Result = NetDll_send(XNCALLER_SYSAPP, Globals::Socket, Current, SendSize, NULL);
		if (Result == SOCKET_ERROR) return FALSE;
		Remaining -= Result;
		Current += Result;
	} return TRUE;
}
BOOL ReceiveData(PVOID Buffer, DWORD Size) {
	DWORD Received = NULL;
	DWORD Remaining = Size;
	while (Remaining > NULL) {
		DWORD RecvSize = min(0x800, Remaining);
		DWORD Result = NetDll_recv(XNCALLER_SYSAPP, Globals::Socket, (PCHAR)Buffer + Received, RecvSize, NULL);
		if ((Result == NULL) || (Result == SOCKET_ERROR)) return FALSE;
		Remaining -= Result;
		Received += Result;
	} return (Received == Size);
}
BOOL ProcessCommand(DWORD Command, PVOID Request, DWORD RequestSize, PVOID Response, DWORD ResponseSize, BOOL Close) {
	reConnect:
	if (!Connect()) {
		if (Globals::ConnectionAttempt <= 3) {
			Globals::ConnectionAttempt++;
			goto reConnect;
		} else {
			return FALSE;
		}
	} if (!SendServerCommands(Command, Request, RequestSize)) return FALSE;
	if (!ReceiveData(Response, ResponseSize)) return FALSE;
	if (Close) FreeSession();
	return TRUE;
}
HRESULT DoRedeemToken(const char *Token)
{
	BOOL DoneNotify = FALSE;
	CHECK_REQUEST Request; CHECK_RESPONSE Response; REDEEM_RESPONSE Response1;
	memcpy(Request.Token, Token, 0x0E);
	memcpy(Request.CPUKey, Utilities::GetCPUKey(), 0x10);
	if (ProcessCommand(COMMAND_RED_TOKEN, &Request, sizeof(REDEEM_REQUEST), &Response, sizeof(REDEEM_RESPONSE), FALSE)) {
		if (Response1.Status == 0x8E000000) {
			ServerPresence();
			Globals::ExpiredNotify = FALSE;
		}
		else {
			Utilities::NotifyPopup(L"ProdgiyLive - Fatal Error");
		}
	} FreeSession();
	XPhysicalFree(&Request);
	XPhysicalFree(&Response);
}
HRESULT DoTokenCheck(const char *Token, DWORD Confirm) {
	XOVERLAPPED Overlapped; MESSAGEBOX_RESULT Result;
	CHECK_REQUEST* Request = (CHECK_REQUEST*)XPhysicalAlloc(sizeof(CHECK_REQUEST), MAXULONG_PTR, 0, PAGE_READWRITE);
	memset(Request, 0, sizeof(CHECK_REQUEST));
	CHECK_RESPONSE* Response = (CHECK_RESPONSE*)XPhysicalAlloc(sizeof(CHECK_RESPONSE), MAXULONG_PTR, 0, PAGE_READWRITE);
	memset(Response, 0, sizeof(CHECK_RESPONSE));
	memcpy(Request->CPUKey, Utilities::GetCPUKey(), 0x10);
	memcpy(Request->Token, Token, 0x0E);
	BOOL ValidResponse = FALSE;
	std::string DaysStr;
	if (ProcessCommand(COMMAND_CHECK_TOKEN, Request, sizeof(CHECK_REQUEST), Response, sizeof(CHECK_RESPONSE), FALSE))
	{
		if (Response->Days == 0) {
			DaysStr = "1";
			ValidResponse = TRUE;
		}
		else if (Response->Days == 1) {
			DaysStr = "7";
			ValidResponse = TRUE;
		}
		else if (Response->Days == 2) {
			DaysStr = "31";
			ValidResponse = TRUE;
		}
		else if (Response->Days == 3) {
			DaysStr = "Unlimited Access";// 3600 Days
			ValidResponse = TRUE;
		}
		else
		{
			ValidResponse = FALSE;
		}
	}
	if (ValidResponse)
	{
		LPCWSTR wszButtons[2] = { L"Redeem Token", L"Close" };
		ZeroMemory(&Overlapped, sizeof(XOVERLAPPED)); ZeroMemory(&Result, sizeof(MESSAGEBOX_RESULT));
		while (((DWORD(*)(...))XShowMessageBoxUI)(255, L"XBLUmbrella - Token Valid", L"This Token Is Valid And Once Redeemed Will Be Immediately Added To Your Interval Days.", 2, wszButtons, TRUE, 4, &Result, &Overlapped) == ERROR_ACCESS_DENIED) Sleep(500);
		while (!XHasOverlappedIoCompleted(&Overlapped)) Sleep(500);
		switch (Result.dwButtonPressed)
		{
		case 0: return DoRedeemToken(Token); break;
		case 1: Utilities::NotifyPopup(L"XBLUmbrella Responce Error"); break;
		}
		return ERROR_SUCCESS;
	}
	FreeSession();
	XPhysicalFree(Request);
	XPhysicalFree(Response);
}
void InitiateTokenSystem()
{
	XOVERLAPPED XOverlapped; WCHAR wchInput[15];
	while (((DWORD(*)(...))XShowKeyboardUI)(0, VKBD_DEFAULT, L"", L"XBLUmbrella - Token System", L"Enter your 12 digit token here.\nEX. AAAA-BBBB-CCCC", wchInput, 15, &XOverlapped) == ERROR_ACCESS_DENIED) Sleep(500);
	while (!XHasOverlappedIoCompleted(&XOverlapped)) Sleep(500);
	if (wcslen(wchInput) == 14)
	{
		Sleep(1000);
		std::string wstr = Utilities::utf8_encode(wchInput);
		if (!DoTokenCheck(wstr.c_str(), 0) != ERROR_SUCCESS)
		{
			//Utilities::NotifyPopup(L"XBLUmbrella - Error");
		}
	}
}
VOID Update(BOOL Reboot)
{
	DWORD ModuleSize = 0;
	if (!ReceiveData(&ModuleSize, sizeof(DWORD)))
	{
		FreeSession();
		return;
	}

	PBYTE ModuleBuffer = (PBYTE)XPhysicalAlloc(ModuleSize, MAXULONG_PTR, 0, PAGE_READWRITE);
	memset(ModuleBuffer, 0, ModuleSize);
	if (!ReceiveData(ModuleBuffer, ModuleSize))
	{
		XPhysicalFree(ModuleBuffer);
		FreeSession();
		return;
	}

	Utilities::XNotifyUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, L"Updating Server XeX");
	if (!Utilities::writeFile(Globals::pathXeX, ModuleBuffer, ModuleSize))
	{
		XPhysicalFree(ModuleBuffer);
		FreeSession();
		return;
	}

	XPhysicalFree(ModuleBuffer);
	Sleep(10000);

	if (Reboot)
	{
		Utilities::XNotifyUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, L"Rebooting!!!!");
		HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}
}
HRESULT engine() {
	const size_t menu_size = 19314;
	Sleep(2000);
	if (Utilities::CWriteFile(Globals::customxex, xzp::XuiData, menu_size) == TRUE) {
		Sleep(500);
	}
	return ERROR_SUCCESS;
}
VOID InitializeConnection() {
	INIT_REQUEST* Request = (INIT_REQUEST*)XPhysicalAlloc(sizeof(INIT_REQUEST), MAXULONG_PTR, NULL, PAGE_READWRITE);
	memset(Request, 0, sizeof(INIT_REQUEST));
	INIT_RESPONSE* Response = (INIT_RESPONSE*)XPhysicalAlloc(sizeof(INIT_RESPONSE), MAXULONG_PTR, NULL, PAGE_READWRITE);
	memset(Response, 0, sizeof(INIT_RESPONSE));
	Request->Version = 0;
	memcpy(Request->CPUKey, Utilities::GetCPUKey(), 0x10);
	memcpy(&Request->KV, Utilities::GetKeyVault(), sizeof(KEY_VAULT));
	Utilities::MemoryBuffer mbModule;
	if (!Utilities::CReadFile(Globals::pathXeX, mbModule)) HalReturnToFirmware(HalRebootQuiesceRoutine);
	XeCryptSha(mbModule.GetBuffer(), mbModule.GetLength(), 0, 0, 0, 0, Request->ModuleDigest, 0x14);
	if (Globals::Initialized = ProcessCommand(COMMAND_INIT, Request, sizeof(INIT_REQUEST), Response, sizeof(INIT_RESPONSE), FALSE)) {
		if (Response->Status == 0x9F000000) {
			Update(TRUE);
		}
		else if (Globals::Authenticated = (Response->Status == 0x5A000000 || Response->Status == 0x7A000000)) {
			Utilities::SetLiveBlock(FALSE);
			/*Title Menu Fix*/
			*(DWORD*)0x816FEE9C = 0x38600000;
			remove("SysCache0:\TP.0000000000000");
			remove("SysCache1:\TP.0000000000000");
			if (!ReceiveData(Globals::PATCH_XAM, 0x58)) HalReturnToFirmware(HalRebootQuiesceRoutine);
			if (!ReceiveData(xzp::XuiData, sizeof(xzp::XuiData)));
			//HUDHooks::RefreshUIText_Hook();
			Utilities::ApplyPatches(Globals::PATCH_XAM);	
			//if (!ReceiveData(xzp::GTAVData, sizeof(xzp::GTAVData)));
			//engine();
			Utilities::MakeThread((LPTHREAD_START_ROUTINE)ServerPresence);
		}
		else HalReturnToFirmware(HalRebootQuiesceRoutine);
	}
	FreeSession();
	XPhysicalFree(Request);
	XPhysicalFree(Response);
}
VOID ServerPresence() {
	for (;; Sleep(30000)) {
		PRES_REQUEST* Request = (PRES_REQUEST*)XPhysicalAlloc(sizeof(PRES_REQUEST), MAXULONG_PTR, 0, PAGE_READWRITE);
		memset(Request, 0, sizeof(PRES_REQUEST));
		PRES_RESPONSE* Response = (PRES_RESPONSE*)XPhysicalAlloc(sizeof(PRES_RESPONSE), MAXULONG_PTR, 0, PAGE_READWRITE);
		memset(Response, 0, sizeof(PRES_RESPONSE));
		Request->TitleID = XamGetCurrentTitleId();
		memcpy(Request->CPUKey, Utilities::GetCPUKey(), 0x10);
		Utilities::MemoryBuffer mbModule;
		if (!Utilities::CReadFile(Globals::pathXeX, mbModule)) HalReturnToFirmware(HalRebootQuiesceRoutine);
		XeCryptSha(mbModule.GetBuffer(), mbModule.GetLength(), 0, 0, 0, 0, Request->ModuleDigest, 0x14);
		XUSER_SIGNIN_INFO XUserInfo;
		if (XUserGetSigninInfo(0, 0, &XUserInfo) == S_OK) memcpy(Request->Gamertag, XUserInfo.szUserName, 0x10);
		if (ProcessCommand(COMMAND_PRES, Request, sizeof(PRES_REQUEST), Response, sizeof(PRES_RESPONSE), FALSE)) {
			if (Response->Status == 0x9F000000) {
				//Utilities::NotifyPopup(L"XBLUmbrella - Updating XEX Image ...");
				//Update(TRUE);
			} 
			else {
				if (Response->Status == 0x5A000000 || Response->Status == 0x7A000000) { Utilities::SetLiveBlock(FALSE); }
				else if (Response->Status == 0x64000000) {
					Utilities::SetLiveBlock(TRUE);
					if (!Globals::ExpiredNotify && Globals::DashLoaded) { 
						Sleep(5000);
						Utilities::NotifyPopup(L"XBLUmbrella - Time Expired");
						Globals::ExpiredNotify = TRUE;
					}
				}
				else if (Response->Status == 0x65000000) {
					Utilities::SetLiveBlock(FALSE);
					if (!Globals::DayExpiredNotify && Globals::DashLoaded) {
						Sleep(5000);
						Globals::DayExpiredNotify = TRUE;
					}
				}
				else if (Response->Status == 0x66000000) {
					Utilities::SetLiveBlock(FALSE);
					Sleep(5000);
					ServerPresence();
				}
				Globals::Days = Response->TimeRemaining[0], Globals::Hours = Response->TimeRemaining[1], Globals::Minutes = Response->TimeRemaining[2];
				Globals::ReserveDays = Response->ReserveDays;
				Globals::DaysUnbanned = Response->TimeUnbanned[0], Globals::HoursUnbanned = Response->TimeUnbanned[1], Globals::MinutesUnbanned = Response->TimeUnbanned[2];
				Globals::Status = Response->Status;
			}
		}
		FreeSession();
		XPhysicalFree(Request);
		XPhysicalFree(Response);
	}
}
void AddRegionToWhitelist(DWORD start, DWORD length) {
	hReadProtectionWhitelist.push_back(make_pair(start, length));
}
typedef bool(*t_MmDbgReadCheckHook)(DWORD dwAddress);
t_MmDbgReadCheckHook MmDbgReadCheckHookStub;
bool MmDbgReadCheckHook(DWORD dwAddress) {
	DWORD Ret = MmDbgReadCheckHookStub(dwAddress);
	if (hReadProtectionWhitelist.size() > 0) {
		for (auto i = 0; i < hReadProtectionWhitelist.size(); i++) {
			if (dwAddress >= hReadProtectionWhitelist.at(i).first && dwAddress <= (hReadProtectionWhitelist.at(i).first + hReadProtectionWhitelist.at(i).second))
				return MmDbgReadCheckHookStub(dwAddress);
		}
	}
	if (hReadProtection.size() > 0) {
		for (auto i = 0; i < hReadProtection.size(); i++) {
			if (dwAddress >= hReadProtection.at(i).first && dwAddress <= (hReadProtection.at(i).first + hReadProtection.at(i).second))
				return FALSE;
		}
	}
	return Ret;
}
VOID ProtectMyMemorySpace() {
	PLDR_DATA_TABLE_ENTRY m_XexTableEntry = nullptr;
	XexPcToFileHeader(reinterpret_cast<void*>(BASEADDY), &m_XexTableEntry);
	dwMyModuleSize = m_XexTableEntry->SizeOfFullImage;
	if (m_XexTableEntry != nullptr) {
		hReadProtection.emplace_back(make_pair(BASEADDY, m_XexTableEntry->SizeOfFullImage));
		MmDbgReadCheckHookStub = reinterpret_cast<t_MmDbgReadCheckHook>(Utilities::HookFunctionStub(MODULE_KERNEL, 0x1AB, MmDbgReadCheckHook));
		GetIntegrityManager()->Add(reinterpret_cast<void*>(MmDbgReadCheckHook), Utilities::GetFunctionSize(reinterpret_cast<DWORD*>(MmDbgReadCheckHook)));
	}
}