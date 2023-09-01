#include "stdafx.h"
#pragma once
VOID InitializeBypass();
class Games {

public:
	class AW {
	public:
		struct AWBypassData_s {
			DWORD Seed;
			DWORD SeedAnswer;
		};
		static AWBypassData_s AwBypassData;
	};

	class BO2 {
	public:
		static DWORD WINAPI B02Start();
	};

	class Ghosts {
	public:
		static DWORD WINAPI GhostsStart();
	};

	class B03 {
	public:
		static DWORD WINAPI B03Start();
	};
	class GTAV {
	public:
	};
	class MW3 {
	public:
		static DWORD WINAPI MW3Start();
	};
	class B01 {
	public:
		static DWORD WINAPI B01Start();
	};

	class DestinyClass {
	public:
		static DWORD WINAPI DestinyBypass();
	};

};


/*#include "stdafx.h"
#pragma once

class Games {
public:
	

};*/

