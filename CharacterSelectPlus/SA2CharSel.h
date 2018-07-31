// SA2CharSel.cpp : Defines the exported functions for the DLL application.
// This code is made by MainMemory | https://github.com/MainMemory/SA2CharSel
// it was optimized to cooperate with my CharacterSelectPlus mod

#include "stdafx.h"
#include <cstdio>
#include <vector>
#include <algorithm>
#include "SA2ModLoader.h"
#include "IniFile.hpp"

using std::vector;
using std::string;
using std::unordered_map;
using std::transform;

DataArray(uint32_t, MenuPressedButtons, 0x1DEFB10, 4);

LevelCutscene *const stru_173A808 = (LevelCutscene*)0x173A808;
signed int __cdecl sub_458970()
{
	signed int v0; // ecx@3
	unsigned int v1; // eax@6

	if (MainCharObj2[0])
		v0 = MainCharObj2[0]->CharID2;
	else
		v0 = -1;
	if (v0 >= Characters_Amy) return 0;
	if (MainCharObj2[1] && MainCharObj2[1]->CharID2 >= Characters_Amy) return 0;
	if ( *(char*)0x1DEB321 && *(char*)0x1DEB320)
	{
		v1 = 0;
		while ( stru_173A808[v1].Level != (signed __int16)CurrentLevel
			|| stru_173A808[v1].Character )
		{
			v1++;
			if ( v1 >= 16 )
				return 0;
		}
		return 1;
	}
	else
		return 0;
}

bool __cdecl CheckEmeraldManager()
{
	if (MissionNum == 1 || MissionNum == 2)
		return false;
	if (*(int*)0x1AF014C)
		return false;
	switch ((short)CurrentLevel)
	{
	case LevelIDs_PumpkinHill:
	case LevelIDs_AquaticMine:
	case LevelIDs_SecurityHall:
	case LevelIDs_WildCanyon:
	case LevelIDs_DryLagoon:
	case LevelIDs_DeathChamber:
	case LevelIDs_EggQuarters:
	case LevelIDs_MeteorHerd:
	case LevelIDs_WildCanyon2P:
	case LevelIDs_MadSpace:
	case LevelIDs_DryLagoon2P:
	case LevelIDs_PoolQuest:
	case LevelIDs_PlanetQuest:
	case LevelIDs_DeathChamber2P:
		return true;
	}
	return false;
}

static const void *const loc_73AAC2 = (void*)0x73AAC2;
__declspec(naked) void LoadEmeraldManager_r()
{
	if (!CheckEmeraldManager())
		__asm retn;
	__asm
	{
		push	esi
			push	edi
			jmp loc_73AAC2
	}
}

template <typename T1, typename T2> struct pair { T1 key; T2 value; };

pair<short, short> SonicAnimReplacements[] = {
	{ 211, 1 },
	{ 212, 77 },
	{ 215, 15 }
};

pair<short, short> OthersAnimReplacements[] = {
	{ 76, 0 },
	{ 77, 15 },
	{ 185, 62 },
	{ 186, 62 },
	{ 187, 62 },
	{ 189, 62 },
	{ 190, 62 },
	{ 192, 15 },
	{ 193, 15 },
	{ 194, 15 },
	{ 195, 15 },
	{ 196, 15 },
	{ 197, 15 },
	{ 198, 15 },
	{ 211, 1 },
	{ 212, 62 },
	{ 215, 15 }
};

pair<short, short> KnucklesAnimReplacements[] = {
	{ 190, 75 },
	{ 192, 105 },
	{ 193, 105 },
	{ 194, 15 },
	{ 195, 15 },
	{ 196, 15 },
	{ 197, 15 },
	{ 198, 15 },
	{ 211, 1 },
	{ 212, 77 },
	{ 215, 15 }
};

pair<short, short> MechAnimReplacements[] = {
	{ 76, 0 },
	{ 77, 15 },
	{ 185, 75 },
	{ 186, 75 },
	{ 187, 75 },
	{ 189, 75 },
	{ 190, 75 },
	{ 192, 15 },
	{ 193, 15 },
	{ 194, 15 },
	{ 195, 15 },
	{ 196, 15 },
	{ 197, 15 },
	{ 198, 15 },
	{ 211, 1 },
	{ 212, 77 },
	{ 215, 15 }
};

#define altcostume 0x80u
#define altcharacter 0x40
#define charmask ~(altcostume|altcharacter)
int defaultcharacters[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };

#define Texlist_SonEff 0xA08B94
#define Texlist_ShadEff 0xA08D94
#define Texlist_AmyEff 0xA08F94
#define Texlist_MetEff 0xA0917C
const int loc_75783C = 0x75783C;
__declspec(naked) void __cdecl sub_757810()
{
	__asm
	{
		mov     eax, [esp+4]
		mov     eax, [eax]ObjectMaster.Data2
			movsx   ecx, [eax].CharID
			mov     edx, MainCharObj2
			mov edx, [edx+ecx*4]
		mov ecx, 0xA0B3B8
			mov dl, [edx].CharID2
			cmp     dl, Characters_Sonic
			jnz NotSonic
			mov     dword ptr [ecx], Texlist_SonEff
			jmp loc_75783C

NotSonic:
		cmp     dl, Characters_Shadow
			jnz NotShadow
			mov     dword ptr [ecx], Texlist_ShadEff
			jmp loc_75783C

NotShadow:
		cmp     dl, Characters_Amy
			jnz NotAmy
			mov     dword ptr [ecx], Texlist_AmyEff
			jmp loc_75783C

NotAmy:
		mov     dword ptr [ecx], Texlist_MetEff
			jmp loc_75783C
	}
}

const int loc_759A3C = 0x759A3C;
__declspec(naked) void loc_759A18()
{
	__asm
	{
		movsx	eax, [eax].CharID2
			mov	ecx, 0x2670544
			mov ecx, [ecx]
		cmp	eax, Characters_Sonic
			jne	short NotSonic
			mov	dword ptr [ecx+20h], Texlist_SonEff
			jmp	loc_759A3C

NotSonic:
		cmp	eax, Characters_Shadow
			jne	short NotShadow
			mov	dword ptr [ecx+20h], Texlist_ShadEff
			jmp	loc_759A3C

NotShadow:
		cmp	eax, Characters_Amy
			jne	short NotAmy
			mov	dword ptr [ecx+20h], Texlist_AmyEff
			jmp	loc_759A3C

NotAmy:
		mov	dword ptr [ecx+20h], Texlist_MetEff
			jmp	loc_759A3C
	}
}

StartPosition KnucklesStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_PumpkinHill, 0xD000u, 0xD000u, 0xD000u, { 199, -1361, -1035 }, { 188.63f, -1361, -1045 }, { 208.3f, -1361, -1021.5f } },
	{ LevelIDs_AquaticMine, 0x4000, 0x4000, 0x4000, { 0, 155, -233 }, { 10, 155, -233 }, { -10, 155, -233 } },
	{ LevelIDs_WildCanyon, 0x4000, 0x4000, 0x4000, { 200, 100, -400 }, { 220, 65.2f, -400 }, { 180, 65.2f, -400 } },
	{ LevelIDs_DeathChamber, 0xA000u, 0x8000u, 0xC000u, { 870, 70, 870 }, { 0, 240, 180 }, { 180, 240, 0 } },
	{ LevelIDs_KingBoomBoo, 0, 0, 0, { 276, -40, 190 }, { 276, -40, 190 }, { 276, -40, 190 } },
	{ LevelIDs_KnucklesVsRouge, 0, 0, 0, { -20, 0, 0 }, { -20, 0, 0 }, { -20, 0, 0 } },
	{ LevelIDs_WildCanyon2P, 0x4000, 0x4000, 0x4000, { 200, 100, -400 }, { 220, 66, -345 }, { 180, 66, -345 } },
	{ LevelIDs_CannonsCoreK, 0x8000u, 0x8000u, 0x8000u, { 0, 580, 60 }, { 0, 580, 60 }, { 0, 580, 60 } },
	{ LevelIDs_MeteorHerd, 0x4000, 0x4000, 0x4000, { 0, -450, -1025 }, { 25, -450, -1025 }, { -25, -450, -1025 } },
	{ LevelIDs_DryLagoon2P, 0x4000, 0x4000, 0x4000, { 220, 210, 1350 }, { 225, 480, 1325 }, { 185, 480, 1325 } },
	{ LevelIDs_PoolQuest, 0x4000, 0x4000, 0x4000, { 0, 155, -233 }, { 10, 155, -233 }, { -10, 155, -233 } },
	{ LevelIDs_PlanetQuest, 0, 0x4000, 0x4000, { 0, 660, 0 }, { -470, 1500, -630 }, { -430, 1500, -630 } },
	{ LevelIDs_SecurityHall, 0xA000u, 0xA000u, 0xE000u, { 405, -830, 410 }, { 405, -830, 410 }, { -405, -830, 410 } },
	{ LevelIDs_EggQuarters, 0xA000u, 0xC000u, 0xC000u, { 940, -30, 940 }, { 20, 110, 40 }, { -20, 110, 40 } },
	{ LevelIDs_MadSpace, 0x7000, 0xC000u, 0xC000u, { 0, 660, 0 }, { 30, 630, 17 }, { 0, 630, -35 } },
	{ LevelIDs_DeathChamber2P, 0xA000u, 0x8000u, 0xC000u, { 870, 70, 870 }, { 0, 240, 180 }, { 180, 240, 0 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 0, -400, -910 }, { -40, -400, -910 }, { 40, -400, -910 } },
	{ LevelIDs_Invalid }
};

StartPosition MechEggmanStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_IronGate },
	{ LevelIDs_WeaponsBed, 0xC000u, 0xC000u, 0xC000u, { 0 }, { 10, 0, 0 }, { -10, 0, 0 } },
	{ LevelIDs_WeaponsBed2P, 0xC000u, 0xC000u, 0x4000, { 50, -170, 50 }, { 20, -170, 95 }, { 20, -170, -95 } },
	{ LevelIDs_TailsVsEggman1, 0xB200u, 0xB200u, 0xB200u, { 50, -170, 50 }, { 50, -170, 50 }, { 50, -170, 50 } },
	{ LevelIDs_SandOcean, 0xC000u, 0xC000u, 0xC000u, { 0, 80, 0 }, { 0, 80, 0 }, { 0, 80, 0 } },
	{ LevelIDs_EternalEngine, 0xC000u, 0xC000u, 0xC000u, { 0, 370, 0 }, { 0, 370, 0 }, { 0, 370, 0 } },
	{ LevelIDs_LostColony, 0xC000u, 0xC000u, 0xC000u, { 0, -175, 29 }, { 0, -175, 29 }, { 0, -175, 29 } },
	{ LevelIDs_TailsVsEggman2, 0xB200u, 0xB200u, 0xB200u, { 50, -50, 80 }, { 50, -50, 80 }, { 50, -50, 80 } },
	{ LevelIDs_MissionStreet2P, 0, 0, 0x8000u, { -520, 0, 0 }, { -520, 0, 0 }, { 0 } },
	{ LevelIDs_EggGolemE, 0, 0, 0, { 0, 200, 210 }, { 0, 200, 210 }, { 0, 200, 210 } },
	{ LevelIDs_CannonsCoreE, 0xC000u, 0xC000u, 0xC000u, { 0, 20.1f, -200 }, { 0, 20.1f, -200 }, { 0, 20.1f, -200 } },
	{ LevelIDs_SandOcean2P, 0, 0, 0x8000u, { -60, 35, 0 }, { -60, 35, 0 }, { 60, 35, 0 } },
	{ LevelIDs_CosmicWall },
	{ LevelIDs_DeckRace, 0xC000u, 0xC000u, 0xC000u, { 0 }, { -15, -170, 1480 }, { 15, -170, 1480 } },
	{ LevelIDs_LostColony2P, 0, 0, 0x8000u, { -280, 150, 280 }, { -280, 150, 280 }, { 280, 150, -280 } },
	{ LevelIDs_PyramidRace, 0, 0xC000u, 0xC000u, { 0 }, { -35, 10, -20 }, { 35, 10, -20 } },
	{ LevelIDs_HiddenBase2P, 0, 0, 0x8000u, { 60, 0, 10 }, { -60, 0, 10 }, { 60, 0, 10 } },
	{ LevelIDs_CosmicWall2P, 0, 0, 0x8000u, { -80, 0, 0 }, { -80, 0, 0 }, { 80, 0, 5 } },
	{ LevelIDs_EternalEngine2P, 0x4000, 0x4000, 0xC000u, { 0, -50, -120 }, { 0, -50, -120 }, { 0, -50, 120 } },
	{ LevelIDs_IronGate2P, 0, 0, 0x8000u, { 0 }, { -50, -10, -20 }, { 50, -10, -20 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 0, 0, -1330 }, { -40, 0, -1330 }, { 40, 0, -1330 } },
	{ LevelIDs_Invalid }
};

