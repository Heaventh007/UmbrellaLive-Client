#include "stdafx.h"
using namespace std;
#pragma warning(disable:4826 4800)
BOOL ChecksumHDD;
char m_hookSection[0x500];
int m_hookCount;
namespace Utilities {
	PWCHAR LinkWChar(PWCHAR Text, ...) {
		static WCHAR Buffer[0x1000], MessageBuffer[0x100];;

		va_list pArgList;
		va_start(pArgList, Text);
		vswprintf(Buffer, Text, pArgList);
		va_end(pArgList);

		swprintf(MessageBuffer, Buffer);
		return MessageBuffer;
	}
	DWORD Resolve(HMODULE Module, int ordinal) {
		auto ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(Module);
		auto header = reinterpret_cast<_IMAGE_XEX_HEADER*>(ldr->XexHeaderBase);
		if (header) {
			auto sec = reinterpret_cast<PHV_IMAGE_INFO>(header->SecurityInfo);
			if (sec) {
				auto exports = reinterpret_cast<PIMAGE_EXPORT_ADDRESS_TABLE>(sec->ExportTableAddress);
				if (exports)
					return exports->ordOffset[ordinal - 1] + (exports->ImageBaseAddress << 16);
			}
		}
		return ERROR_SUCCESS;
	}
	wchar_t *convertCharArrayToLPCWSTR(const char* charArray) {
		wchar_t* wString = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
		return wString;
	}
	BOOL TrayOpen()
	{
		BYTE Input[0x10] = { 0 }, Output[0x10] = { 0 };
		Input[0] = 0xA;
		HalSendSMCMessage(Input, Output);
		return (Output[1] == 0x60);
	}
	BOOL FileExists(CONST PCHAR FilePath)
	{
		if (GetFileAttributes(FilePath) == INVALID_FILE_ATTRIBUTES)
		{
			DWORD LastError = GetLastError();
			if ((LastError == ERROR_PATH_NOT_FOUND) || (LastError == ERROR_FILE_NOT_FOUND)) return FALSE;
		}
		return TRUE;
	}
	BOOL pfShow = (BOOL)0xDEADBEEF; BOOL pfShowMovie;
	BOOL pfPlaySound; BOOL pfShowIPTV;
	VOID toggleNotify(BOOL on) {
		if ((int)pfShow == 0xDEADBEEF)
			XNotifyUIGetOptions(&pfShow, &pfShowMovie, &pfPlaySound, &pfShowIPTV);
		if (!on) {
			XNotifyUISetOptions(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		}
		else {
			XNotifyUISetOptions(true, true, true, true);
		} Sleep(500);
	}
	VOID XNotifyDoQueueUI(PWCHAR pwszStringParam) {
		toggleNotify(true);
		XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, XUSER_INDEX_ANY, XNOTIFYUI_PRIORITY_HIGH, pwszStringParam, NULL);
		toggleNotify(false);
	}
	VOID NotifyPopup(PWCHAR myPWCHAR) {
		if (KeGetCurrentProcessType() != PROC_USER) {
			HANDLE th = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)XNotifyDoQueueUI, (LPVOID)myPWCHAR, CREATE_SUSPENDED, NULL);
			if (th == NULL) return; ResumeThread(th);
		}
		else XNotifyDoQueueUI(myPWCHAR);
	}
	BOOL CReadFile(CONST PCHAR FilePath, MemoryBuffer &Buffer)
	{
		HANDLE Handle = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (Handle == INVALID_HANDLE_VALUE) return FALSE;
		DWORD FileSize = GetFileSize(Handle, 0);
		PBYTE FileBuffer = (PBYTE)malloc(FileSize);
		if (FileBuffer == 0)
		{
			CloseHandle(Handle);
			return FALSE;
		}
		DWORD ReadSize = 0;
		if (!ReadFile(Handle, FileBuffer, FileSize, &ReadSize, 0))
		{
			free(FileBuffer);
			CloseHandle(Handle);
			return FALSE;
		}
		else if (ReadSize != FileSize)
		{
			free(FileBuffer);
			CloseHandle(Handle);
			return FALSE;
		}
		Buffer.Add(FileBuffer, FileSize);
		free(FileBuffer);
		CloseHandle(Handle);
		return TRUE;
	}
	BOOL writeFile(const CHAR* FilePath, const VOID* Data, DWORD Size)
	{
		// Open our file
		HANDLE fHandle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fHandle == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		// Write our data and close
		DWORD writeSize = Size;
		if (WriteFile(fHandle, Data, writeSize, &writeSize, NULL) != TRUE)
		{
			return FALSE;
		}

		CloseHandle(fHandle);
		return TRUE;
	}
	BOOL CWriteFile(CONST PCHAR FilePath, CONST PVOID Buffer, DWORD Size)
	{
		DWORD WriteSize = Size;
		HANDLE Handle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (Handle == INVALID_HANDLE_VALUE) return FALSE;
		if (!WriteFile(Handle, Buffer, WriteSize, &WriteSize, 0))
		{
			CloseHandle(Handle);
			return FALSE;
		}
		CloseHandle(Handle);
		return (WriteSize == Size);
	}
