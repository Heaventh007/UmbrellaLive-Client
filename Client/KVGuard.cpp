#include "stdafx.h"
#include <vector>
using namespace std;
#define BASEADDY 0x90DF0000
typedef NTSTATUS(NTAPI* tNtCreateFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, PLARGE_INTEGER, DWORD, DWORD, DWORD, DWORD);
tNtCreateFile NtCreateFile_Original;
NTSTATUS NTAPI NtCreateFile_Hook(PHANDLE FihLE, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, DWORD FileAttributes, DWORD ShareAccess, DWORD CreateDisposition, DWORD CreateOptions) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}
	if (strstr(ObjectAttributes->ObjectName->Buffer, "kv") || strstr(ObjectAttributes->ObjectName->Buffer, "Kv") || strstr(ObjectAttributes->ObjectName->Buffer, "kV") || strstr(ObjectAttributes->ObjectName->Buffer, "KV")) {
		bool Stealth = (dwLink & 0xFFFF0000) == BASEADDY;
		if (!Stealth) {
			//printf("[XBLUmbrella] %s Attempted To Read KV.bin", Utilities::GetModuleNameFromAddress(dwLink).c_str());
			return ERROR_ACCESS_DENIED;
		}
	}

	return NtCreateFile_Original(FihLE, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions);
}
typedef NTSTATUS(NTAPI* tNtOpenFile)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PIO_STATUS_BLOCK, DWORD, DWORD);
tNtOpenFile NtOpenFile_Original;
NTSTATUS NtOpenFile_Hook(PHANDLE FihLE, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, DWORD ShareAccess, DWORD OpenOptions) {
	DWORD dwLink;
	__asm {
		mflr r31
		mr dwLink, r31
	}

	if (strstr(ObjectAttributes->ObjectName->Buffer, "kv") || strstr(ObjectAttributes->ObjectName->Buffer, "Kv") || strstr(ObjectAttributes->ObjectName->Buffer, "kV") || strstr(ObjectAttributes->ObjectName->Buffer, "KV")) {
		bool TheByte = (dwLink & 0xFFFF0000) == BASEADDY;
		if (!TheByte) {
			//printf("[XBLUmbrella] %s Attempted To Read KV.bin", Utilities::GetModuleNameFromAddress(dwLink).c_str());
			return ERROR_ACCESS_DENIED;
		}
	}
	return NtOpenFile_Original(FihLE, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess, OpenOptions);
}
void KVGuardSetup() {
	NtCreateFile_Original = reinterpret_cast<tNtCreateFile>(Utilities::HookFunctionStub(reinterpret_cast<DWORD>(NtCreateFile), NtCreateFile_Hook));
	NtOpenFile_Original = reinterpret_cast<tNtOpenFile>(Utilities::HookFunctionStub(reinterpret_cast<DWORD>(NtOpenFile), NtOpenFile_Hook));
	Utilities::NotifyPopup(L"Extra KV Protections For Backup - KV Guard Enabled");
}