StartPosition MechTailsStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_PrisonLane, 0x8000u, 0x8000u, 0x8000u, { 115, 10, 4 }, { 115, 10, 4 }, { 115, 10, 4 } },
	{ LevelIDs_WeaponsBed, 0xC000u, 0xC000u, 0xC000u, { 0 }, { 10, 0, 0 }, { -10, 0, 0 } },
	{ LevelIDs_WeaponsBed2P, 0xC000u, 0xC000u, 0x4000, { 50, -170, 50 }, { 20, -170, 95 }, { 20, -170, -95 } },
	{ LevelIDs_MissionStreet, 0xC000u, 0xC000u, 0xC000u, { 0, 200, 0 }, { 0, 200, 0 }, { 0, 200, 0 } },
	{ LevelIDs_TailsVsEggman1, 0x3800, 0x3800, 0x3800, { 50, -170, -250 }, { 50, -170, -250 }, { 50, -170, -250 } },
	{ LevelIDs_HiddenBase, 0xC000u, 0xC000u, 0xC000u, { 0, 140, 0 }, { 0, 140, 0 }, { 0, 140, 0 } },
	{ LevelIDs_EternalEngine, 0xC000u, 0xC000u, 0xC000u, { 0, 370, 0 }, { 0, 370, 0 }, { 0, 370, 0 } },
	{ LevelIDs_TailsVsEggman2, 0x3800, 0x3800, 0x3800, { 50, -50, -80 }, { 50, -50, -80 }, { 50, -50, -80 } },
	{ LevelIDs_MissionStreet2P, 0x8000u, 0, 0x8000u, { 0 }, { -520, 0, 0 }, { 0 } },
	{ LevelIDs_CannonsCoreT, 0xC000u, 0xC000u, 0xC000u, { 0, 670, 0 }, { 0, 670, 0 }, { 0, 670, 0 } },
	{ LevelIDs_SandOcean2P, 0, 0, 0x8000u, { -60, 35, 0 }, { -60, 35, 0 }, { 60, 35, 0 } },
	{ LevelIDs_DeckRace, 0xC000u, 0xC000u, 0xC000u, { 0 }, { -15, -170, 1480 }, { 15, -170, 1480 } },
	{ LevelIDs_LostColony2P, 0, 0, 0x8000u, { -280, 150, 280 }, { -280, 150, 280 }, { 280, 150, -280 } },
	{ LevelIDs_PyramidRace, 0, 0xC000u, 0xC000u, { 0 }, { -35, 10, -20 }, { 35, 10, -20 } },
	{ LevelIDs_HiddenBase2P, 0, 0, 0x8000u, { 60, 0, 10 }, { -60, 0, 10 }, { 60, 0, 10 } },
	{ LevelIDs_EternalEngine2P, 0x4000, 0x4000, 0xC000u, { 0, -50, -120 }, { 0, -50, -120 }, { 0, -50, 120 } },
	{ LevelIDs_IronGate2P, 0, 0, 0x8000u, { 0 }, { -50, -10, -20 }, { 50, -10, -20 } },
	{ LevelIDs_CosmicWall2P, 0, 0, 0x8000u, { -80, 0, 0 }, { -80, 0, 0 }, { 80, 0, 5 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 0, 0, -1330 }, { -40, 0, -1330 }, { 40, 0, -1330 } },
	{ LevelIDs_Invalid }
};

StartPosition RougeStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_PumpkinHill, 0xD000u, 0xD000u, 0xD000u, { 199, -1361, -1035 }, { 188.63f, -1361, -1045 }, { 208.3f, -1361, -1021.5f } },
	{ LevelIDs_SecurityHall, 0xA000u, 0xA000u, 0xE000u, { 405, -830, 410 }, { 405, -830, 410 }, { -405, -830, 410 } },
	{ LevelIDs_DeathChamber, 0xA000u, 0x8000u, 0xC000u, { 870, 70, 870 }, { 0, 240, 180 }, { 180, 240, 0 } },
	{ LevelIDs_EggQuarters, 0xA000u, 0xC000u, 0xC000u, { 940, -30, 940 }, { 20, 110, 40 }, { -20, 110, 40 } },
	{ LevelIDs_DryLagoon, 0x4000, 0x4000, 0x4000, { 200, 65, -400 }, { 200, 65, -400 }, { 200, 65, -400 } },
	{ LevelIDs_WildCanyon, 0x4000, 0x4000, 0x4000, { 200, 100, -400 }, { 220, 65.2f, -400 }, { 180, 65.2f, -400 } },
	{ LevelIDs_KnucklesVsRouge, 0x8000u, 0x8000u, 0x8000u, { 20, 0, 0 }, { 20, 0, 0 }, { 20, 0, 0 } },
	{ LevelIDs_WildCanyon2P, 0x4000, 0x4000, 0x4000, { 200, 100, -400 }, { 220, 66, -345 }, { 180, 66, -345 } },
	{ LevelIDs_CannonsCoreR, 0xC000u, 0xC000u, 0xC000u, { 0, 480, 150 }, { 0, 480, 150 }, { 0, 480, 150 } },
	{ LevelIDs_MeteorHerd, 0x4000, 0x4000, 0x4000, { 0, -450, -1025 }, { 25, -450, -1025 }, { -25, -450, -1025 } },
	{ LevelIDs_MadSpace, 0x7000, 0xC000u, 0xC000u, { 0, 660, 0 }, { 30, 630, 17 }, { 0, 630, -35 } },
	{ LevelIDs_DryLagoon2P, 0x4000, 0x4000, 0x4000, { 220, 210, 1350 }, { 225, 480, 1325 }, { 185, 480, 1325 } },
	{ LevelIDs_PoolQuest, 0x4000, 0x4000, 0x4000, { 0, 155, -233 }, { 10, 155, -233 }, { -10, 155, -233 } },
	{ LevelIDs_PlanetQuest, 0, 0x4000, 0x4000, { 0, 660, 0 }, { -470, 1500, -630 }, { -430, 1500, -630 } },
	{ LevelIDs_DeathChamber2P, 0xA000u, 0x8000u, 0xC000u, { 870, 70, 870 }, { 0, 240, 180 }, { 180, 240, 0 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 0, -400, -910 }, { -40, -400, -910 }, { 40, -400, -910 } },
	{ LevelIDs_Invalid }
};

StartPosition ShadowStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_GreenForest, 0x4000, 0x4000, 0x4000, { 1.61f, 40, -416 }, { 15, 40, -416 }, { -15, 40, -416 } },
	{ LevelIDs_WhiteJungle, 0x4000, 0x4000, 0x4000, { 0, -85, -45 }, { 15, -85, -45 }, { -15, -85, -45 } },
	{ LevelIDs_SkyRail, 0x4000, 0x4000, 0x4000, { -9.5f, 800, -526.1438f }, { -9.5f, 800, -526.1438f }, { 8.7f, 800, -526.1438f } },
	{ LevelIDs_CityEscape, 0x4000, 0x4000, 0x4000, { 0, 300, 340 }, { 10, 300, 340 }, { -10, 300, 340 } },
	{ LevelIDs_MetalHarbor, 0xC000u, 0xC000u, 0xC000u, { 0, 500, 40 }, { 10, 500, 45 }, { -10, 500, 45 } },
	{ LevelIDs_MetalHarbor2P, 0xC000u, 0xC000u, 0xC000u, { 0, 500, 40 }, { -10, 500, 45 }, { 10, 500, 45 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 103, 182, -39 }, { 103, 182, -39 }, { -103, 182, -39 } },
	{ LevelIDs_SonicVsShadow1, 0x8000u, 0x8000u, 0x8000u, { 60, 64, 0 }, { 60, 64, 0 }, { 60, 64, 0 } },
	{ LevelIDs_CrazyGadget, 0xC000u, 0xC000u, 0xC000u, { 0, 180, 340 }, { 20, 180, 340 }, { -20, 180, 340 } },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 0, 0, -1000 }, { 0, 0, -1000 }, { 0, 0, -1000 } },
	{ LevelIDs_SonicVsShadow2, 0, 0, 0, { 0, 0, 15 }, { 0, 0, 15 }, { 0, 0, 15 } },
	{ LevelIDs_FinalRush, 0, 0, 0, { -100, 0, 0 }, { -100, 0, -25 }, { -100, 0, 25 } },
	{ LevelIDs_FinalChase, 0, 0x4000, 0x4000, { -223, -100, 63 }, { 3206, -4000, 2455 }, { 3176, -4000, 2455 } },
	{ LevelIDs_GreenHill, 0xC000u, 0xC000u, 0xC000u, { 0, 40, 0 }, { 15, 40, 0 }, { -15, 40, 0 } },
	{ LevelIDs_DowntownRace, 0x4000, 0x4000, 0x4000, { -2215, 4320, -4670 }, { -2235, 4320, -4670 }, { -2195, 4320, -4670 } },
	{ LevelIDs_GrindRace, 0, 0, 0, { 10, -445, -900 }, { 10, -795, -900 }, { -10, -795, -900 } },
	{ LevelIDs_Invalid }
};

StartPosition SonicStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_GreenForest, 0x4000, 0x4000, 0x4000, { 1.61f, 40, -416 }, { 15, 40, -416 }, { -15, 40, -416 } },
	{ LevelIDs_SkyRail, 0x4000, 0x4000, 0x4000, { -9.5f, 800, -526.1438f }, { -9.5f, 800, -526.1438f }, { 8.7f, 800, -526.1438f } },
	{ LevelIDs_MetalHarbor, 0xC000u, 0xC000u, 0xC000u, { 0, 500, 40 }, { 10, 500, 45 }, { -10, 500, 45 } },
	{ LevelIDs_MetalHarbor2P, 0xC000u, 0xC000u, 0xC000u, { 0, 500, 40 }, { -10, 500, 45 }, { 10, 500, 45 } },
	{ LevelIDs_CityEscape, 0x4000, 0x4000, 0x4000, { 0, 300, 340 }, { 10, 300, 340 }, { -10, 300, 340 } },
	{ LevelIDs_WhiteJungle, 0x4000, 0x4000, 0x4000, { 0, -85, -45 }, { 15, -80, -45 }, { -15, -80, -45 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 103, 182, -39 }, { 103, 182, -39 }, { -103, 182, -39 } },
	{ LevelIDs_SonicVsShadow1, 0, 0, 0, { -60, 64, 0 }, { -60, 64, 0 }, { -60, 64, 0 } },
	{ LevelIDs_CrazyGadget, 0xC000u, 0xC000u, 0xC000u, { 0, 180, 340 }, { 20, 180, 340 }, { -20, 180, 340 } },
	{ LevelIDs_PyramidCave, 0xC000u, 0xC000u, 0xC000u, { 0, 300, 0 }, { 0, 300, 0 }, { 0, 300, 0 } },
	{ LevelIDs_FinalRush, 0, 0, 0, { -100, 0, 0 }, { -100, 0, -25 }, { -100, 0, 25 } },
	{ LevelIDs_FinalChase, 0, 0x4000, 0x4000, { -223, -100, 63 }, { 3206, -4000, 2455 }, { 3176, -4000, 2455 } },
	{ LevelIDs_CannonsCoreS, 0xC000u, 0xC000u, 0xC000u, { 0, 45, 0 }, { 0, 45, 0 }, { 0, 45, 0 } },
	{ LevelIDs_EggGolemS, 0, 0, 0, { 0, 200, 210 }, { 0, 200, 210 }, { 0, 200, 210 } },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 0, 0, 1000 }, { 0, 0, 1000 }, { 0, 0, 1000 } },
	{ LevelIDs_SonicVsShadow2, 0, 0, 0, { 0, 0, -15 }, { 0, 0, -15 }, { 0, 0, -15 } },
	{ LevelIDs_GreenHill, 0xC000u, 0xC000u, 0xC000u, { 0, 40, 0 }, { 15, 40, 0 }, { -15, 40, 0 } },
	{ LevelIDs_DowntownRace, 0x4000, 0x4000, 0x4000, { -2215, 4320, -4670 }, { -2235, 4320, -4670 }, { -2195, 4320, -4670 } },
	{ LevelIDs_GrindRace, 0, 0, 0, { 10, -445, -900 }, { 10, -795, -900 }, { -10, -795, -900 } },
	{ LevelIDs_Invalid }
};

StartPosition SuperShadowStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 0, 0, -1000 }, { 0, 0, -1000 }, { 0, 0, -1000 } },
	{ LevelIDs_Invalid }
};

StartPosition SuperSonicStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 0, 0, 1000 }, { 0, 0, 1000 }, { 0, 0, 1000 } },
	{ LevelIDs_Invalid }
};

StartPosition TailsStart[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { 0, -400, -910 }, { -40, -400, -910 }, { 40, -400, -910 } },
	{ LevelIDs_Invalid }
};