#pragma warning(push)
	BOOL InitializeHvxPeekPoke()
	{
		PVOID pBuffer = XPhysicalAlloc(0x1000, MAXULONG_PTR, 0, PAGE_READWRITE);
		DWORD Address = (DWORD)MmGetPhysicalAddress(pBuffer);
		memset(pBuffer, 0, 0x1000);
		memcpy(pBuffer, HvxPeekPokeExp, sizeof(HvxPeekPokeExp));
		DWORD Result = (DWORD)HvxExpansionInstall(Address, 0x1000);
		XPhysicalFree(pBuffer);
		return (Result == S_OK);
	}
	BYTE HvxPeekBYTE(QWORD Address) { return (BYTE)HvxExpansionCall(HvxPeekPokeExpID, PEEK_BYTE, Address); }
	WORD HvxPeekWORD(QWORD Address) { return (WORD)HvxExpansionCall(HvxPeekPokeExpID, PEEK_WORD, Address); }
	DWORD HvxPeekDWORD(QWORD Address) { return (DWORD)HvxExpansionCall(HvxPeekPokeExpID, PEEK_DWORD, Address); }
	QWORD HvxPeekQWORD(QWORD Address) { return HvxExpansionCall(HvxPeekPokeExpID, PEEK_QWORD, Address); }
	DWORD HvxPeekBytes(QWORD Address, PVOID Buffer, DWORD Size)
	{
		PVOID pBuffer = XPhysicalAlloc(Size, MAXULONG_PTR, 0, PAGE_READWRITE);
		memset(pBuffer, 0, Size);
		DWORD Result = (DWORD)HvxExpansionCall(HvxPeekPokeExpID, PEEK_BYTES, Address, (QWORD)MmGetPhysicalAddress(pBuffer), Size);
		if (Result == S_OK) memcpy(Buffer, pBuffer, Size);
		XPhysicalFree(pBuffer);
		return Result;
	}
	DWORD HvxPokeBYTE(QWORD Address, BYTE Value) { return (DWORD)HvxExpansionCall(HvxPeekPokeExpID, POKE_BYTE, Address, Value); }
	DWORD HvxPokeWORD(QWORD Address, WORD Value) { return (DWORD)HvxExpansionCall(HvxPeekPokeExpID, POKE_WORD, Address, Value); }
	DWORD HvxPokeDWORD(QWORD Address, DWORD Value) { return (DWORD)HvxExpansionCall(HvxPeekPokeExpID, POKE_DWORD, Address, Value); }
	DWORD HvxPokeQWORD(QWORD Address, QWORD Value) { return (DWORD)HvxExpansionCall(HvxPeekPokeExpID, POKE_QWORD, Address, Value); }
	DWORD HvxPokeBytes(QWORD Address, CONST PVOID Buffer, DWORD Size)
	{
		PVOID pBuffer = XPhysicalAlloc(Size, MAXULONG_PTR, 0, PAGE_READWRITE);
		memcpy(pBuffer, Buffer, Size);
		DWORD Result = (DWORD)HvxExpansionCall(HvxPeekPokeExpID, POKE_BYTES, Address, (QWORD)MmGetPhysicalAddress(pBuffer), Size);
		XPhysicalFree(pBuffer);
		return Result;
	}
