#pragma once
#include "stdafx.h"

class INI {
public:

	// 
	static bool EnableBlockXblDns;

	//Raibow
	static bool Rainbow;
	static bool linear;
	static bool Radial;

	// cheats
	static bool cheats;
	static bool GTAV;
	// ui
	static bool CustomNotification;
	static bool DrawGuideInformation;
	static bool DashUiModifications;
	static bool HudColorModifications;
	static bool front;

	// bypass
	static bool GhostBypass;
	static bool AwBypass;
	static bool BO2Bypass;
	static bool B03Bypass;

	//XeCheats
	static bool ESP_ENEMIES;
	static bool ESP_FRIENDLY;
	static bool ESP_DISTANCE;
	static bool ESP_NAME;
	static bool ESP_WEAPON;
	static bool ESP_BOX;
	static bool ESP_BONES;
	static bool NORECOIL;
	static bool NOSPREAD;
	static bool NOSWAY;
	static bool RADAR;
	static bool CROSSHAIR;
	static bool AIMBOT;
	static bool AUTOAIM;
	static bool StealthAIM;
	static string COLOR_ENEMIES;
	static string COLOR_FRIENDLY;
	static string COLOR_VISIBLE;
	static string COLOR_CROSSHAIR;
	static string COLOR_ENEMIES_SHAD;
	static string COLOR_FRIENDLY_SHAD;
	static string COLOR_VISIBLE_SHAD;
	static string COLOR_MENU;

	//Colors
	static void writerainbow();
	static void writelinear();
	static void writeradial();
	static void turnoffeverything();


	static void writerainbow2();
	static void writelinear2();
	static void writeradial2();
	static void turnoffeverything2();
	//Colors
	static void Init();
	static void Read();
	static void Create();
	static void XeCheatsini();
	static void ReadCheats();
	static void UpdateSettings();
};