StartPosition *NSonicStartArray[] = { SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, KnucklesStart, RougeStart, SuperSonicStart, SuperShadowStart, TailsStart };
StartPosition *NShadowStartArray[] = { ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, RougeStart, KnucklesStart, SuperShadowStart, SuperSonicStart, TailsStart };
StartPosition *NMechTailsStartArray[] = { MechTailsStart, MechEggmanStart, SonicStart, ShadowStart, KnucklesStart, RougeStart, SuperSonicStart, SuperShadowStart, TailsStart };
StartPosition *NMechEggmanStartArray[] = { MechEggmanStart, MechTailsStart, ShadowStart, SonicStart, RougeStart, KnucklesStart, SuperShadowStart, SuperSonicStart, TailsStart };
StartPosition *NKnucklesStartArray[] = { KnucklesStart, RougeStart, SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, SuperSonicStart, SuperShadowStart, TailsStart };
StartPosition *NRougeStartArray[] = { RougeStart, KnucklesStart, ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, SuperShadowStart, SuperSonicStart, TailsStart };
StartPosition *NSuperSonicStartArray[] = { SuperSonicStart, SuperShadowStart, SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, KnucklesStart, RougeStart, TailsStart };
StartPosition *NSuperShadowStartArray[] = { SuperShadowStart, SuperSonicStart, ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, RougeStart, KnucklesStart, TailsStart };
StartPosition *NTailsStartArray[] = { TailsStart, SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, KnucklesStart, RougeStart, SuperSonicStart, SuperShadowStart };
StartPosition *NEggmanStartArray[] = { TailsStart, ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, RougeStart, KnucklesStart, SuperShadowStart, SuperSonicStart };

int __cdecl LoadStartPosition_ri(int playerNum, NJS_VECTOR *position, Rotation *rotation)
{
	ObjectMaster *v1; // eax@1
	CharObj2Base *v4; // eax@7
	StartPosition **list;
	StartPosition *v5; // eax@9
	int v6; // edx@25
	NJS_VECTOR *v8; // edx@35

	v1 = MainCharacter[playerNum];
	if ( position )
	{
		position->z = 0.0;
		position->y = 0.0;
		position->x = 0.0;
	}
	if ( rotation )
	{
		rotation->z = 0;
		rotation->y = 0;
		rotation->x = 0;
	}
	if ( v1 )
	{
		v4 = MainCharObj2[playerNum];
		if ( v4 )
		{
			switch ( v4->CharID )
			{
			case Characters_Sonic:
				list = NSonicStartArray;
				break;
			case Characters_Shadow:
				list = NShadowStartArray;
				break;
			case Characters_Knuckles:
				list = NKnucklesStartArray;
				break;
			case Characters_Rouge:
				list = NRougeStartArray;
				break;
			case Characters_Tails:
				list = NTailsStartArray;
				break;
			case Characters_Eggman:
				list = NEggmanStartArray;
				break;
			case Characters_MechEggman:
				list = NMechEggmanStartArray;
				break;
			case Characters_MechTails:
				list = NMechTailsStartArray;
				break;
			case Characters_SuperSonic:
				list = NSuperSonicStartArray;
				break;
			case Characters_SuperShadow:
				list = NSuperShadowStartArray;
				break;
			default:
				return 1;
			}
		}
		else
			return 1;
		if ( TwoPlayerMode
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow1
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
			|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge )
			v6 = (playerNum != 0) + 1;
		else
			v6 = 0;
		for (int i = 0; i < (int)LengthOfArray(NSonicStartArray); i++)
		{
			v5 = list[i];
			if ( v5 )
			{
				while ( v5->Level != LevelIDs_Invalid )
				{
					if ( v5->Level == (short)CurrentLevel )
					{
						if ( rotation )
							rotation->y = *(&v5->Rotation1P + v6);
						if ( position )
						{
							v8 = &(&v5->Position1P)[v6];
							position->x = v8->x;
							position->y = v8->y;
							position->z = v8->z;
						}
						return 1;
					}
					++v5;
				}
			}
		}
		return 1;
	}
	return 0;
}

__declspec(naked) void LoadStartPosition_r()
{
	__asm
	{
		mov eax, [esp+4]
		push eax
			push edi
			push ecx
			call LoadStartPosition_ri
			add esp, 12
			retn
	}
}