#pragma warning(pop)
	VOID ApplyPatches(CONST PVOID Buffer)
	{
		PDWORD PatchData = (PDWORD)Buffer;
		while (*PatchData != 0xFFFFFFFF)
		{
			memcpy((PVOID)PatchData[0], &PatchData[2], (PatchData[1] * sizeof(DWORD)));
			PatchData += (PatchData[1] + 2);
		}
	}
	VOID ApplyPatches(QWORD Address, CONST PVOID Buffer)
	{
		PDWORD PatchData = (PDWORD)Buffer;
		while (*PatchData != 0xFFFFFFFF)
		{
			QWORD _Address = Address;
			if (PatchData[0] >= 0x10000 && PatchData[0] < 0x20000) _Address |= 0x200000000;
			if (PatchData[0] >= 0x20000 && PatchData[0] < 0x30000) _Address |= 0x400000000;
			if (PatchData[0] >= 0x30000 && PatchData[0] < 0x40000) _Address |= 0x600000000;
			_Address |= PatchData[0];
			Utilities::HvxPokeBytes(_Address, &PatchData[2], (PatchData[1] * sizeof(DWORD)));
			PatchData += (PatchData[1] + 2);
		}
	}
	DWORD ResolveFunction(PCHAR Module, DWORD Ordinal)
	{
		HANDLE Handle = 0;
		if (NT_SUCCESS(XexGetModuleHandle(Module, &Handle)))
		{
			DWORD Address = 0;
			if (NT_SUCCESS(XexGetProcedureAddress(Handle, Ordinal, &Address))) return (Address != 0) ? Address : 0;
		}
		return 0;
	}
	FARPROC ResolveFunction(HMODULE hHandle, DWORD Ordinal) {
		return (hHandle == NULL) ? NULL : GetProcAddress(hHandle, (LPCSTR)Ordinal);
	}
	VOID PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked)
	{
		if (Destination & 0x8000) Address[0] = (0x3D600000 + (((Destination >> 16) & 0xFFFF) + 1));
		else Address[0] = (0x3D600000 + ((Destination >> 16) & 0xFFFF));
		Address[1] = (0x396B0000 + (Destination & 0xFFFF));
		Address[2] = 0x7D6903A6;
		Address[3] = (0x4E800420 | Linked);
		doSync(Address);
	}
	VOID __declspec(naked) GPLR()
	{
		__asm
		{
			std r14, -0x98(r1)
			std r15, -0x90(r1)
			std r16, -0x88(r1)
			std r17, -0x80(r1)
			std r18, -0x78(r1)
			std r19, -0x70(r1)
			std r20, -0x68(r1)
			std r21, -0x60(r1)
			std r22, -0x58(r1)
			std r23, -0x50(r1)
			std r24, -0x48(r1)
			std r25, -0x40(r1)
			std r26, -0x38(r1)
			std r27, -0x30(r1)
			std r28, -0x28(r1)
			std r29, -0x20(r1)
			std r30, -0x18(r1)
			std r31, -0x10(r1)
			stw r12, -0x8(r1)
			blr
		}
	}
	DWORD RelinkGPLR(DWORD Offset, PDWORD SaveStub, PDWORD Original)
	{
		DWORD Instruction = 0, Replacing = 0;
		PDWORD Saver = (PDWORD)GPLR;
		if (Offset & 0x2000000) Offset |= 0xFC000000;
		Replacing = Original[Offset / 4];
		for (DWORD i = 0; i < 0x14; i++)
		{
			if (Replacing == Saver[i])
			{
				DWORD NewOffset = ((DWORD)&Saver[i] - (DWORD)SaveStub);
				Instruction = (0x48000001 | (NewOffset & 0x03FFFFFC));
			}
		}
		return Instruction;
	}
	VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination)
	{
		if ((Address != 0) && (SaveStub != 0))
		{
			DWORD Relocation = (DWORD)&Address[4];
			if (Relocation & 0x8000) SaveStub[0] = (0x3D600000 + (((Relocation >> 16) & 0xFFFF) + 1));
			else SaveStub[0] = (0x3D600000 + ((Relocation >> 16) & 0xFFFF));
			SaveStub[1] = (0x396B0000 + (Relocation & 0xFFFF));
			SaveStub[2] = 0x7D6903A6;
			for (DWORD i = 0; i < 4; i++)
			{
				if ((Address[i] & 0x48000003) == 0x48000001) SaveStub[i + 3] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 3], &Address[i]);
				else SaveStub[i + 3] = Address[i];
			}
			SaveStub[7] = 0x4E800420;
			doSync(SaveStub);
			PatchInJump(Address, Destination, FALSE);
		}
	}
	DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR Import, DWORD Ordinal, DWORD Destination) {
		DWORD OrdinalAddress = ResolveFunction(Import, Ordinal);
		if (OrdinalAddress == 0) return S_FALSE;
		PXEX_IMPORT_DESCRIPTOR ImportDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(Module->XexHeaderBase, XEX_HEADER_IMPORTS);
		if (ImportDesc == 0) return S_FALSE;
		PXEX_IMPORT_TABLE ImportTable = (PXEX_IMPORT_TABLE)((PBYTE)ImportDesc + sizeof(*ImportDesc) + ImportDesc->NameTableSize);
		for (DWORD i = 0; i < ImportDesc->ModuleCount; i++) {
			for (WORD j = 0; j < ImportTable->ImportCount; j++) {
				DWORD StubAddress = *((PDWORD)ImportTable->ImportStubAddr[j]);
				if (OrdinalAddress != StubAddress) continue;
				StubAddress = (DWORD)ImportTable->ImportStubAddr[j + 1];
				PatchInJump((PDWORD)StubAddress, Destination, FALSE);
				j = ImportTable->ImportCount;
			}
			ImportTable = (PXEX_IMPORT_TABLE)((PBYTE)ImportTable + ImportTable->TableSize);
		}
		return S_OK;
	}
	DWORD PatchModuleImport(PCHAR Module, PCHAR Import, DWORD Ordinal, DWORD Destination)
	{
		PLDR_DATA_TABLE_ENTRY Handle = (PLDR_DATA_TABLE_ENTRY)GetModuleHandle(Module);
		return (Handle != 0) ? PatchModuleImport(Handle, Import, Ordinal, Destination) : S_FALSE;
	}
	DWORD MountPath(PCHAR Drive, PCHAR Device)
	{
		CHAR Destination[MAX_PATH] = { 0 };
		sprintf_s(Destination, MAX_PATH, (KeGetCurrentProcessType() == PROC_SYSTEM) ? OBJ_SYS_STRING : OBJ_USR_STRING, Drive);
		ANSI_STRING LinkName, DeviceName;
		RtlInitAnsiString(&LinkName, Destination);
		RtlInitAnsiString(&DeviceName, Device);
		ObDeleteSymbolicLink(&LinkName);
		return (DWORD)ObCreateSymbolicLink(&LinkName, &DeviceName);
	}
	VOID MakeThread(LPTHREAD_START_ROUTINE Address) {
		HANDLE Handle = 0;
		ExCreateThread(&Handle, 0, 0, (PVOID)XapiThreadStartup, Address, 0, (EX_CREATE_FLAG_SUSPENDED | EX_CREATE_FLAG_SYSTEM | 0x18000424));
		XSetThreadProcessor(Handle, 0x4);
		SetThreadPriority(Handle, THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread(Handle);
		CloseHandle(Handle);
	}
	VOID SetLiveBlock(BOOL State) {
		DWORD Address = ResolveFunction(MODULE_LAUNCH, DL_ORDINALS_SETOPTVALBYNAME);
		((VOID(*)(...))Address)("liveblock", (PDWORD)&State);
		((VOID(*)(...))Address)("livestrong", (PDWORD)&State);
	}
	BOOL FindPaths() {
		if ((XboxHardwareInfo->Flags & 0x20) == DM_XBOX_HW_FLAG_HDD) {
			if (MountPath("XBLUmbrella:\\", "\\Device\\Harddisk0\\Partition1") != S_OK) return FALSE;
			Globals::pathKeyVault = KeyVaultPath;
			Globals::pathCPUKey = CPUKeyPath;
			Globals::pathXeX = ModulePath;
			Globals::PATH_NewXEX = "XBLUmbrella:\\XBLUmbrella.xex";

			//Color UI
			Globals::PATH_rainbow = "XBLUmbrella:\\XBLUmbrella.ini";
			Globals::PATH_linear = "XBLUmbrella:\\XBLUmbrella.ini";
			Globals::PATH_radial = "XBLUmbrella:\\XBLUmbrella.ini";
			Globals::PATH_EverythingOff = "XBLUmbrella:\\XBLUmbrella.ini";
		}
		else {
			if (MountPath("XBLUmbrella:\\", "\\Device\\Mass0") != S_OK) return FALSE;
			Globals::pathKeyVault = KeyVaultPath;
			Globals::pathCPUKey = CPUKeyPath;
			Globals::pathXeX = ModulePath; 
			Globals::customxex = resourcePath;
			Globals::PATH_NewXEX = "XBLUmbrella:\\XBLUmbrella.xex";

			//Color UI
			Globals::PATH_rainbow = "XBLUmbrella:\\XBLUmbrella.ini";
			Globals::PATH_linear = "XBLUmbrella:\\XBLUmbrella.ini";
			Globals::PATH_radial = "XBLUmbrella:\\XBLUmbrella.ini";
			Globals::PATH_EverythingOff = "XBLUmbrella:\\XBLUmbrella.ini";
		} return TRUE;
	}
	PBYTE GetCPUKey()
	{
		QWORD Fuses[2] = { (HvxPeekQWORD(0x8000020000020600) | HvxPeekQWORD(0x8000020000020800)), (HvxPeekQWORD(0x8000020000020A00) | HvxPeekQWORD(0x8000020000020C00)) };
		memcpy(Globals::CPUKey, Fuses, 0x10);
		return Globals::CPUKey;
	}
	PBYTE GetKeyVault()
	{
		QWORD KVAddress = HvxPeekQWORD(0x00000002000163C0);
		if (HvxPeekBytes(KVAddress, &Globals::KV, sizeof(KEY_VAULT)) != S_OK) return 0;
		return (PBYTE)&Globals::KV;
	}
	BOOL SetKeyVault()
	{
		MemoryBuffer mbKV, mbCPU;
		BOOL NoKVbin = !CReadFile(Globals::pathKeyVault, mbKV);
		if (FileExists(Globals::pathCPUKey))
			if (!CReadFile(Globals::pathCPUKey, mbCPU)) return FALSE;
		memcpy(Globals::KVCPUKey, (mbCPU.GetLength() == 0x10) ? mbCPU.GetBuffer() : GetCPUKey(), 0x10);
		XeCryptSha(Globals::KVCPUKey, 0x10, 0, 0, 0, 0, Globals::CPUKeyDigest, 0x14);
		memcpy((PVOID)0x8E03AA30, Globals::CPUKeyDigest, 0x10);
		memcpy(&Globals::KV, NoKVbin ? GetKeyVault() : mbKV.GetBuffer(), sizeof(KEY_VAULT));
		for (int i = 0; i < sizeof(Globals::KV.KeyVaultSignature); ++i)
		{
			if (Globals::KV.KeyVaultSignature[i] != 0)
				Globals::kvtype = FALSE;
		}
		XECRYPT_HMACSHA_STATE HMACSHA;
		XeCryptHmacShaInit(&HMACSHA, Globals::CPUKey, 0x10);
		XeCryptHmacShaUpdate(&HMACSHA, (PBYTE)&Globals::KV.OddFeatures, 0xD4);
		XeCryptHmacShaUpdate(&HMACSHA, (PBYTE)&Globals::KV.DvdKey, 0x1CF8);
		XeCryptHmacShaUpdate(&HMACSHA, (PBYTE)&Globals::KV.CardeaCertificate, 0x2108);
		XeCryptHmacShaFinal(&HMACSHA, Globals::KVDigest, 0x14);
		memcpy((PVOID)0x8E03AA40, Globals::KVDigest, 0x14);
		Globals::Fcrt = ((Globals::KV.OddFeatures & ODD_POLICY_FLAG_CHECK_FIRMWARE) != 0);
		memcpy((PVOID)0x8E03A000, &Globals::KV.ConsoleCertificate, 0x1A8);
		memcpy((PVOID)0x8E038020, &Globals::KV.ConsoleCertificate.ConsoleId.abData, 0x5);
		XeCryptSha((PBYTE)0x8E038014, 0x3EC, 0, 0, 0, 0, (PBYTE)0x8E038000, 0x14);
		QWORD KVAddress = HvxPeekQWORD(0x00000002000163C0);
		if (HvxPeekBytes(KVAddress + 0xD0, &Globals::KV.ConsoleObfuscationKey, 0x40) != S_OK) return FALSE;
		if (HvxPokeBytes(KVAddress, &Globals::KV, sizeof(KEY_VAULT)) != S_OK) return FALSE;
		Globals::SpoofedMACAddress[1] = 0x22;
		Globals::SpoofedMACAddress[2] = 0x48;
		Globals::SpoofedMACAddress[3] = Globals::KV.ConsoleCertificate.ConsoleId.asBits.MacIndex3;
		Globals::SpoofedMACAddress[4] = Globals::KV.ConsoleCertificate.ConsoleId.asBits.MacIndex4;
		Globals::SpoofedMACAddress[5] = Globals::KV.ConsoleCertificate.ConsoleId.asBits.MacIndex5;
		if (NT_SUCCESS(ExGetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, Globals::CurrentMACAddress, 0x6, 0)))
			if (memcmp(Globals::CurrentMACAddress, Globals::SpoofedMACAddress, 0x6) != 0)
				if (NT_SUCCESS(ExSetXConfigSetting(XCONFIG_SECURED_CATEGORY, XCONFIG_SECURED_MAC_ADDRESS, Globals::SpoofedMACAddress, 0x6))) HalReturnToFirmware(HalFatalErrorRebootRoutine);
		DWORD Temp = 0;
		XeCryptSha(Globals::SpoofedMACAddress, 0x6, 0, 0, 0, 0, (PBYTE)&Temp, 0x4);
		Globals::UpdateSequence |= (Temp & ~0xFF);
		XamCacheReset(XAM_CACHE_TICKETS);
		return TRUE;
	}
	PWCHAR PCHARtoWCHAR(PCHAR Text, ...) {
		CHAR Buffer[0x100]; PWCHAR Buffer2 = new WCHAR[0x200];
		va_list pArgList;
		va_start(pArgList, Text);
		vsprintf(Buffer, Text, pArgList);
		va_end(pArgList);
		Buffer2 = new WCHAR[strlen(Buffer) + 1];
		mbstowcs(Buffer2, Buffer, strlen(Buffer) + 1);
		ZeroMemory(Buffer, sizeof(Buffer));
		return Buffer2;
	}
	CHAR Buffer[0x1000]; CHAR MessageBuffer[0x100]; WCHAR Message[0x100];
	PWCHAR toWCHAR(PCHAR Text, ...)
	{
		va_list pArgList;
		va_start(pArgList, Text);
		vsprintf(Buffer, Text, pArgList);
		va_end(pArgList);
		sprintf(MessageBuffer, Buffer);
		mbstowcs(Message, MessageBuffer, strlen(MessageBuffer) + 1);
		ZeroMemory(Buffer, sizeof(Buffer));
		ZeroMemory(MessageBuffer, sizeof(MessageBuffer));
		return Message;
	}
	std::string utf8_encode(const std::wstring &wstr)
	{
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}
	std::wstring utf8_decode(const std::string &str)
	{
		if (str.empty()) return std::wstring();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}
	typedef HRESULT(*pDmSetMemory)(LPVOID lpbAddr, DWORD cb, LPCVOID lpbBuf, LPDWORD pcbRet);
	pDmSetMemory DevSetMemory = NULL;
	HRESULT SetMemory(VOID * Destination, VOID * Source, DWORD Length) {
		if (DevSetMemory == NULL)
			DevSetMemory = (pDmSetMemory)ResolveFunction("xbdm.xex", 40);
		if (DevSetMemory == NULL) {
			memcpy(Destination, Source, Length);
			return ERROR_SUCCESS;
		}
		else {
			if (DevSetMemory(Destination, Length, Source, NULL) == MAKE_HRESULT(0, 0x2da, 0))
				return ERROR_SUCCESS;
		}
		return E_FAIL;
	}
	bool hasConnection() {
		DWORD dwStatus = XNetGetEthernetLinkStatus();
		int m_bIsActive = (dwStatus & XNET_ETHERNET_LINK_ACTIVE) != 0;
		return m_bIsActive;
	}
	DWORD makeBranch(DWORD branchAddr, DWORD destination, BOOL linked) {
		return (0x48000000) | ((destination - branchAddr) & 0x03FFFFFF) | (DWORD)linked;
	}
	HANDLE searchForHandle(CONST CHAR* mName) {
		PLDR_DATA_TABLE_ENTRY curTab = (PLDR_DATA_TABLE_ENTRY)GetModuleHandle("xboxkrnl.exe");
		PXEX_HEADER_STRING peName;
		curTab = (PLDR_DATA_TABLE_ENTRY)curTab->InLoadOrderLinks.Flink;
		while (curTab != NULL) {
			peName = (PXEX_HEADER_STRING)RtlImageXexHeaderField(curTab->XexHeaderBase, 0x000183FF);
			if ((peName != NULL) && (peName->Data[0] != 0)) {
				if (stricmp((char*)peName->Data, mName) == 0) {
					HANDLE ret = (HANDLE)curTab;
					return ret;
				}
			} curTab = (PLDR_DATA_TABLE_ENTRY)curTab->InLoadOrderLinks.Flink;
		} return INVALID_HANDLE_VALUE;
	}
	VOID replaceHudString(const char* newtext, DWORD addr, INT length) {
		char* temp = new char[length + 1];
		for (INT i = 0; i < length; i++) temp[i] = ' ';
		temp[length] = '\0';
		int nLen = strlen(newtext);
		for (INT i = 0; i < nLen; i++)temp[i] = newtext[i];
		memcpy((LPVOID)addr, temp, length + 1);
	}
	string GetModuleNameFromAddress(DWORD dwAddress) {
		auto ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(GetModuleHandle("xboxkrnl.exe"));
		auto CurrentEntry = ldr->InLoadOrderLinks.Flink;
		PLDR_DATA_TABLE_ENTRY Current = nullptr;
		char buffer[100];
		while (CurrentEntry != &ldr->InLoadOrderLinks && CurrentEntry != nullptr) {
			Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (!Current)
				break;

			if (dwAddress >= reinterpret_cast<DWORD>(Current->ImageBase)) {
				if (dwAddress <= reinterpret_cast<DWORD>(Current->ImageBase) + Current->SizeOfFullImage) {
					wcstombs(buffer, Current->BaseDllName.Buffer, sizeof(buffer));
					return std::string(buffer);
				}
			}
			CurrentEntry = CurrentEntry->Flink;
		}
		return nullptr;
	}
	DWORD HookFunctionStub(DWORD _Address, void* Function) {
		DWORD* startStub = (DWORD*)&m_hookSection[m_hookCount * 32];
		m_hookCount++;

		for (auto i = 0; i < 7; i++)
			startStub[i] = 0x60000000;
		startStub[7] = 0x4E800020;

		HookFunctionStart((DWORD*)_Address, startStub, (DWORD)Function);
		return (DWORD)startStub;
	}
	DWORD HookFunctionStub(CHAR* ModuleName, DWORD Ordinal, void* Destination) {
		return HookFunctionStub((DWORD)ResolveFunction(ModuleName, Ordinal), Destination);
	}
	DWORD GetFunctionSize(DWORD* pdwAddress) {
		DWORD dwAddress = (DWORD)pdwAddress;
		DWORD dwSize = 0;
		while (*(DWORD*)dwAddress != 0x0000000 && *(DWORD*)dwAddress != 0x4E800020) { dwAddress += 4; dwSize += 4; }
		return dwSize > 0 ? (dwSize - 4) : 0;
	}
}