LevelEndPosition KnucklesEnd[] = {
	{ LevelIDs_PumpkinHill, 0x8000u, 0xC000u, 0, { 530, -986, -770 }, { -13, 34.8f, 1275 } },
	{ LevelIDs_AquaticMine, 0, 0, 0, { 0, 130, -365 }, { -600, 211, 443 } },
	{ LevelIDs_WildCanyon, 0xC000u, 0xC000u, 0, { 705, 1010, -14 }, { 1700, 0, 121 } },
	{ LevelIDs_DeathChamber, 0, 0xE000u, 0, { 100, 100, 180 }, { -805, -35, -805 } },
	{ LevelIDs_MeteorHerd, 0, 0x5200, 0, { 10, 170, 140 }, { -438, 2752, -432 } },
	{ LevelIDs_CannonsCoreK, 0xC000u, 0xFFFFu, 0, { 0, 660, -160 }, { 0, 580, 60 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition MechEggmanEnd[] = {
	{ LevelIDs_IronGate, 0x8000u, 0xC000u, 0, { 1490, -270, -1025 }, { 3184, -233, -702 } },
	{ LevelIDs_WeaponsBed, 0x4000, 0xFA00u, 0, { -4, -160, -40 }, { 1500, -220, -6008 } },
	{ LevelIDs_SandOcean, 0x4000, 0x4A00, 0, { 0, 80, 0 }, { 659, 120, -4660 } },
	{ LevelIDs_LostColony, 0x8000u, 0, 0, { 595, -518, -2350 }, { 2225, -308, -2400 } },
	{ LevelIDs_CannonsCoreE, 0, 0xFFFFu, 0, { 80, -30, -1570 }, { 0, 20, -200 } },
	{ LevelIDs_CosmicWall, 0x4000, 0xA000u, 0, { 6673, 4000, 11677 }, { 2753, 2583, -1045 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition MechTailsEnd[] = {
	{ LevelIDs_PrisonLane, 0x8000u, 0x8000u, 0, { -350, 117, 410 }, { -3129, 790, -242 } },
	{ LevelIDs_MissionStreet, 0x8000u, 0, 0, { 2200, 800, -3500 }, { 4065, 758, -4170 } },
	{ LevelIDs_HiddenBase, 0, 0x4000, 0, { -2295, 110, -2845 }, { -3525, 920, -4960 } },
	{ LevelIDs_EternalEngine, 0x4000, 0x4000, 0, { -2055, 936, -5825 }, { -652, 788, -4426 } },
	{ LevelIDs_CannonsCoreT, 0, 0xFFFFu, 0, { -1050, 638, -1540 }, { 0, 670, 0 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition RougeEnd[] = {
	{ LevelIDs_SecurityHall, 0, 0x4000, 0, { 100, -573, 490 }, { 0, 314.6f, 110 } },
	{ LevelIDs_DryLagoon, 0x8000u, 0x7000, 0, { 191, 211, 1317 }, { 1728, 0, -382 } },
	{ LevelIDs_EggQuarters, 0x8000u, 0x8000u, 0, { 100, 70, 1255 }, { -655, -20, 2680 } },
	{ LevelIDs_CannonsCoreR, 0x4000, 0xFFFFu, 0, { 80, 135, 80 }, { 0, 480, 150 } },
	{ LevelIDs_MadSpace, 0xEC00u, 0, 0, { 272.5f, 2358, -334.5f }, { -154, 3350, 481 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition ShadowEnd[] = {
	{ LevelIDs_BasicTest, 0, 0, 0, { 0 }, { 0 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0x8000u, 0, { -40, -400, -970 }, { -6330, -4500, -8830 } },
	{ LevelIDs_WhiteJungle, 0xC000u, 0xB000u, 0, { 5040, -2220, -1550 }, { 13280, -3157, -7370 } },
	{ LevelIDs_FinalChase, 0xC000u, 0xE000u, 0, { 3408, -6592, 16865 }, { -695, -6959.5f, 10275 } },
	{ LevelIDs_SkyRail, 0x8000u, 0xA000u, 0, { -2411, -1792, 4260 }, { -3457, -1047, 3001 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition SonicEnd[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_GreenForest, 0x8000u, 0x8000u, 0, { 5858, -1812, 7541 }, { 6890, -1610, 7542 } },
	{ LevelIDs_MetalHarbor, 0, 0x4000, 0, { 2158, -160, -5294 }, { 1707, -170, -6583 } },
	{ LevelIDs_CityEscape, 0xC000u, 0x4000, 0, { -1570, -6060, 8860 }, { 7700, -13145, 5570 } },
	{ LevelIDs_CrazyGadget, 0xC000u, 0x8000u, 0, { -9710, -1045, -4005 }, { -8725, -537, -2905 } },
	{ LevelIDs_PyramidCave, 0x4000, 0, 0, { -685, -4190, -19525 }, { -2170, -3970, -21870 } },
	{ LevelIDs_FinalRush, 0, 0xC000u, 0, { 5776, -15687, 20080 }, { 4207, -16632, 24462 } },
	{ LevelIDs_CannonsCoreS, 0x4000, 0x4000, 0, { 0, -480, -1550 }, { -510, -655, -4700 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition *NSonicEndArray[] = { SonicEnd, ShadowEnd, MechTailsEnd, MechEggmanEnd, KnucklesEnd, RougeEnd };
LevelEndPosition *NShadowEndArray[] = { ShadowEnd, SonicEnd, MechEggmanEnd, MechTailsEnd, RougeEnd, KnucklesEnd };
LevelEndPosition *NMechTailsEndArray[] = { MechTailsEnd, MechEggmanEnd, SonicEnd, ShadowEnd, KnucklesEnd, RougeEnd };
LevelEndPosition *NMechEggmanEndArray[] = { MechEggmanEnd, MechTailsEnd, ShadowEnd, SonicEnd, RougeEnd, KnucklesEnd };
LevelEndPosition *NKnucklesEndArray[] = { KnucklesEnd, RougeEnd, SonicEnd, ShadowEnd, MechTailsEnd, MechEggmanEnd };
LevelEndPosition *NRougeEndArray[] = { RougeEnd, KnucklesEnd, ShadowEnd, SonicEnd, MechEggmanEnd, MechTailsEnd };

StartPosition KnucklesStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_PumpkinHill, 0xD000u, 0xD000u, 0xD000u, { 199, -1361, -1035 }, { 188.63f, -1361, -1045 }, { 208.3f, -1361, -1021.5f } },
	{ LevelIDs_AquaticMine, 0x4000, 0, 0, { 0, 130, -340 }, { 10, 130, -200 }, { -10, 130, -200 } },
	{ LevelIDs_WildCanyon, 0x4000, 0x4000, 0x4000, { 200, 856, -140 }, { 220, 856, -140 }, { 180, 856, -140 } },
	{ LevelIDs_DeathChamber, 0x2000, 0x2000, 0x2000, { 830, 20, 830 }, { 830, 20, 830 }, { 830, 20, 830 } },
	{ LevelIDs_EggQuarters, 0x2000, 0xA000u, 0xA000u, { 980, -30, 980 }, { -820, 20, 820 }, { -820, 20, 820 } },
	{ LevelIDs_KingBoomBoo, 0x1800, 0x1800, 0x1800, { 374, -40, -120 }, { 374, -40, -120 }, { 374, -40, -120 } },
	{ LevelIDs_KnucklesVsRouge, 0, 0, 0, { 0, -340, 10 }, { 0, -340, 10 }, { 0, -340, 10 } },
	{ LevelIDs_WildCanyon2P, 0x4000, 0xF000u, 0xF000u, { 200, 856, -140 }, { -359, 927, -44 }, { -359, 927, -44 } },
	{ LevelIDs_CannonsCoreK, 0, 0, 0, { -435, -175, -1735 }, { -435, -175, 1735 }, { -435, -175, 1735 } },
	{ LevelIDs_MeteorHerd, 0x4000, 0x4000, 0x4000, { 0, -500, -1025 }, { 25, -500, -1025 }, { -25, -500, -1025 } },
	{ LevelIDs_DryLagoon2P, 0x4000, 0x4000, 0x4000, { 200, 210, 1350 }, { 200, 210, 1350 }, { 200, 210, 1350 } },
	{ LevelIDs_PoolQuest, 0, 0, 0, { 0, 130, -200 }, { 10, 130, -200 }, { -10, 130, -200 } },
	{ LevelIDs_PlanetQuest, 0, 0, 0, { 0, 620, 0 }, { -432, 1640.5f, -560 }, { -432, 1640.5f, -560 } },
	{ LevelIDs_SecurityHall, 0xA000u, 0x6000, 0xA000u, { 405, -933, 410 }, { 217, -933, 410 }, { 217, -933, 410 } },
	{ LevelIDs_MadSpace, 0xF000u, 0xF000u, 0xF000u, { 0, 620, 0 }, { 0, 620, 5 }, { 0, 620, 5 } },
	{ LevelIDs_DeathChamber2P, 0x2000, 0x2000, 0x2000, { 830, 20, 830 }, { 830, 20, 830 }, { 830, 20, 830 } },
	{ LevelIDs_Invalid }
};

StartPosition MechEggmanStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_IronGate, 0, 0, 0, { 6150, -1521, -1230 }, { 6150, -1521, -1230 }, { 6150, -1521, -1230 } },
	{ LevelIDs_WeaponsBed, 0, 0, 0, { 4251, -220, -10138 }, { 4231, -220, -10138 }, { 4281, -220, -10138 } },
	{ LevelIDs_WeaponsBed2P, 0x4000, 0x4000, 0x4000, { 140, -170, 20 }, { 140, -170, 20 }, { 140, -170, 20 } },
	{ LevelIDs_TailsVsEggman1, 0x4000, 0x4000, 0x4000, { 140, -170, 20 }, { 140, -170, 20 }, { 140, -170, 20 } },
	{ LevelIDs_SandOcean, 0x8000u, 0x8000u, 0x8000u, { 2895, 155, -7185 }, { 2895, 155, -7185 }, { 2895, 155, -7185 } },
	{ LevelIDs_EternalEngine, 0xC000u, 0xC000u, 0xC000u, { 207, -1826, -9150 }, { 207, -1826, -9150 }, { 207, -1826, -9150 } },
	{ LevelIDs_LostColony, 0, 0, 0, { 4710, 1552, -2400 }, { 4710, 1552, -2400 }, { 4710, 1552, -2400 } },
	{ LevelIDs_TailsVsEggman2, 0x4000, 0x4000, 0x4000, { 0, -50, -181.5f }, { 0, -50, -181.5f }, { 0, -50, -181.5f } },
	{ LevelIDs_MissionStreet2P, 0xC000u, 0xC000u, 0xC000u, { 0, 0, -25 }, { 0, 0, -25 }, { 0, 0, -25 } },
	{ LevelIDs_SandOcean2P, 0x4000, 0x4000, 0x4000, { 17, 30, -150 }, { 17, 30, -150 }, { 17, 30, -150 } },
	{ LevelIDs_CannonsCoreE, 0xC000u, 0xC000u, 0xC000u, { 570, -2630, -3450 }, { 570, -2630, -3450 }, { 570, -2630, -3450 } },
	{ LevelIDs_EggGolemE, 0, 0, 0, { 0, 200, 220 }, { 0, 200, 220 }, { 0, 200, 220 } },
	{ LevelIDs_CosmicWall, 0x8000u, 0x8000u, 0x8000u, { 3550, -1600, 29690 }, { 3550, -1600, 29690 }, { 3550, -1600, 29690 } },
	{ LevelIDs_HiddenBase2P, 0, 0x4000, 0x4000, { 0, 0, 30 }, { -20, 0, 25 }, { -20, 0, 25 } },
	{ LevelIDs_IronGate2P, 0, 0xC000u, 0xC000u, { 0 }, { -30, -10, -20 }, { -30, -10, -20 } },
	{ LevelIDs_LostColony2P, 0, 0, 0x2000, { -150, 0, 0 }, { -150, 0, 0 }, { -280, 0, -170 } },
	{ LevelIDs_PyramidRace, 0, 0, 0, { 0 }, { 0, 0, -2020 }, { 0, 0, -2020 } },
	{ LevelIDs_DeckRace, 0xC000u, 0xC000u, 0xC000u, { 0 }, { 0, -170, -475 }, { 0, -170, -475 } },
	{ LevelIDs_EternalEngine2P, 0x6000, 0x6000, 0x6000, { 0, -50, -120 }, { 0, -50, -120 }, { 0, -50, 120 } },
	{ LevelIDs_Invalid }
};

StartPosition MechTailsStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_PrisonLane, 0xC000u, 0xC000u, 0xC000u, { -4440, 940, -1178 }, { -4440, 940, -1178 }, { -4440, 940, -1178 } },
	{ LevelIDs_WeaponsBed, 0, 0, 0, { 4251, -220, -10138 }, { 4231, -220, -10138 }, { 4281, -220, -10138 } },
	{ LevelIDs_WeaponsBed2P, 0xC000u, 0xC000u, 0xC000u, { 140, -170, -40 }, { 140, -170, -40 }, { 140, -170, -40 } },
	{ LevelIDs_MissionStreet, 0xC000u, 0xC000u, 0xC000u, { 7170, 598, -6913 }, { 7170, 598, -6913 }, { 7170, 598, -6913 } },
	{ LevelIDs_TailsVsEggman1, 0xC000u, 0xC000u, 0xC000u, { 140, -170, -40 }, { 140, -170, -40 }, { 140, -170, -40 } },
	{ LevelIDs_HiddenBase, 0x4000, 0x4000, 0x4000, { -3035, 1280, -5400 }, { -3035, 1280, -5400 }, { -3035, 1280, -5400 } },
	{ LevelIDs_EternalEngine, 0xC000u, 0xC000u, 0xC000u, { 207, -1826, -9150 }, { 207, -1826, -9150 }, { 207, -1826, -9150 } },
	{ LevelIDs_TailsVsEggman2, 0xC000u, 0xC000u, 0xC000u, { 140, -50, -40 }, { 140, -50, -40 }, { 140, -50, -40 } },
	{ LevelIDs_MissionStreet2P, 0x4000, 0x4000, 0x4000, { 0, 0, 25 }, { 0, 0, 25 }, { 0, 0, 25 } },
	{ LevelIDs_SandOcean2P, 0x4000, 0x4000, 0x4000, { 17, 30, -150 }, { 17, 30, -150 }, { 17, 30, -150 } },
	{ LevelIDs_CannonsCoreE, 0xC000u, 0xC000u, 0xC000u, { -1990, -1258, -2500 }, { -1990, -1258, -2500 }, { -1990, -1258, -2500 } },
	{ LevelIDs_HiddenBase2P, 0, 0x4000, 0x4000, { 0, 0, 30 }, { -20, 0, 25 }, { -20, 0, 25 } },
	{ LevelIDs_IronGate2P, 0, 0xC000u, 0xC000u, { 0 }, { -30, -10, -20 }, { -30, -10, -20 } },
	{ LevelIDs_LostColony2P, 0, 0, 0x2000, { -150, 0, 0 }, { -150, 0, 0 }, { -280, 0, -170 } },
	{ LevelIDs_PyramidRace, 0, 0, 0, { 0 }, { 0, 0, -2020 }, { 0, 0, -2020 } },
	{ LevelIDs_DeckRace, 0xC000u, 0xC000u, 0xC000u, { 0 }, { 0, -170, -475 }, { 0, -170, -475 } },
	{ LevelIDs_EternalEngine2P, 0x6000, 0x6000, 0x6000, { 0, -50, -120 }, { 0, -50, -120 }, { 0, -50, 120 } },
	{ LevelIDs_Invalid }
};

StartPosition RougeStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_PumpkinHill, 0xD000u, 0xD000u, 0xD000u, { 199, -1361, -1035 }, { 188.63f, -1361, -1045 }, { 208.3f, -1361, -1021.5f } },
	{ LevelIDs_SecurityHall, 0xA000u, 0x6000, 0xA000u, { 405, -933, 410 }, { 217, -933, 410 }, { 217, -933, 410 } },
	{ LevelIDs_DryLagoon, 0x4000, 0x4000, 0x4000, { 200, 15, -140 }, { 200, 15, -140 }, { 200, 15, -140 } },
	{ LevelIDs_WildCanyon, 0x4000, 0x4000, 0x4000, { 200, 856, -140 }, { 220, 856, -140 }, { 180, 856, -140 } },
	{ LevelIDs_DeathChamber, 0x2000, 0xE000u, 0xE000u, { 830, 20, 830 }, { 830, 20, 830 }, { 830, 20, 830 } },
	{ LevelIDs_EggQuarters, 0x2000, 0x2000, 0x2000, { 980, -30, 980 }, { -820, 20, 820 }, { -820, 20, 820 } },
	{ LevelIDs_KnucklesVsRouge, 0, 0, 0, { 10, -340, 0 }, { 10, -340, 0 }, { 10, -340, 0 } },
	{ LevelIDs_WildCanyon2P, 0x4000, 0xF000u, 0xF000u, { 200, 856, -400 }, { -359, 927, -44 }, { -359, 927, -44 } },
	{ LevelIDs_CannonsCoreR, 0x8000u, 0x8000u, 0x8000u, { 120, 20, 0 }, { 120, 20, 0 }, { 120, 20, 0 } },
	{ LevelIDs_FlyingDog },
	{ LevelIDs_MeteorHerd, 0x4000, 0x4000, 0x4000, { 0, -500, -1025 }, { 25, -500, -1025 }, { -25, -500, -1025 } },
	{ LevelIDs_MadSpace, 0xF000u, 0xF000u, 0xF000u, { 0, 620, 0 }, { 0, 620, 5 }, { 0, 620, 5 } },
	{ LevelIDs_DryLagoon2P, 0x4000, 0x4000, 0x4000, { 200, 210, 1350 }, { 200, 210, 1350 }, { 200, 210, 1350 } },
	{ LevelIDs_PoolQuest, 0, 0, 0, { 0, 130, -200 }, { 10, 130, -200 }, { -10, 130, -200 } },
	{ LevelIDs_PlanetQuest, 0, 0, 0, { 0, 620, 0 }, { -432, 1640.5f, -560 }, { -432, 1640.5f, -560 } },
	{ LevelIDs_DeathChamber2P, 0x2000, 0xE000u, 0xE000u, { 830, 20, 830 }, { 830, 20, 830 }, { 830, 20, 830 } },
	{ LevelIDs_Invalid }
};

StartPosition ShadowStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_GreenForest, 0x8000u, 0x8000u, 0x8000u, { 10935, -1854, 11056 }, { 10935, -1854, 11076 }, { 10935, -1854, 11066 } },
	{ LevelIDs_WhiteJungle, 0xC000u, 0xC000u, 0xC000u, { 13166, -3599, -5518 }, { 9135, -3154, -4930 }, { 9135, -3154, -4930 } },
	{ LevelIDs_SkyRail, 0x4000, 0x4000, 0x4000, { -1343.7f, -4928, 10098 }, { -1333.7f, -4928, 10098 }, { -1353.7f, -4928, 10098 } },
	{ LevelIDs_CityEscape, 0x4000, 0x8000u, 0x8000u, { 3955, -22175, 16130 }, { 6450, -15915, 9070 }, { 6450, -15915, 9070 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { -9970, -6999, -18430 }, { -9970, -6999, -18350 }, { -9970, -6999, -18350 } },
	{ LevelIDs_SonicVsShadow1, 0x6C00, 0x6C00, 0x6C00, { 10, -42, 0 }, { 10, -42, 0 }, { 10, -42, 0 } },
	{ LevelIDs_CrazyGadget, 0xC000u, 0xC000u, 0xC000u, { -8470, -1097, -2904 }, { -8470, -1097, -2904 }, { -8470, -1097, -2904 } },
	{ LevelIDs_HotShot },
	{ LevelIDs_Biolizard, 0, 0, 0, { -265, 10, 0 }, { -265, 10, 0 }, { -265, 10, 0 } },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 0, 0, -1000 }, { 0, 0, -1000 }, { 0, 0, -1000 } },
	{ LevelIDs_SonicVsShadow2, 0x4000, 0x4000, 0x4000, { -292, 0, 0 }, { -292, 0, 0 }, { -292, 0, 0 } },
	{ LevelIDs_FinalChase, 0, 0xC000u, 0xC000u, { 122, -9960, 7627 }, { 3440, -6812, 17730 }, { 3440, -6812, 17730 } },
	{ LevelIDs_GreenHill, 0xC000u, 0xC000u, 0xC000u, { -1970, -1390, -6940 }, { -1970, -1390, -6940 }, { -1970, -1390, -6910 } },
	{ LevelIDs_FinalRush, 0, 0x4000, 0x4000, { 2530, -26495.5f, 7465 }, { 2531, -20965, 11350 }, { 2531, -20965, 11350 } },
	{ LevelIDs_MetalHarbor2P, 0, 0x8000u, 0x8000u, { 8854, -31, -10370 }, { 4913, -31, -10370 }, { 4913, -31, -10370 } },
	{ LevelIDs_GrindRace, 0x8000u, 0x8000u, 0x8000u, { 13990, -35280, 10050 }, { 13990, -35280, 10050 }, { 13990, -35280, 10050 } },
	{ LevelIDs_DowntownRace, 0xC000u, 0xC000u, 0xC000u, { -1540, -6060, 8800 }, { -1540, -6060, 8800 }, { -1540, -6060, 8770 } },
	{ LevelIDs_Invalid }
};

StartPosition SonicStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_GreenForest, 0x8000u, 0x8000u, 0x8000u, { 10935, -1854, 11056 }, { 10935, -1854, 11076 }, { 10935, -1854, 11066 } },
	{ LevelIDs_SkyRail, 0x4000, 0x4000, 0x4000, { -1343.7f, -4928, 10098 }, { -1333.7f, -4928, 10098 }, { -1353.7f, -4928, 10098 } },
	{ LevelIDs_MetalHarbor, 0x6000, 0, 0, { 8777, -170, -10240 }, { 8854, -170, -10241 }, { 8854, -170, -10281 } },
	{ LevelIDs_CityEscape, 0x4000, 0x8000u, 0x8000u, { 3955, -22175, 16130 }, { 6450, -15915, 9070 }, { 6450, -15915, 9070 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0xC000u, { -9970, -6999, -18430 }, { -9970, -6999, -18350 }, { -9970, -6999, -18350 } },
	{ LevelIDs_SonicVsShadow1, 0x800, 0x800, 0x800, { -10, -42, 0 }, { -10, -42, 0 }, { -10, -42, 0 } },
	{ LevelIDs_CrazyGadget, 0xC000u, 0xC000u, 0xC000u, { -8470, -1097, -2904 }, { -8470, -1097, -2904 }, { -8470, -1097, -2904 } },
	{ LevelIDs_PyramidCave, 0x4000, 0x4000, 0x4000, { 940, -4060, -22190 }, { 940, -4060, -22190 }, { 940, -4060, -22190 } },
	{ LevelIDs_FinalRush, 0, 0x4000, 0x4000, { 2530, -26495.5f, 7465 }, { 2531, -20965, 11350 }, { 2531, -20965, 11350 } },
	{ LevelIDs_BigFoot, 0, 0, 0, { 0, -1, 0 }, { 0, -1, 0 }, { 0, -1, 0 } },
	{ LevelIDs_EggGolemS, 0, 0, 0, { 0, 200, 220 }, { 0, 200, 220 }, { 0, 200, 220 } },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 0, 0, 1000 }, { 0, 0, 1000 }, { 0, 0, 1000 } },
	{ LevelIDs_CannonsCoreS, 0x1000, 0x1000, 0x1000, { -1600, -5755, -9565 }, { -308, 0, 0 }, { -308, 0, 0 } },
	{ LevelIDs_SonicVsShadow2, 0x4000, 0x4000, 0x4000, { -308, 0, 0 }, { -308, 0, 0 }, { -308, 0, 0 } },
	{ LevelIDs_GreenHill, 0xC000u, 0xC000u, 0xC000u, { -1970, -1390, -6940 }, { -1970, -1390, -6940 }, { -1970, -1390, -6910 } },
	{ LevelIDs_DowntownRace, 0xC000u, 0xC000u, 0xC000u, { -1540, -6060, 8800 }, { -1540, -6060, 8800 }, { -1540, -6060, 8770 } },
	{ LevelIDs_GrindRace, 0x8000u, 0x8000u, 0x8000u, { 13990, -35280, 10050 }, { 13990, -35280, 10050 }, { 13990, -35280, 10050 } },
	{ LevelIDs_MetalHarbor2P, 0, 0x8000u, 0x8000u, { 8854, -31, -10370 }, { 4913, -31, -10370 }, { 4913, -31, -10370 } },
	{ LevelIDs_FinalChase, 0, 0xC000u, 0xC000u, { 122, -9960, 7627 }, { 3440, -6812, 17730 }, { 3440, -6812, 17730 } },
	{ LevelIDs_WhiteJungle, 0xC000u, 0xC000u, 0xC000u, { 13166, -3599, -5518 }, { 9135, -3154, -4930 }, { 9135, -3154, -4930 } },
	{ LevelIDs_Invalid }
};

StartPosition SuperShadowStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 600, -400, 200 }, { 600, -400, 200 }, { 600, -400, 200 } },
	{ LevelIDs_Invalid }
};

StartPosition SuperSonicStart2[] = {
	{ LevelIDs_BasicTest },
	{ LevelIDs_FinalHazard, 0, 0, 0, { 600, -400, 200 }, { 600, -400, 200 }, { 600, -400, 200 } },
	{ LevelIDs_Invalid }
};

StartPosition *NSonicStartArray2[] = { SonicStart2, ShadowStart2, MechTailsStart2, MechEggmanStart2, KnucklesStart2, RougeStart2, SuperSonicStart2, SuperShadowStart2 };
StartPosition *NShadowStartArray2[] = { ShadowStart2, SonicStart2, MechEggmanStart2, MechTailsStart2, RougeStart2, KnucklesStart2, SuperShadowStart2, SuperSonicStart2 };
StartPosition *NMechTailsStartArray2[] = { MechTailsStart2, MechEggmanStart2, SonicStart2, ShadowStart2, KnucklesStart2, RougeStart2, SuperSonicStart2, SuperShadowStart2 };
StartPosition *NMechEggmanStartArray2[] = { MechEggmanStart2, MechTailsStart2, ShadowStart2, SonicStart2, RougeStart2, KnucklesStart2, SuperShadowStart2, SuperSonicStart2 };
StartPosition *NKnucklesStartArray2[] = { KnucklesStart2, RougeStart2, SonicStart2, ShadowStart2, MechTailsStart2, MechEggmanStart2, SuperSonicStart2, SuperShadowStart2 };
StartPosition *NRougeStartArray2[] = { RougeStart2, KnucklesStart2, ShadowStart2, SonicStart2, MechEggmanStart2, MechTailsStart2, SuperShadowStart2, SuperSonicStart2 };
StartPosition *NSuperSonicStartArray2[] = { SuperSonicStart2, SuperShadowStart2, SonicStart2, ShadowStart2, MechTailsStart2, MechEggmanStart2, KnucklesStart2, RougeStart2 };
StartPosition *NSuperShadowStartArray2[] = { SuperShadowStart2, SuperSonicStart2, MechEggmanStart2, MechTailsStart2, RougeStart2, KnucklesStart2, ShadowStart2, SonicStart2 };

static const void *const sub_46DC70Ptr = (void*)0x46DC70;
void sub_46DC70(int a1, NJS_VECTOR *a2, char a3)
{
	__asm
	{
		xor eax, eax
			mov al, [a3]
		push eax
			mov eax, [a1]
		mov ecx, [a2]
		call sub_46DC70Ptr
			add esp, 4
	}
}

DataArray(char, byte_1DE4664, 0x1DE4664, 2);
DataPointer(void *, off_1DE95E0, 0x1DE95E0);

signed int sub_46DBD0(int a1)
{
	CharObj2Base *v1; // eax@1
	signed int result; // eax@2

	v1 = MainCharObj2[a1];
	if ( v1 )
		result = v1->CharID;
	else
		result = -1;
	return result;
}

signed int LoadEndPosition_r(int playerNum)
{
	int v1; // edi@1
	__int16 v2; // bp@2
	int v3; // edx@12
	EntityData1 *v4; // esi@12
	LevelEndPosition **list;
	LevelEndPosition *v5 = NULL; // eax@13
	int v8; // edi@24
	int v9; // ecx@24
	NJS_VECTOR *v10; // eax@24
	float v11; // ST14_4@24

	v1 = playerNum;
	if ( TwoPlayerMode
		|| (v2 = CurrentLevel, (short)CurrentLevel == LevelIDs_SonicVsShadow1)
		|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
		|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
		|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
		|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge
		|| (short)CurrentLevel > LevelIDs_BigFoot && (short)CurrentLevel != LevelIDs_Route101280
		|| MissionNum != 1 && MissionNum != 2 )
		return 0;
	else
	{
		v3 = MissionNum == 1 ? 0 : 1;
		v4 = MainCharacter[playerNum]->Data1.Entity;
		switch ( sub_46DBD0(playerNum) )
		{
		case Characters_Sonic:
		case Characters_Tails:
		case Characters_SuperSonic:
			list = NSonicEndArray;
			break;
		case Characters_Shadow:
		case Characters_Eggman:
		case Characters_SuperShadow:
			list = NShadowEndArray;
			break;
		case Characters_Knuckles:
			list = NKnucklesEndArray;
			break;
		case Characters_Rouge:
			list = NRougeEndArray;
			break;
		case Characters_MechEggman:
			list = NMechEggmanEndArray;
			break;
		case Characters_MechTails:
			list = NMechTailsEndArray;
			break;
		default:
			return 0;
		}
		for (int i = 0; i < (int)LengthOfArray(NSonicEndArray); i++)
		{
			v5 = list[i];
			while ( v5->Level != LevelIDs_Invalid )
			{
				if ( v5->Level == v2 )
					goto endloop;
				++v5;
			}
		}
	endloop:
		if (*(&v5->Mission2YRotation + v3) == 0xFFFF )
			return 0;
		v4->Rotation.z = 0;
		v4->Rotation.x = 0;
		v9 = *(&v5->Mission2YRotation + v3);
		v4->Rotation.y = v9;
		*((int *)*(&off_1DE95E0 + v1) + 7) = v9;
		v10 = (NJS_VECTOR *)((char *)&v5->Mission2Position + 12 * v3);
		v4->Position = *v10;
		v11 = v4->Position.y - 10.0f;
		*(float *)&MainCharObj2[v1]->field_1A0[20] = v11;
		MainCharObj2[v1]->field_144[0] = 0;
		sub_46DC70(v1, &v4->Position, 0);
		v4->Collision->CollisionArray->field_2 |= 0x70u;
		MainCharObj2[v1]->gap_70[24] = 0;
		MainCharObj2[v1]->gap_70[25] = 0;
		MainCharObj2[v1]->gap_70[26] = 0;
		MainCharObj2[v1]->gap_70[27] = 0;
		v8 = v1 & 1;
		if ( (short)CurrentLevel == LevelIDs_LostColony )
		{
			byte_1DE4664[v8] = 5;
			return 1;
		}
		else
		{
			byte_1DE4664[v8] = *(char*)0x1DE4660;
			return 1;
		}
	}
}

void __cdecl sub_43DF30_i(int playerNum)
{
	int v1; // edi@1
	ObjectMaster *v2; // esi@1
	CharObj2Base *v3; // eax@3
	EntityData1 *v4; // esi@3
	StartPosition **list;
	StartPosition *v5; // eax@5
	int v6; // edx@20
	int v8; // ecx@30
	float v10; // ST14_4@30

	v1 = playerNum;
	v2 = MainCharacter[playerNum];
	if ( v2 && LoadEndPosition_r(playerNum) != 1 )
	{
		v3 = MainCharObj2[v1];
		v4 = v2->Data1.Entity;
		if ( v3 )
		{
			switch ( v3->CharID )
			{
			case Characters_Sonic:
			case Characters_Tails:
				list = NSonicStartArray2;
				break;
			case Characters_Shadow:
			case Characters_Eggman:
				list = NShadowStartArray2;
				break;
			case Characters_Knuckles:
				list = NKnucklesStartArray2;
				break;
			case Characters_Rouge:
				list = NRougeStartArray2;
				break;
			case Characters_MechEggman:
				list = NMechEggmanStartArray2;
				break;
			case Characters_MechTails:
				list = NMechTailsStartArray2;
				break;
			case Characters_SuperSonic:
				list = NSuperSonicStartArray2;
				break;
			case Characters_SuperShadow:
				list = NSuperShadowStartArray2;
				break;
			default:
				goto LABEL_13;
			}
		}
		else
		{
LABEL_13:
			list = 0;
		}
		if ( TwoPlayerMode
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow1
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
			|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge )
			v6 = (v1 != 0) + 1;
		else
			v6 = 0;
		if ( list )
		{
			for (int i = 0; i < (int)LengthOfArray(NSonicStartArray2); i++)
			{
				v5 = list[i];
				while ( v5->Level != LevelIDs_Invalid )
				{
					if ( v5->Level == (short)CurrentLevel )
					{
						v4->Rotation.z = 0;
						v4->Rotation.x = 0;
						v8 = *(&v5->Rotation1P + v6);
						v4->Rotation.y = v8;
						*((int *)*(&off_1DE95E0 + v1) + 7) = v8;
						v4->Position = (&v5->Position1P)[v6];
						v10 = v4->Position.y - 10.0f;
						*(float *)&MainCharObj2[v1]->field_1A0[20] = v10;
						MainCharObj2[v1]->field_144[0] = 0;
						goto LABEL_27;
					}
					++v5;
				}
			}
		}
		v4->Rotation.z = 0;
		v4->Rotation.y = 0;
		v4->Rotation.x = 0;
		v4->Position.z = 0.0;
		v4->Position.y = 0.0;
		v4->Position.x = 0.0;
		*((int *)*(&off_1DE95E0 + v1) + 7) = 0;
LABEL_27:
		sub_46DC70(v1, &v4->Position, 0);
		v4->Collision->CollisionArray->field_2 |= 0x70u;
		MainCharObj2[v1]->gap_70[24] = 0;
		MainCharObj2[v1]->gap_70[25] = 0;
		MainCharObj2[v1]->gap_70[26] = 0;
		MainCharObj2[v1]->gap_70[27] = 0;
		if ( (short)CurrentLevel == LevelIDs_RadicalHighway || (short)CurrentLevel == LevelIDs_LostColony )
			byte_1DE4664[v1 & 1] = 5;
		else
			byte_1DE4664[v1 & 1] = *(char*)0x1DE4660;
	}
}

__declspec(naked) void sub_43DF30()
{
	__asm
	{
		push eax
		call sub_43DF30_i
		add esp, 4
		retn
	}
}

LevelEndPosition Knuckles2PIntro[] = {
	{ LevelIDs_PumpkinHill, 0x5800, 0x5800, 0, { -193, -742.38f, -908.24f }, { -193, -742.38f, -908.24f } },
	{ LevelIDs_DeathChamber, 0, 0xC000u, 0, { 0, 210, 150 }, { 150, 210, 0 } },
	{ LevelIDs_WildCanyon2P, 0x4000, 0x4000, 0, { 220, 65.2f, -400 }, { 180, 65.2f, -400 } },
	{ LevelIDs_MeteorHerd, 0, 0x8000u, 0, { 190, -340, 0 }, { -190, -340, 0 } },
	{ LevelIDs_DryLagoon2P, 0, 0x8000u, 0, { 40, 270, 1438 }, { 355, 270, 1438 } },
	{ LevelIDs_EggQuarters, 0x2000, 0x6000, 0, { -845, 20, 845 }, { 845, 20, 845 } },
	{ LevelIDs_SecurityHall, 0x6000, 0xB000u, 0, { 470, -573, 490 }, { -395, -416, -20 } },
	{ LevelIDs_PoolQuest, 0x4000, 0x4000, 0, { 10, 130, -225 }, { -10, 130, -225 } },
	{ LevelIDs_PlanetQuest, 0, 0, 0, { -432, 1640.5f, -560 }, { -432, 1640.5f, -560 } },
	{ LevelIDs_MadSpace, 0x9800u, 0x4000, 0, { 30, 620, 17 }, { 0, 620, -35 } },
	{ LevelIDs_DeathChamber2P, 0, 0xC000u, 0, { 0, 210, 150 }, { 150, 210, 0 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition MechEggman2PIntro[] = {
	{ LevelIDs_WeaponsBed2P, 0xC000u, 0xC000u, 0, { 50, -170, 50 }, { 50, -170, 50 } },
	{ LevelIDs_MissionStreet2P, 0, 0, 0, { -450, 0, 0 }, { -450, 0, 0 } },
	{ LevelIDs_SandOcean2P, 0, 0x8000u, 0, { -155, 30, -15 }, { 155, 30, -15 } },
	{ LevelIDs_DeckRace, 0xC000u, 0xC000u, 0, { -15, -170, 1480 }, { 15, -170, 1480 } },
	{ LevelIDs_PyramidRace, 0x4000, 0x4000, 0, { -35, 0, -200 }, { 35, 0, -220 } },
	{ LevelIDs_HiddenBase2P, 0, 0x8000u, 0, { 60, 0, 10 }, { -60, 0, 10 } },
	{ LevelIDs_CosmicWall2P, 0, 0x8000u, 0, { -80, 0, 0 }, { 80, 0, 0 } },
	{ LevelIDs_EternalEngine2P, 0x4000, 0xC000u, 0, { 0, -50, 120 }, { 0, -50, -150 } },
	{ LevelIDs_IronGate2P, 0, 0x8000u, 0, { -50, -10, -20 }, { 50, -10, -20 } },
	{ LevelIDs_LostColony2P, 0xC000u, 0, 0, { 280, 65, -110 }, { -90, 0, 0 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition MechTails2PIntro[] = {
	{ LevelIDs_WeaponsBed2P, 0x4000, 0x4000, 0, { 50, -170, -250 }, { 50, -170, -250 } },
	{ LevelIDs_MissionStreet2P, 0x8000u, 0x8000u, 0, { 0 }, { 0 } },
	{ LevelIDs_SandOcean2P, 0, 0x8000u, 0, { -155, 30, -15 }, { 155, 30, -15 } },
	{ LevelIDs_DeckRace, 0xC000u, 0xC000u, 0, { -15, -170, 1480 }, { 15, -170, 1480 } },
	{ LevelIDs_PyramidRace, 0x4000, 0x4000, 0, { -35, 0, -200 }, { 35, 0, -220 } },
	{ LevelIDs_HiddenBase2P, 0, 0x8000u, 0, { 60, 0, 10 }, { -60, 0, 10 } },
	{ LevelIDs_CosmicWall2P, 0, 0x8000u, 0, { -80, 0, 0 }, { 80, 0, 0 } },
	{ LevelIDs_EternalEngine2P, 0x4000, 0xC000u, 0, { 0, -50, 120 }, { 0, -50, -150 } },
	{ LevelIDs_IronGate2P, 0, 0x8000u, 0, { -50, -10, -20 }, { 50, -10, -20 } },
	{ LevelIDs_LostColony2P, 0xC000u, 0, 0, { 280, 65, -110 }, { -90, 0, 0 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition Rouge2PIntro[] = {
	{ LevelIDs_PumpkinHill, 0xC000u, 0xC000u, 0, { 0, -590, -1642 }, { 0, -590, -1642 } },
	{ LevelIDs_DeathChamber, 0, 0xC000u, 0, { 0, 210, 150 }, { 150, 210, 0 } },
	{ LevelIDs_WildCanyon2P, 0x4000, 0x4000, 0, { 220, 65.2f, -400 }, { 180, 65.2f, -400 } },
	{ LevelIDs_MeteorHerd, 0, 0x8000u, 0, { 190, -340, 0 }, { -190, -340, 0 } },
	{ LevelIDs_DryLagoon2P, 0, 0x8000u, 0, { 40, 270, 1438 }, { 355, 270, 1438 } },
	{ LevelIDs_EggQuarters, 0x2000, 0x6000, 0, { -845, 20, 845 }, { 845, 20, 845 } },
	{ LevelIDs_SecurityHall, 0x6000, 0xB000u, 0, { 470, -573, 490 }, { -395, -416, -20 } },
	{ LevelIDs_PoolQuest, 0x4000, 0x4000, 0, { 10, 130, -225 }, { -10, 130, -225 } },
	{ LevelIDs_PlanetQuest, 0, 0, 0, { -432, 1640.5f, -560 }, { -432, 1640.5f, -560 } },
	{ LevelIDs_MadSpace, 0x9800u, 0x4000, 0, { 30, 620, 17 }, { 0, 620, -35 } },
	{ LevelIDs_DeathChamber2P, 0, 0xC000u, 0, { 0, 210, 150 }, { 150, 210, 0 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition Shadow2PIntro[] = {
	{ LevelIDs_GreenForest, 0, 0, 0, { 1247, -955, 2920 }, { 1247, -955, 2920 } },
	{ LevelIDs_SkyRail, 0xED00u, 0xED00u, 0, { -297, -724.9f, 2118 }, { -297, -724.9f, 2118 } },
	{ LevelIDs_MetalHarbor2P, 0x4000, 0x4000, 0, { 2065, -160, -4325 }, { 2065, -160, -4325 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0, { 105, 200, 0 }, { -105, 200, 0 } },
	{ LevelIDs_GreenHill, 0x5800, 0x5800, 0, { -760, -70, -2930 }, { -760, -70, -2930 } },
	{ LevelIDs_CityEscape, 0x8000u, 0x8000u, 0, { 150, -2226, 4010 }, { 150, -2226, 4010 } },
	{ LevelIDs_FinalRush, 0, 0, 0, { -100, 0, -25 }, { -100, 0, 25 } },
	{ LevelIDs_FinalChase, 0x8000u, 0x4000, 0, { 3600, -4525, 4675 }, { 3600, -4525, 4675 } },
	{ LevelIDs_GrindRace, 0, 0, 0, { 0, -815, -910 }, { 0, -815, -910 } },
	{ LevelIDs_WhiteJungle, 0x4000, 0xA000u, 0, { 4205, -2126, 430 }, { 6395, -1919, -2650 } },
	{ LevelIDs_DowntownRace, 0xF700u, 0xB00, 0, { -2010, 4793, -5215 }, { -2010, 4793, -5110 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition Sonic2PIntro[] = {
	{ LevelIDs_GreenForest, 0, 0, 0, { -87, -860, 1305 }, { -87, -860, 1305 } },
	{ LevelIDs_SkyRail, 0x1800, 0x1800, 0, { -2548, -1762.4f, 3508 }, { -2548, -1762.4f, 3508 } },
	{ LevelIDs_MetalHarbor2P, 0x4000, 0x4000, 0, { 2065, -160, -4325 }, { 2065, -160, -4325 } },
	{ LevelIDs_RadicalHighway, 0xC000u, 0xC000u, 0, { 105, 200, 0 }, { -105, 200, 0 } },
	{ LevelIDs_GreenHill, 0x4000, 0x4000, 0, { -80, 70, -480 }, { -80, 70, -480 } },
	{ LevelIDs_CityEscape, 0xC000u, 0xC000u, 0, { 90, -2813, 5000 }, { 90, -2813, 5000 } },
	{ LevelIDs_FinalRush, 0, 0, 0, { -100, 0, -25 }, { -100, 0, 25 } },
	{ LevelIDs_FinalChase, 0x8000u, 0x4000, 0, { 3600, -4525, 4675 }, { 3600, -4525, 4675 } },
	{ LevelIDs_GrindRace, 0, 0, 0, { 0, -815, -910 }, { 0, -815, -910 } },
	{ LevelIDs_WhiteJungle, 0x4000, 0xA000u, 0, { 4205, -2126, 430 }, { 6395, -1919, -2650 } },
	{ LevelIDs_DowntownRace, 0xF700u, 0xB00, 0, { -2010, 4793, -5215 }, { -2010, 4793, -5110 } },
	{ LevelIDs_Invalid },
};

LevelEndPosition *NSonic2PIntroArray[] = { Sonic2PIntro, Shadow2PIntro, MechTails2PIntro, MechEggman2PIntro, Knuckles2PIntro, Rouge2PIntro };
LevelEndPosition *NShadow2PIntroArray[] = { Shadow2PIntro, Sonic2PIntro, MechEggman2PIntro, MechTails2PIntro, Rouge2PIntro, Knuckles2PIntro };
LevelEndPosition *NMechTails2PIntroArray[] = { MechTails2PIntro, MechEggman2PIntro, Sonic2PIntro, Shadow2PIntro, Knuckles2PIntro, Rouge2PIntro };
LevelEndPosition *NMechEggman2PIntroArray[] = { MechEggman2PIntro, MechTails2PIntro, Shadow2PIntro, Sonic2PIntro, Rouge2PIntro, Knuckles2PIntro };
LevelEndPosition *NKnuckles2PIntroArray[] = { Knuckles2PIntro, Rouge2PIntro, Sonic2PIntro, Shadow2PIntro, MechTails2PIntro, MechEggman2PIntro };
LevelEndPosition *NRouge2PIntroArray[] = { Rouge2PIntro, Knuckles2PIntro, Shadow2PIntro, Sonic2PIntro, MechEggman2PIntro, MechTails2PIntro };

void __cdecl Load2PIntroPos_ri(int playerNum)
{
	ObjectMaster *v1; // eax@1
	int v2; // edi@1
	CharObj2Base *v3; // eax@2
	EntityData1 *v4; // esi@2
	LevelEndPosition **list;
	LevelEndPosition *v5; // eax@4
	bool v6; // edx@11
	NJS_VECTOR *v8; // ecx@15
	CharObj2Base *v9; // eax@16
	int v10; // edi@16
	char v11; // al@16
	NJS_VECTOR *v12; // eax@20
	float v13; // ST10_4@20

	v2 = playerNum;
	v1 = MainCharacter[playerNum];
	if ( v1 )
	{
		v4 = v1->Data1.Entity;
		v3 = MainCharObj2[v2];
		if ( v3 )
		{
			switch ( v3->CharID )
			{
			case Characters_Sonic:
			case Characters_Tails:
			case Characters_SuperSonic:
				list = NSonic2PIntroArray;
				break;
			case Characters_Shadow:
			case Characters_Eggman:
			case Characters_SuperShadow:
				list = NShadow2PIntroArray;
				break;
			case Characters_Knuckles:
				list = NKnuckles2PIntroArray;
				break;
			case Characters_Rouge:
				list = NRouge2PIntroArray;
				break;
			case Characters_MechEggman:
				list = NMechEggman2PIntroArray;
				break;
			case Characters_MechTails:
				list = NMechTails2PIntroArray;
				break;
			default:
				goto LABEL_10;
			}
		}
		else
		{
LABEL_10:
			list = 0;
		}
		v6 = v2 != 0;
		if ( list )
		{
			for (int i = 0; i < (int)LengthOfArray(NSonic2PIntroArray); i++)
			{
				v5 = list[i];
				while ( v5->Level != LevelIDs_Invalid )
				{
					if ( v5->Level == (short)CurrentLevel )
					{
						v4->Rotation.y = *(&v5->Mission2YRotation + v6);
						v12 = (NJS_VECTOR *)((char *)&v5->Mission2Position + 12 * v6);
						v4->Position = *v12;
						v8 = &v4->Position;
						*((int *)*(&off_1DE95E0 + v2) + 7) = v4->Rotation.y;
						v13 = v4->Position.y - 10.0f;
						*(float *)&MainCharObj2[v2]->field_1A0[5] = v13;
						goto LABEL_16;
					}
					++v5;
				}
			}
		}
		v4->Position.z = 0.0;
		v8 = &v4->Position;
		v4->Position.y = 0.0;
		v4->Rotation.y = 0;
		v4->Position.x = 0.0;
LABEL_16:
		sub_46DC70(v2, v8, 0);
		v4->Collision->CollisionArray->field_2 |= 0x70u;
		v11 = *(char*)0x1DE4660;
		MainCharObj2[v2]->gap_70[24] = 0;
		MainCharObj2[v2]->gap_70[25] = 0;
		MainCharObj2[v2]->gap_70[26] = 0;
		MainCharObj2[v2]->gap_70[27] = 0;
		byte_1DE4664[v2 & 1] = v11;
		v9 = MainCharObj2[v2];
		v10 = (int)*(&off_1DE95E0 + v2);
		if ( v9 )
		{
			v9->Speed.x = 0.0;
			v9->Speed.y = 0.0;
			v9->Speed.z = 0.0;
		}
		if ( v10 )
		{
			*(float *)(v10 + 8) = 0.0;
			*(float *)(v10 + 4) = 0.0;
			*(float *)v10 = 0.0;
		}
	}
}

__declspec(naked) void Load2PIntroPos_r()
{
	__asm
	{
		push eax
			call Load2PIntroPos_ri
			add esp, 4
			retn
	}
}

void __cdecl LoadBossCharacter()
{
	int character = CurrentCharacter ^ 1;
	int buttons = MenuPressedButtons[1];
	if (buttons & Buttons_Left)
		character = Characters_Sonic;
	if (buttons & Buttons_Right)
		character = Characters_Shadow;
	if (buttons & Buttons_Down)
		character = Characters_Knuckles;
	if (buttons & Buttons_Up)
		character = Characters_Rouge;
	if (buttons & Buttons_R)
		character = Characters_MechTails;
	if (buttons & Buttons_L)
		character = Characters_MechEggman;
	if (buttons & Buttons_Y)
		character = Characters_Tails;
	if (buttons & Buttons_X)
		character = Characters_Eggman;
	if (buttons & Buttons_B)
		AltCharacter[1] = 1;
	if (buttons & Buttons_A)
		AltCostume[1] = 1;
	switch (character)
	{
	case Characters_Sonic:
		LoadSonic(1);
		break;
	case Characters_Shadow:
		LoadShadow(1);
		break;
	case Characters_Tails:
		LoadTails(1);
		break;
	case Characters_Eggman:
		LoadEggman(1);
		break;
	case Characters_Knuckles:
		LoadKnuckles(1);
		break;
	case Characters_Rouge:
		LoadRouge(1);
		break;
	case Characters_MechTails:
		LoadMechTails(1);
		break;
	case Characters_MechEggman:
		LoadMechEggman(1);
		break;
	}
}

const void *const loc_6193F5 = (void*)0x6193F5;
__declspec(naked) void sub_6193D0()
{
	LoadBossCharacter();
	__asm
	{
		push ebx
		push esi
		push edi
		xor ebx, ebx
		jmp loc_6193F5
	}
}

const void *const loc_4C7120 = (void*)0x4C7120;
__declspec(naked) void sub_4C7100()
{
	LoadBossCharacter();
	__asm jmp loc_4C7120
}

const void *const loc_6486B2 = (void*)0x6486B2;
__declspec(naked) void sub_648690()
{
	LoadBossCharacter();
	__asm jmp loc_6486B2
}

const void *const loc_6266A2 = (void*)0x6266A2;
__declspec(naked) void sub_626680()
{
	LoadBossCharacter();
	__asm jmp loc_6266A2
}

const void *const loc_661D12 = (void*)0x661D12;
__declspec(naked) void sub_661CF0()
{
	LoadBossCharacter();
	__asm jmp loc_661D12
}

DataPointer(NJS_TEXLIST, OTexList_SonicLife, 0x171A654);
DataPointer(NJS_TEXLIST, OTexList_ShadowLife, 0x171A65C);
DataPointer(NJS_TEXLIST, OTexList_TailsLife, 0x171A670);
DataPointer(NJS_TEXLIST, OTexList_EggmanLife, 0x171A690);
DataPointer(NJS_TEXLIST, OTexList_KnucklesLife, 0x171A6A4);
DataPointer(NJS_TEXLIST, OTexList_RougeLife, 0x171A6B8);
DataPointer(NJS_TEXLIST, OTexList_AmyLife, 0x171A6CC);
DataPointer(NJS_TEXLIST, OTexList_MetalLife, 0x171A6E0);
DataPointer(NJS_TEXLIST, OTexList_ChaoLife, 0x171A6F4);
DataPointer(NJS_TEXLIST, OTexList_DarkChaoLife, 0x171A708);
DataPointer(NJS_TEXLIST, OTexList_TikalLife, 0x171A71C);
DataPointer(NJS_TEXLIST, OTexList_ChaosLife, 0x171A730);
CharObj2Base *__cdecl loc_727E5B_i(int playerNum)
{
	CharObj2Base *v13 = MainCharObj2[playerNum];
	NJS_TEXLIST *v12 = 0;
	if ( v13 )
	{
		switch ( v13->CharID2 )
		{
		case Characters_Sonic:
		case Characters_SuperSonic:
			v12 = &OTexList_SonicLife;
			break;
		case Characters_Shadow:
		case Characters_SuperShadow:
			v12 = &OTexList_ShadowLife;
			break;
		case Characters_Amy:
			v12 = &OTexList_AmyLife;
			break;
		case Characters_MetalSonic:
			v12 = &OTexList_MetalLife;
			break;
		case Characters_Tails:
		case Characters_MechTails:
			v12 = &OTexList_TailsLife;
			break;
		case Characters_ChaoWalker:
			v12 = &OTexList_ChaoLife;
			break;
		case Characters_DarkChaoWalker:
			v12 = &OTexList_DarkChaoLife;
			break;
		case Characters_Knuckles:
			v12 = &OTexList_KnucklesLife;
			break;
		case Characters_Rouge:
			v12 = &OTexList_RougeLife;
			break;
		case Characters_Tikal:
			v12 = &OTexList_TikalLife;
			break;
		case Characters_Chaos:
			v12 = &OTexList_ChaosLife;
			break;
		default:
			return 0;
		}
		(*(int**)0x2670544)[8] = (int)v12;
	}
	return v13;
}

const void *const loc_727F34 = (void*)0x727F34;
const void *const loc_727EA8 = (void*)0x727EA8;
__declspec(naked) void loc_727E5B()
{
	__asm
	{
		push esi
			call loc_727E5B_i
			add esp, 4
			test eax, eax
			jz lab
			xor ecx, ecx
			jmp loc_727EA8
lab:
		jmp loc_727F34
	}
}

static const void *const loc_6C6431 = (void*)0x6C6431;
static const void *const loc_6C6412 = (void*)0x6C6412;
__declspec(naked) void loc_6C63E7()
{
	__asm
	{
		mov eax, MissionNum
			cmp	long ptr [eax], 1
			je	_loc_6C6412
			mov	eax, [ebp+8]
		cdq
			mov	esi, 3
			idiv	esi
			cmp	edx, 1
			je _loc_6C6431
			mov	eax, CurrentLevel
			mov eax, [eax]
		cmp	ax, LevelIDs_PumpkinHill
			je	short loc_6C659A
			cmp	ax, LevelIDs_AquaticMine
			je	short loc_6C659A
			cmp	ax, LevelIDs_SecurityHall
			je	short loc_6C659A
			cmp	ax, LevelIDs_WildCanyon
			je	short loc_6C659A
			cmp	ax, LevelIDs_DryLagoon
			je	short loc_6C659A
			cmp	ax, LevelIDs_DeathChamber
			je	short loc_6C659A
			cmp	ax, LevelIDs_EggQuarters
			je	short loc_6C659A
			cmp	ax, LevelIDs_MeteorHerd
			je	short loc_6C659A
			cmp	ax, LevelIDs_WildCanyon2P
			je	short loc_6C659A
			cmp	ax, LevelIDs_MadSpace
			je	short loc_6C659A
			cmp	ax, LevelIDs_DryLagoon2P
			je	short loc_6C659A
			cmp	ax, LevelIDs_PoolQuest
			je	short loc_6C659A
			cmp	ax, LevelIDs_PlanetQuest
			je	short loc_6C659A
			cmp	ax, LevelIDs_DeathChamber2P
			jne	_loc_6C6431

loc_6C659A:
		pop	esi
			pop	ebp
			pop	ebx
			retn
_loc_6C6412:
		mov ecx, 2
			jmp loc_6C6412
_loc_6C6431:
		mov ecx, 2
			jmp [loc_6C6431]
	}
}

static const void *const Knuckles_LevelBounds_o = (void*)0x737B50;
__declspec(naked) void Knuckles_LevelBounds_r()
{
	__asm
	{
		mov	ebx, [CurrentLevel]
		mov ebx, [ebx]
		cmp	bx, LevelIDs_PumpkinHill
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_AquaticMine
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_SecurityHall
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_WildCanyon
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_DryLagoon
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_DeathChamber
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_EggQuarters
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_MeteorHerd
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_WildCanyon2P
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_MadSpace
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_DryLagoon2P
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_PoolQuest
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_PlanetQuest
		je	j_Knuckles_LevelBounds
		cmp	bx, LevelIDs_DeathChamber2P
		je	j_Knuckles_LevelBounds
		xor	eax, eax
		retn
j_Knuckles_LevelBounds:
		jmp [Knuckles_LevelBounds_o]
	}
}

DataPointer(NJS_TEXLIST, stru_1738D90, 0x1738D90);
DataPointer(NJS_TEXLIST, stru_1738DB0, 0x1738DB0);
void LoadTitleCardTextures()
{
	char *v15; // esi@19
	char filename[24]; // [sp+Ch] [bp-20h]@27

	if ( TwoPlayerMode || CurrentLevel == LevelIDs_Route101280 )
	{
		switch ( CurrentCharacter )
		{
		case Characters_Knuckles:
		case Characters_Rouge:
			v15 = (char*)"KN";
			break;
		case Characters_Tails:
		case Characters_Eggman:
		case Characters_MechTails:
		case Characters_MechEggman:
			v15 = (char*)"TA";
			break;
		default:
			v15 = (char*)"SO";
			break;
		}
	}
	else
	{
		if ( CurrentLevel == LevelIDs_CannonsCoreT )
			goto LABEL_26;
		switch ( MainCharObj2[0]->CharID2 )
		{
		case Characters_Shadow:
		case Characters_SuperShadow:
			v15 = (char*)"SH";
			break;
		case Characters_Eggman:
		case Characters_MechEggman:
			v15 = (char*)"EG";
			break;
		case Characters_Tails:
		case Characters_MechTails:
			v15 = (char*)"TA";
			break;
		case Characters_Knuckles:
			v15 = (char*)"KN";
			break;
		case Characters_Rouge:
			v15 = (char*)"RO";
			break;
		case Characters_Amy:
			v15 = (char*)"AM";
			break;
		case Characters_MetalSonic:
			v15 = (char*)"ME";
			break;
		case Characters_Tikal:
			v15 = (char*)"TI";
			break;
		case Characters_Chaos:
			v15 = (char*)"C0";
			break;
		case Characters_ChaoWalker:
			v15 = (char*)"CH";
			break;
		case Characters_DarkChaoWalker:
			v15 = (char*)"DC";
			break;
		default:
LABEL_26:
			v15 = (char*)"SO";
			break;
		}
	}
	sprintf_s(filename, "MISSIONTEX_%s", v15);
	LoadTextureList(filename, &stru_1738D90);
	sprintf_s(filename, "MISSIONTEX_%s2", v15);
	LoadTextureList(filename, &stru_1738DB0);
}

static const void *const loc_472B12 = (void*)0x472B12;
__declspec(naked) void loc_472A7D()
{
	LoadTitleCardTextures();
	__asm jmp loc_472B12
}

DataArray(char, byte_174AFF5, 0x174AFF5, 2);
void PlayEndLevelVoice(int playerNum)
{
	bool v3;
	__int16 v6; // cx@12
	CharObj2Base *v14; // edx@29
	bool v15; // eax@30
	int v16; // eax@42
	bool v17; // eax@62

	if (!HaveChaoKey)
		*(int *)AltCostume = 0;
	v3 = !playerNum;
	v6 = CurrentLevel;
	v14 = MainCharObj2[playerNum];
	if ( TwoPlayerMode )
	{
		v15 = byte_174AFF5[playerNum] != 1;
		switch ( v14->CharID2 )
		{
		case Characters_Sonic:
			if ( byte_174AFF5[playerNum] == 1 )
				v16 = 1529;
			else
				v16 = (char)(MainCharObj2[v3]->CharID2 - 1) != 0 ? 1613 : 1531;
			break;
		case Characters_Shadow:
			if ( byte_174AFF5[playerNum] == 1 )
				v16 = MainCharObj2[v3]->CharID2 != 0 ? 1604 : 1528;
			else
				v16 = 1530;
			break;
		case Characters_Knuckles:
			v16 = 2 * (v15 != 0) + 1254;
			break;
		case Characters_Rouge:
			v16 = 2 * (v15 != 0) + 1255;
			break;
		case Characters_MechTails:
			if ( byte_174AFF5[playerNum] == 1 )
				v16 = 1643;
			else
				v16 = (((MainCharObj2[v3]->CharID2 != Characters_MechEggman) - 1) & 0xFFFFFFBA) + 1715;
			break;
		case Characters_MechEggman:
			if ( byte_174AFF5[playerNum] != 1 || *(char*)0x174AFD2 )
				v16 = 1644;
			else
				v16 = 1642;
			break;
		case Characters_Amy:
			if ( byte_174AFF5[playerNum] == 1 )
			{
				if ( RingCount[playerNum] <= 100 )
					v16 = 8 * (RingCount[v3] < 20) + 2670;
				else
					v16 = 2680;
			}
			else
			{
				if ( RingCount[playerNum] <= 100 )
				{
					v17 = RingCount[v3] < 20;
					v17 = RingCount[v3] >= 20;
					v16 = ((v17 - 1) & 6) + 2673;
				}
				else
				{
					v16 = 2681;
				}
			}
			break;
		case Characters_Tikal:
			v16 = 2716;
			break;
		case Characters_Chaos:
			v16 = 2687;
			break;
		case Characters_MetalSonic:
			v16 = 2712;
			break;
		case Characters_ChaoWalker:
			v16 = 2692;
			break;
		case Characters_DarkChaoWalker:
			v16 = 2703;
			break;
		default:
			return;
		}
	}
	else
	{
		switch ( v14->CharID2 )
		{
		case Characters_Sonic:
			switch ( v6 )
			{
			case LevelIDs_BigFoot:
				v16 = 1605;
				break;
			case LevelIDs_SonicVsShadow1:
				v16 = 1606;
				break;
			case LevelIDs_EggGolemS:
				v16 = 1607;
				break;
			case LevelIDs_SonicVsShadow2:
				v16 = 1608;
				break;
			default:
				v16 = 1593;
				break;
			}
			break;
		case Characters_SuperSonic:
			v16 = (((v6 == LevelIDs_FinalHazard) - 1) & 0xFFFFFFEF) + 1610;
			break;
		case Characters_Shadow:
			switch ( v6 )
			{
			case LevelIDs_HotShot:
				v16 = 1594;
				break;
			case LevelIDs_SonicVsShadow1:
				v16 = 1596;
				break;
			case LevelIDs_SonicVsShadow2:
				v16 = 1598;
				break;
			case LevelIDs_Biolizard:
				v16 = 1600;
				break;
			default:
				v16 = 1552;
				break;
			}
			break;
		case Characters_SuperShadow:
			v16 = (((v6 == LevelIDs_FinalHazard) - 1) & 0xFFFFFFCE) + 1602;
			break;
		case Characters_Knuckles:
			if ( v6 == LevelIDs_KnucklesVsRouge )
				v16 = 1344;
			else
				v16 = v6 != LevelIDs_KingBoomBoo ? 1330 : 1342;
			break;
		case Characters_Rouge:
			if ( v6 == LevelIDs_KnucklesVsRouge )
				v16 = 1345;
			else
				v16 = v6 != LevelIDs_FlyingDog ? 1331 : 1343;
			break;
		case Characters_MechTails:
			v16 = (((v6 == LevelIDs_TailsVsEggman1) - 1) & 0xFFFFFFF4) + 1715;
			break;
		case Characters_MechEggman:
			v16 = (((v6 == LevelIDs_TailsVsEggman1) - 1) & 0x3A) + 1642;
			break;
		case Characters_Amy:
			v16 = 2679;
			break;
		case Characters_Tikal:
			v16 = 2716;
			break;
		case Characters_Chaos:
			v16 = 2687;
			break;
		case Characters_MetalSonic:
			v16 = 2712;
			break;
		case Characters_ChaoWalker:
			v16 = 2692;
			break;
		case Characters_DarkChaoWalker:
			v16 = 2703;
			break;
		default:
			return;
		}
	}
	PlayVoice(2, v16);
}

static const void *const loc_43F1FC = (void*)0x43F1FC;
__declspec(naked) void loc_43EE5F()
{
	__asm
	{
		push esi
			call PlayEndLevelVoice
			pop esi
			jmp loc_43F1FC
	}
}

void LoadAquaticMineCharAnims_r()
{
	if (CurrentCharacter == Characters_Knuckles)
		LoadAquaticMineCharAnims();
	else
		LoadDryLagoon2PPoolQuestCharAnims();
}

void LoadDryLagoonCharAnims_r()
{
	if (CurrentCharacter == Characters_Rouge)
		LoadDryLagoonCharAnims();
	else
		LoadDryLagoon2PPoolQuestCharAnims();
}

void LoadCannonsCoreRCharAnims_r()
{
	if (CurrentCharacter == Characters_Rouge)
		LoadCannonsCoreRCharAnims();
	else
		LoadDryLagoon2PPoolQuestCharAnims();
}

void LoadCannonsCoreKCharAnims_r()
{
	if (CurrentCharacter == Characters_Knuckles)
		LoadCannonsCoreKCharAnims();
	else
		LoadDryLagoon2PPoolQuestCharAnims();
}

void LoadSandOceanCharAnims_r()
{
	if (CurrentCharacter == Characters_MechEggman)
		LoadSandOceanCharAnims();
	else
		LoadSandOcean2PCharAnims();
}

void LoadHiddenBaseCharAnims_r()
{
	if (CurrentCharacter == Characters_MechTails)
		LoadHiddenBaseCharAnims();
	else
		LoadSandOcean2PCharAnims();
}

void LoadEggGolemECharAnims_r()
{
	if (CurrentCharacter == Characters_MechEggman)
		LoadEggGolemECharAnims();
	else
		LoadSandOcean2PCharAnims();
}

static const int loc_72F528 = 0x72F528;
static const int loc_72F4DB = 0x72F4DB;
static NJS_OBJECT **const KnucklesSunglassesModel = &CharacterModels[161].Model;
__declspec(naked) void KnucklesSunglassesFix()
{
	__asm
	{
		mov eax, KnucklesSunglassesModel
			mov eax, [eax]
		test eax, eax
			jz label
			jmp loc_72F4DB
label:
		jmp loc_72F528
	}
}

static const int loc_72F564 = 0x72F564;
static const int loc_72F537 = 0x72F537;
static NJS_OBJECT **const KnucklesAirNecklaceModel = &CharacterModels[168].Model;
__declspec(naked) void KnucklesAirNecklaceFix()
{
	__asm
	{
		mov ecx, KnucklesAirNecklaceModel
			mov ecx, [ecx]
		test ecx, ecx
			jz label
			jmp loc_72F537
label:
		jmp loc_72F564
	}
}

static const int loc_74496B = 0x74496B;
static const int loc_74491A = 0x74491A;
static NJS_OBJECT **const EggmanLaserBlasterModel = &CharacterModels[260].Model;
__declspec(naked) void EggmanLaserBlasterFix()
{
	__asm
	{
		mov edx, EggmanLaserBlasterModel
			mov edx, [edx]
		test edx, edx
			jz label
			jmp loc_74491A
label:
		jmp loc_74496B
	}
}

static const int loc_744E59 = 0x744E59;
static const int loc_744E08 = 0x744E08;
static NJS_OBJECT **const EggmanLargeCannonModel1 = &CharacterModels[262].Model;
static NJS_OBJECT **const EggmanLargeCannonModel2 = &CharacterModels[263].Model;
static const float *const flt_8AF004 = (const float *)0x8AF004;
__declspec(naked) void EggmanLargeCannonFix()
{
	__asm
	{
		mov eax, EggmanLargeCannonModel1
			mov eax, [eax]
		test eax, eax
			jz label
			mov eax, EggmanLargeCannonModel2
			mov eax, [eax]
		test eax, eax
			jz label
			mov eax, flt_8AF004
			fld [eax]
		jmp loc_744E08
label:
		jmp loc_744E59
	}
}

static const int loc_7481C3 = 0x7481C3;
static const int loc_74816E = 0x74816E;
static NJS_OBJECT **const TailsLaserBlasterModel = &CharacterModels[306].Model;
__declspec(naked) void TailsLaserBlasterFix()
{
	__asm
	{
		mov edx, TailsLaserBlasterModel
			mov edx, [edx]
		test edx, edx
			jz label
			jmp loc_74816E
label:
		jmp loc_7481C3
	}
}

static const int loc_748717 = 0x748717;
static const int loc_748620 = 0x748620;
static NJS_OBJECT **const TailsBazookaModel = &CharacterModels[309].Model;
__declspec(naked) void TailsBazookaFix()
{
	__asm
	{
		mov esi, TailsBazookaModel
			mov esi, [esi]
		test esi, esi
			jz label
			jmp loc_748620
label:
		jmp loc_748717
	}
}

static const int loc_4EB2B5 = 0x4EB2B5;
__declspec(naked) void InitSplitscreenPlus()
{
	__asm
	{
		mov eax, [MenuPressedButtons]
		mov eax, [eax+4]
		and eax, Buttons_Start
		jz label
		mov dword ptr [esp+4], 2
label:
		push    ebx
		xor     ebx, ebx
		push    esi
		push    edi
		jmp loc_4EB2B5
	}
}

AnimationInfo TailsAnimList2[ChaosAnimList_Length];
AnimationInfo MechEggmanAnimList2[ChaosAnimList_Length];
AnimationInfo MechTailsAnimList2[ChaosAnimList_Length];
AnimationInfo ChaoWalkerAnimList2[ChaosAnimList_Length];
AnimationInfo DarkChaoWalkerAnimList2[ChaosAnimList_Length];
AnimationInfo EggmanAnimList2[ChaosAnimList_Length];
AnimationInfo SonicAnimList2[ChaosAnimList_Length];

pair<int, int> listend = { -1, 0 };
template <size_t N>
void actionlistthing(pair<int, int> *(&order)[N], void **ptr, bool skipmagichands)
{
	vector<pair<int, int>> tmp;
	for (size_t i = 0; i < LengthOfArray(order); i++)
	{
		while (order[i]->key != -1)
		{
			if (skipmagichands && order[i]->key == 74)
				goto next;
			for (size_t j = 0; j < tmp.size(); j++)
				if (tmp[j].key == order[i]->key)
					goto next;
			tmp.emplace_back(*order[i]);
		next:
			++order[i];
		}
	}
	tmp.emplace_back(listend);
	pair<int, int> *buf = new pair<int, int>[tmp.size()];
	memcpy(buf, tmp.data(), tmp.size() * sizeof(pair<int, int>));
	WriteData(ptr, (void*)buf);
}

static string trim(const string &s)
{
	auto st = s.find_first_not_of(' ');
	if (st == string::npos)
		st = 0;
	auto ed = s.find_last_not_of(' ');
	if (ed == string::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

static const unordered_map<string, uint8_t> charnamemap = {
	{ "sonic", Characters_Sonic },
	{ "shadow", Characters_Shadow },
	{ "tails", Characters_Tails },
	{ "eggman", Characters_Eggman },
	{ "knuckles", Characters_Knuckles },
	{ "rouge", Characters_Rouge },
	{ "mechtails", Characters_MechTails },
	{ "mecheggman", Characters_MechEggman },
	{ "amy", Characters_Sonic | altcharacter },
	{ "metalsonic", Characters_Shadow | altcharacter },
	{ "tikal", Characters_Knuckles | altcharacter },
	{ "chaos", Characters_Rouge | altcharacter },
	{ "chaowalker", Characters_MechTails | altcharacter },
	{ "darkchaowalker", Characters_MechEggman | altcharacter },
	{ "sonicalt", Characters_Sonic | altcostume },
	{ "shadowalt", Characters_Shadow | altcostume },
	{ "knucklesalt", Characters_Knuckles | altcostume },
	{ "rougealt", Characters_Rouge | altcostume },
	{ "mechtailsalt", Characters_MechTails | altcostume },
	{ "mecheggmanalt", Characters_MechEggman | altcostume }
};

static uint8_t ParseCharacterID(const string &str, Characters def)
{
	string s = trim(str);
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	auto ch = charnamemap.find(s);
	if (ch != charnamemap.end())
		return ch->second;
	return def;
}

const string charnames[Characters_Amy] = { "Sonic", "Shadow", "Tails", "Eggman", "Knuckles", "Rouge", "MechTails", "MechEggman" };
