#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "Base.h"
#include <cstdio>
#include <vector>
#include <algorithm>

using std::vector;
using std::string;
using std::unordered_map;
using std::transform;

static Trampoline* GoalRing_t = nullptr;

#pragma region Cutscene stuff

LevelCutscene* const stru_173A808 = (LevelCutscene*)0x173A808;
signed int __cdecl sub_458970()
{
	signed int v0; // ecx@3
	unsigned int v1; // eax@6

	if (MainCharObj2[0])
		v0 = MainCharObj2[0]->CharID2;
	else
		v0 = -1;
	if (v0 >= Characters_Amy) return 0;
	if (AltCostume[0]) return 0;
	if (MainCharObj2[1] && MainCharObj2[1]->CharID2 >= Characters_Amy) return 0;
	if (MainCharObj2[1] && AltCostume[1]) return 0;
	if (*(char*)0x1DEB321 && *(char*)0x1DEB320)
	{
		v1 = 0;
		while (stru_173A808[v1].Level != (signed __int16)CurrentLevel
			|| stru_173A808[v1].Character != v0)
		{
			v1++;
			if (v1 >= 15)
				return 0;
		}
		return 1;
	}
	else
		return 0;
}

#pragma endregion
#pragma region Somersault fixes

// Fix 1
#define Texlist_SonEff 0xA08B94
#define Texlist_ShadEff 0xA08D94
#define Texlist_AmyEff 0xA08F94
#define Texlist_MetEff 0xA0917C
const int loc_75783C = 0x75783C;
__declspec(naked) void __cdecl sub_757810()
{
	__asm
	{
		mov     eax, [esp + 4]
		mov     eax, [eax]ObjectMaster.Data2
		movsx   ecx, [eax].CharID
		mov     edx, MainCharObj2
		mov edx, [edx + ecx * 4]
		mov ecx, 0xA0B3B8
		mov dl, [edx].CharID2
		cmp     dl, Characters_Sonic
		jnz NotSonic
		mov     dword ptr[ecx], Texlist_SonEff
		jmp loc_75783C

		NotSonic :
		cmp     dl, Characters_Shadow
			jnz NotShadow
			mov     dword ptr[ecx], Texlist_ShadEff
			jmp loc_75783C

			NotShadow :
		cmp     dl, Characters_Amy
			jnz NotAmy
			mov     dword ptr[ecx], Texlist_AmyEff
			jmp loc_75783C

			NotAmy :
		mov     dword ptr[ecx], Texlist_MetEff
			jmp loc_75783C
	}
}

// Fix 2

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
		mov	dword ptr[ecx + 20h], Texlist_SonEff
		jmp	loc_759A3C

		NotSonic :
		cmp	eax, Characters_Shadow
			jne	short NotShadow
			mov	dword ptr[ecx + 20h], Texlist_ShadEff
			jmp	loc_759A3C

			NotShadow :
		cmp	eax, Characters_Amy
			jne	short NotAmy
			mov	dword ptr[ecx + 20h], Texlist_AmyEff
			jmp	loc_759A3C

			NotAmy :
		mov	dword ptr[ecx + 20h], Texlist_MetEff
			jmp	loc_759A3C
	}
}

#pragma endregion
#pragma region Start Positions

#pragma warning(disable : 4838)
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
#pragma warning(default : 4838)

StartPosition* SonicStartList[] = { SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, KnucklesStart, RougeStart, SuperSonicStart, SuperShadowStart, TailsStart };
StartPosition* ShadowStartList[] = { ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, RougeStart, KnucklesStart, SuperShadowStart, SuperSonicStart, TailsStart };
StartPosition* MechTailsStartList[] = { MechTailsStart, MechEggmanStart, SonicStart, ShadowStart, KnucklesStart, RougeStart, SuperSonicStart, SuperShadowStart, TailsStart };
StartPosition* MechEggmanStartList[] = { MechEggmanStart, MechTailsStart, ShadowStart, SonicStart, RougeStart, KnucklesStart, SuperShadowStart, SuperSonicStart, TailsStart };
StartPosition* KnucklesStartList[] = { KnucklesStart, RougeStart, SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, SuperSonicStart, SuperShadowStart, TailsStart };
StartPosition* RougeStartList[] = { RougeStart, KnucklesStart, ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, SuperShadowStart, SuperSonicStart, TailsStart };
StartPosition* SuperSonicStartList[] = { SuperSonicStart, SuperShadowStart, SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, KnucklesStart, RougeStart, TailsStart };
StartPosition* SuperShadowStartList[] = { SuperShadowStart, SuperSonicStart, ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, RougeStart, KnucklesStart, TailsStart };
StartPosition* TailsStartList[] = { TailsStart, SonicStart, ShadowStart, MechTailsStart, MechEggmanStart, KnucklesStart, RougeStart, SuperSonicStart, SuperShadowStart };
StartPosition* EggmanStartList[] = { TailsStart, ShadowStart, SonicStart, MechEggmanStart, MechTailsStart, RougeStart, KnucklesStart, SuperShadowStart, SuperSonicStart };

int __cdecl LoadStartPosition_ri(int playerNum, NJS_VECTOR* position, Rotation* rotation)
{
	ObjectMaster* v1; // eax@1
	CharObj2Base* v4; // eax@7
	StartPosition** list;
	StartPosition* v5; // eax@9
	int v6; // edx@25
	NJS_VECTOR* v8; // edx@35

	v1 = MainCharacter[playerNum];
	if (position)
	{
		position->z = 0.0;
		position->y = 0.0;
		position->x = 0.0;
	}
	if (rotation)
	{
		rotation->z = 0;
		rotation->y = 0;
		rotation->x = 0;
	}
	if (v1)
	{
		v4 = MainCharObj2[playerNum];
		if (v4)
		{
			switch (v4->CharID)
			{
			case Characters_Sonic:
				list = SonicStartList;
				break;
			case Characters_Shadow:
				list = ShadowStartList;
				break;
			case Characters_Knuckles:
				list = KnucklesStartList;
				break;
			case Characters_Rouge:
				list = RougeStartList;
				break;
			case Characters_Tails:
				list = TailsStartList;
				break;
			case Characters_Eggman:
				list = EggmanStartList;
				break;
			case Characters_MechEggman:
				list = MechEggmanStartList;
				break;
			case Characters_MechTails:
				list = MechTailsStartList;
				break;
			case Characters_SuperSonic:
				list = SuperSonicStartList;
				break;
			case Characters_SuperShadow:
				list = SuperShadowStartList;
				break;
			default:
				return 1;
			}
		}
		else
			return 1;
		if (TwoPlayerMode
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow1
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
			|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge)
			v6 = (playerNum != 0) + 1;
		else
			v6 = 0;
		for (int i = 0; i < (int)LengthOfArray(SonicStartList); i++)
		{
			v5 = list[i];
			if (v5)
			{
				while (v5->Level != LevelIDs_Invalid)
				{
					if (v5->Level == (short)CurrentLevel)
					{
						if (rotation)
							rotation->y = *(&v5->Rotation1P + v6);
						if (position)
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
		mov eax, [esp + 4]
		push eax
		push edi
		push ecx
		call LoadStartPosition_ri
		add esp, 12
		retn
	}
}

#pragma endregion
#pragma region Start2 Positions

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
#pragma warning(default : 4838)

StartPosition* SonicStartList2[] = { SonicStart2, ShadowStart2, MechTailsStart2, MechEggmanStart2, KnucklesStart2, RougeStart2, SuperSonicStart2, SuperShadowStart2 };
StartPosition* ShadowStartList2[] = { ShadowStart2, SonicStart2, MechEggmanStart2, MechTailsStart2, RougeStart2, KnucklesStart2, SuperShadowStart2, SuperSonicStart2 };
StartPosition* MechTailsStartList2[] = { MechTailsStart2, MechEggmanStart2, SonicStart2, ShadowStart2, KnucklesStart2, RougeStart2, SuperSonicStart2, SuperShadowStart2 };
StartPosition* MechEggmanStartList2[] = { MechEggmanStart2, MechTailsStart2, ShadowStart2, SonicStart2, RougeStart2, KnucklesStart2, SuperShadowStart2, SuperSonicStart2 };
StartPosition* KnucklesStartList2[] = { KnucklesStart2, RougeStart2, SonicStart2, ShadowStart2, MechTailsStart2, MechEggmanStart2, SuperSonicStart2, SuperShadowStart2 };
StartPosition* RougeStartList2[] = { RougeStart2, KnucklesStart2, ShadowStart2, SonicStart2, MechEggmanStart2, MechTailsStart2, SuperShadowStart2, SuperSonicStart2 };
StartPosition* SuperSonicStartList2[] = { SuperSonicStart2, SuperShadowStart2, SonicStart2, ShadowStart2, MechTailsStart2, MechEggmanStart2, KnucklesStart2, RougeStart2 };
StartPosition* SuperShadowStartList2[] = { SuperShadowStart2, SuperSonicStart2, MechEggmanStart2, MechTailsStart2, RougeStart2, KnucklesStart2, ShadowStart2, SonicStart2 };

#pragma endregion
#pragma region End Positions

#pragma warning(disable : 4838)
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

LevelEndPosition* SonicEndList[] = { SonicEnd, ShadowEnd, MechTailsEnd, MechEggmanEnd, KnucklesEnd, RougeEnd };
LevelEndPosition* ShadowEndList[] = { ShadowEnd, SonicEnd, MechEggmanEnd, MechTailsEnd, RougeEnd, KnucklesEnd };
LevelEndPosition* MechTailsEndList[] = { MechTailsEnd, MechEggmanEnd, SonicEnd, ShadowEnd, KnucklesEnd, RougeEnd };
LevelEndPosition* MechEggmanEndList[] = { MechEggmanEnd, MechTailsEnd, ShadowEnd, SonicEnd, RougeEnd, KnucklesEnd };
LevelEndPosition* KnucklesEndList[] = { KnucklesEnd, RougeEnd, SonicEnd, ShadowEnd, MechTailsEnd, MechEggmanEnd };
LevelEndPosition* RougeEndList[] = { RougeEnd, KnucklesEnd, ShadowEnd, SonicEnd, MechEggmanEnd, MechTailsEnd };

static const void* const sub_46DC70Ptr = (void*)0x46DC70;
void sub_46DC70(int a1, NJS_VECTOR* a2, char a3)
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
DataPointer(void*, off_1DE95E0, 0x1DE95E0);

signed int sub_46DBD0(int a1)
{
	CharObj2Base* v1; // eax@1
	signed int result; // eax@2

	v1 = MainCharObj2[a1];
	if (v1)
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
	EntityData1* v4; // esi@12
	LevelEndPosition** list;
	LevelEndPosition* v5; // eax@13
	int v8; // edi@24
	int v9; // ecx@24
	NJS_VECTOR* v10; // eax@24
	float v11; // ST14_4@24

	v1 = playerNum;
	v5 = &SonicEnd[0];
	if (TwoPlayerMode
		|| (v2 = CurrentLevel, (short)CurrentLevel == LevelIDs_SonicVsShadow1)
		|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
		|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
		|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
		|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge
		|| (short)CurrentLevel > LevelIDs_BigFoot && (short)CurrentLevel != LevelIDs_Route101280
		|| MissionNum != 1 && MissionNum != 2)
		return 0;
	else
	{
		v3 = MissionNum == 1 ? 0 : 1;
		v4 = MainCharacter[playerNum]->Data1.Entity;
		switch (sub_46DBD0(playerNum))
		{
		case Characters_Sonic:
		case Characters_Tails:
		case Characters_SuperSonic:
			list = SonicEndList;
			break;
		case Characters_Shadow:
		case Characters_Eggman:
		case Characters_SuperShadow:
			list = ShadowEndList;
			break;
		case Characters_Knuckles:
			list = KnucklesEndList;
			break;
		case Characters_Rouge:
			list = RougeEndList;
			break;
		case Characters_MechEggman:
			list = MechEggmanEndList;
			break;
		case Characters_MechTails:
			list = MechTailsEndList;
			break;
		default:
			return 0;
		}
		for (int i = 0; i < (int)LengthOfArray(SonicEndList); i++)
		{
			v5 = list[i];
			while (v5->Level != LevelIDs_Invalid)
			{
				if (v5->Level == v2)
					goto endloop;
				++v5;
			}
		}
	endloop:
		if (*(&v5->Mission2YRotation + v3) == 0xFFFF)
			return 0;
		v4->Rotation.z = 0;
		v4->Rotation.x = 0;
		v9 = *(&v5->Mission2YRotation + v3);
		v4->Rotation.y = v9;
		*((int*)*(&off_1DE95E0 + v1) + 7) = v9;
		v10 = (NJS_VECTOR*)((char*)&v5->Mission2Position + 12 * v3);
		v4->Position = *v10;
		v11 = v4->Position.y - 10.0f;
		*(float*)&MainCharObj2[v1]->SurfaceInfo.BottomSurfaceDist = v11;
		MainCharObj2[playerNum]->SomeVectors[0].x = 0;
		sub_46DC70(v1, &v4->Position, 0);
		v4->Collision->CollisionArray->push |= 0x70u;
		*(int*)&MainCharObj2[v1]->CurrentSurfaceFlags = 0;
		v8 = v1 & 1;
		if ((short)CurrentLevel == LevelIDs_LostColony)
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

void __cdecl SetEndLevelPosition_i(int playerNum)
{
	int v1; // edi@1
	ObjectMaster* v2; // esi@1
	CharObj2Base* v3; // eax@3
	EntityData1* v4; // esi@3
	StartPosition** list;
	StartPosition* v5; // eax@5
	int v6; // edx@20
	int v8; // ecx@30
	float v10; // ST14_4@30

	v1 = playerNum;
	v2 = MainCharacter[playerNum];
	if (v2 && LoadEndPosition_r(playerNum) != 1)
	{
		v3 = MainCharObj2[v1];
		v4 = v2->Data1.Entity;
		if (v3)
		{
			switch (v3->CharID)
			{
			case Characters_Sonic:
			case Characters_Tails:
				list = SonicStartList2;
				break;
			case Characters_Shadow:
			case Characters_Eggman:
				list = ShadowStartList2;
				break;
			case Characters_Knuckles:
				list = KnucklesStartList2;
				break;
			case Characters_Rouge:
				list = RougeStartList2;
				break;
			case Characters_MechEggman:
				list = MechEggmanStartList2;
				break;
			case Characters_MechTails:
				list = MechTailsStartList2;
				break;
			case Characters_SuperSonic:
				list = SuperSonicStartList2;
				break;
			case Characters_SuperShadow:
				list = SuperShadowStartList2;
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
		if (TwoPlayerMode
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow1
			|| (short)CurrentLevel == LevelIDs_SonicVsShadow2
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman1
			|| (short)CurrentLevel == LevelIDs_TailsVsEggman2
			|| (short)CurrentLevel == LevelIDs_KnucklesVsRouge)
			v6 = (v1 != 0) + 1;
		else
			v6 = 0;
		if (list)
		{
			for (int i = 0; i < (int)LengthOfArray(SonicStartList2); i++)
			{
				v5 = list[i];
				while (v5->Level != LevelIDs_Invalid)
				{
					if (v5->Level == (short)CurrentLevel)
					{
						v4->Rotation.z = 0;
						v4->Rotation.x = 0;
						v8 = *(&v5->Rotation1P + v6);
						v4->Rotation.y = v8;
						*((int*)*(&off_1DE95E0 + v1) + 7) = v8;
						v4->Position = (&v5->Position1P)[v6];
						v10 = v4->Position.y - 10.0f;
						MainCharObj2[v1]->SurfaceInfo.BottomSurfaceDist = v10;
						MainCharObj2[v1]->SomeVectors[0].x = 0;
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
		*((int*)*(&off_1DE95E0 + v1) + 7) = 0;
	LABEL_27:
		sub_46DC70(v1, &v4->Position, 0);
		v4->Collision->CollisionArray->push |= 0x70u;
		MainCharObj2[v1]->CurrentSurfaceFlags = (SurfaceFlags)0x0;
		if ((short)CurrentLevel == LevelIDs_RadicalHighway || (short)CurrentLevel == LevelIDs_LostColony)
			byte_1DE4664[v1 & 1] = 5;
		else
			byte_1DE4664[v1 & 1] = *(char*)0x1DE4660;
	}
}

__declspec(naked) void SetEndLevelPosition()
{
	__asm
	{
		push eax
		call SetEndLevelPosition_i
		add esp, 4
		retn
	}
}

#pragma endregion
#pragma region 2P Intro Pos

#pragma warning(disable : 4838)
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
#pragma warning(default : 4838)

LevelEndPosition* Sonic2PIntroList[] = { Sonic2PIntro, Shadow2PIntro, MechTails2PIntro, MechEggman2PIntro, Knuckles2PIntro, Rouge2PIntro };
LevelEndPosition* Shadow2PIntroList[] = { Shadow2PIntro, Sonic2PIntro, MechEggman2PIntro, MechTails2PIntro, Rouge2PIntro, Knuckles2PIntro };
LevelEndPosition* MechTails2PIntroList[] = { MechTails2PIntro, MechEggman2PIntro, Sonic2PIntro, Shadow2PIntro, Knuckles2PIntro, Rouge2PIntro };
LevelEndPosition* MechEggman2PIntroList[] = { MechEggman2PIntro, MechTails2PIntro, Shadow2PIntro, Sonic2PIntro, Rouge2PIntro, Knuckles2PIntro };
LevelEndPosition* Knuckles2PIntroList[] = { Knuckles2PIntro, Rouge2PIntro, Sonic2PIntro, Shadow2PIntro, MechTails2PIntro, MechEggman2PIntro };
LevelEndPosition* Rouge2PIntroList[] = { Rouge2PIntro, Knuckles2PIntro, Shadow2PIntro, Sonic2PIntro, MechEggman2PIntro, MechTails2PIntro };

void __cdecl Load2PIntroPos_ri(int playerNum)
{
	ObjectMaster* v1; // eax@1
	int v2; // edi@1
	CharObj2Base* v3; // eax@2
	EntityData1* v4; // esi@2
	LevelEndPosition** list;
	LevelEndPosition* v5; // eax@4
	bool v6; // edx@11
	NJS_VECTOR* v8; // ecx@15
	CharObj2Base* v9; // eax@16
	int v10; // edi@16
	char v11; // al@16
	NJS_VECTOR* v12; // eax@20
	float v13; // ST10_4@20

	v2 = playerNum;
	v1 = MainCharacter[playerNum];
	if (v1)
	{
		v4 = v1->Data1.Entity;
		v3 = MainCharObj2[v2];
		if (v3)
		{
			switch (v3->CharID)
			{
			case Characters_Sonic:
			case Characters_Tails:
			case Characters_SuperSonic:
				list = Sonic2PIntroList;
				break;
			case Characters_Shadow:
			case Characters_Eggman:
			case Characters_SuperShadow:
				list = Shadow2PIntroList;
				break;
			case Characters_Knuckles:
				list = Knuckles2PIntroList;
				break;
			case Characters_Rouge:
				list = Rouge2PIntroList;
				break;
			case Characters_MechEggman:
				list = MechEggman2PIntroList;
				break;
			case Characters_MechTails:
				list = MechTails2PIntroList;
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
		if (list)
		{
			for (int i = 0; i < (int)LengthOfArray(Sonic2PIntroList); i++)
			{
				v5 = list[i];
				while (v5->Level != LevelIDs_Invalid)
				{
					if (v5->Level == (short)CurrentLevel)
					{
						v4->Rotation.y = *(&v5->Mission2YRotation + v6);
						v12 = (NJS_VECTOR*)((char*)&v5->Mission2Position + 12 * v6);
						v4->Position = *v12;
						v8 = &v4->Position;
						*((int*)*(&off_1DE95E0 + v2) + 7) = v4->Rotation.y;
						v13 = v4->Position.y - 10.0f;
						MainCharObj2[v2]->SurfaceInfo.BottomSurfaceDist = v13;
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
		v4->Collision->CollisionArray->push |= 0x70u;
		v11 = *(char*)0x1DE4660;
		*(int*)&MainCharObj2[v2]->CurrentSurfaceFlags = 0;
		byte_1DE4664[v2 & 1] = v11;
		v9 = MainCharObj2[v2];
		v10 = (int)*(&off_1DE95E0 + v2);
		if (v9)
		{
			v9->Speed.x = 0.0;
			v9->Speed.y = 0.0;
			v9->Speed.z = 0.0;
		}
		if (v10)
		{
			*(float*)(v10 + 8) = 0.0;
			*(float*)(v10 + 4) = 0.0;
			*(float*)v10 = 0.0;
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

#pragma endregion
#pragma region 2P Race Bar

CharObj2Base* __cdecl loc_727E5B_i(int playerNum)
{
	CharObj2Base* v13 = MainCharObj2[playerNum];
	NJS_TEXLIST* v12 = 0;
	if (v13)
	{
		switch (v13->CharID2)
		{
		case Characters_Sonic:
		case Characters_SuperSonic:
			v12 = &TexList_SonicLife;
			break;
		case Characters_Shadow:
		case Characters_SuperShadow:
			v12 = &TexList_ShadowLife;
			break;
		case Characters_Amy:
			v12 = &TexList_AmyLife;
			break;
		case Characters_MetalSonic:
			v12 = &TexList_MetalLife;
			break;
		case Characters_Tails:
		case Characters_MechTails:
			v12 = &TexList_TailsLife;
			break;
		case Characters_ChaoWalker:
			v12 = &TexList_ChaoLife;
			break;
		case Characters_DarkChaoWalker:
			v12 = &TexList_DarkChaoLife;
			break;
		case Characters_Knuckles:
			v12 = &TexList_KnucklesLife;
			break;
		case Characters_Rouge:
			v12 = &TexList_RougeLife;
			break;
		case Characters_Tikal:
			v12 = &TexList_TikalLife;
			break;
		case Characters_Chaos:
			v12 = &TexList_ChaosLife;
			break;
		default:
			return 0;
		}
		(*(int**)0x2670544)[8] = (int)v12;
	}
	return v13;
}

const void* const loc_727F34 = (void*)0x727F34;
const void* const loc_727EA8 = (void*)0x727EA8;
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
		lab :
		jmp loc_727F34
	}
}

#pragma endregion
#pragma region Goal Ring

void GoalRing_Main_r(ObjectMaster* obj)
{
	EntityData1* data = obj->Data1.Entity;

	for (uint8_t i = 0; i < StageSelectLevels_Length; i++)
	{
		if ((CurrentLevel == StageSelectLevels[i].Level) && (StageSelectLevels[i].Character == Characters_Knuckles || StageSelectLevels[i].Character == Characters_Rouge))
		{
			if (MissionNum == 1)
			{
				data->Rotation.x = 2;
				data->Action = 1;
				if (!MainCharObj1[0] || MainCharObj1[0]->Timer < 120u)
				{
					return;
				}
			}
			else if (data->Rotation.x % 3 != 1)
			{
				return;
			}
		}
	}

	ObjectFunc(origin, GoalRing_t->Target());
	origin(obj);
}

#pragma endregion
#pragma region Title Card Textures

DataPointer(NJS_TEXLIST, stru_1738D90, 0x1738D90);
DataPointer(NJS_TEXLIST, stru_1738DB0, 0x1738DB0);
void LoadTitleCardTextures()
{
	const char* v15; // esi@19
	char filename[24]; // [sp+Ch] [bp-20h]@27

	if (TwoPlayerMode || CurrentLevel == LevelIDs_Route101280)
	{
		switch (CurrentCharacter)
		{
		case Characters_Knuckles:
		case Characters_Rouge:
			v15 = "KN";
			break;
		case Characters_Tails:
		case Characters_Eggman:
		case Characters_MechTails:
		case Characters_MechEggman:
			v15 = "TA";
			break;
		default:
			v15 = "SO";
			break;
		}
	}
	else
	{
		if (CurrentLevel == LevelIDs_CannonsCoreT)
			goto LABEL_26;
		switch (MainCharObj2[0]->CharID2)
		{
		case Characters_Shadow:
		case Characters_SuperShadow:
			v15 = "SH";
			break;
		case Characters_Eggman:
		case Characters_MechEggman:
			v15 = "EG";
			break;
		case Characters_Tails:
		case Characters_MechTails:
			v15 = "TA";
			break;
		case Characters_Knuckles:
			v15 = "KN";
			break;
		case Characters_Rouge:
			v15 = "RO";
			break;
		case Characters_Amy:
			v15 = "AM";
			break;
		case Characters_MetalSonic:
			v15 = "ME";
			break;
		case Characters_Tikal:
			v15 = "TI";
			break;
		case Characters_Chaos:
			v15 = "C0";
			break;
		case Characters_ChaoWalker:
			v15 = "CH";
			break;
		case Characters_DarkChaoWalker:
			v15 = "DC";
			break;
		default:
		LABEL_26:
			v15 = "SO";
			break;
		}
	}
	sprintf_s(filename, "MISSIONTEX_%s", v15);
	LoadTextureList(filename, &stru_1738D90);
	sprintf_s(filename, "MISSIONTEX_%s2", v15);
	LoadTextureList(filename, &stru_1738DB0);
}

static const void* const loc_472B12 = (void*)0x472B12;
__declspec(naked) void loc_472A7D()
{
	LoadTitleCardTextures();
	__asm jmp loc_472B12
}

#pragma endregion
#pragma region End Level Voices

DataArray(char, byte_174AFF5, 0x174AFF5, 2);
void PlayEndLevelVoice(int playerNum)
{
	bool v3;
	__int16 v6; // cx@12
	CharObj2Base* v14; // edx@29
	bool v15; // eax@30
	int v16; // eax@42
	bool v17; // eax@62

	if (!HaveChaoKey)
		*(int*)AltCostume = 0;
	v3 = !playerNum;
	v6 = CurrentLevel;
	v14 = MainCharObj2[playerNum];
	if (TwoPlayerMode)
	{
		v15 = byte_174AFF5[playerNum] != 1;
		switch (v14->CharID2)
		{
		case Characters_Sonic:
			if (byte_174AFF5[playerNum] == 1)
				v16 = 1529;
			else
				v16 = (char)(MainCharObj2[v3]->CharID2 - 1) != 0 ? 1613 : 1531;
			break;
		case Characters_Shadow:
			if (byte_174AFF5[playerNum] == 1)
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
			if (byte_174AFF5[playerNum] == 1)
				v16 = 1643;
			else
				v16 = (((MainCharObj2[v3]->CharID2 != Characters_MechEggman) - 1) & 0xFFFFFFBA) + 1715;
			break;
		case Characters_MechEggman:
			if (byte_174AFF5[playerNum] != 1 || *(char*)0x174AFD2)
				v16 = 1644;
			else
				v16 = 1642;
			break;
		case Characters_Amy:
			if (byte_174AFF5[playerNum] == 1)
			{
				if (RingCount[playerNum] <= 100)
					v16 = 8 * (RingCount[v3] < 20) + 2670;
				else
					v16 = 2680;
			}
			else
			{
				if (RingCount[playerNum] <= 100)
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
		switch (v14->CharID2)
		{
		case Characters_Sonic:
			switch (v6)
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
			switch (v6)
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
			if (v6 == LevelIDs_KnucklesVsRouge)
				v16 = 1344;
			else
				v16 = v6 != LevelIDs_KingBoomBoo ? 1330 : 1342;
			break;
		case Characters_Rouge:
			if (v6 == LevelIDs_KnucklesVsRouge)
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

static const void* const loc_43F1FC = (void*)0x43F1FC;
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

#pragma endregion
/*
#pragma region Chao World Sounds

ThiscallFunctionPointer(void, sub_435880, (const char*), 0x435880);
void LoadChaoWorldSoundBank()
{
	const char* v4;

	int v2 = MainCharObj2[0]->CharID2;
	switch (v2)
	{
	case Characters_Amy:
	case Characters_MetalSonic:
		v4 = "chao_chara_sn.mlt";
		break;
	case Characters_Knuckles:
	case Characters_Rouge:
		v4 = "chao_chara_kr.mlt";
		break;
	case Characters_Tikal:
	case Characters_Chaos:
		v4 = "se_ch_kn_BATTLE.mlt";
		break;
	case Characters_Tails:
	case Characters_Eggman:
		v4 = "chao_chara_te.mlt";
		break;
	case Characters_MechTails:
	case Characters_MechEggman:
		v4 = "se_ch_wk.mlt";
		break;
	case Characters_ChaoWalker:
	case Characters_DarkChaoWalker:
		v4 = "se_ch_wk_BATTLE.mlt";
		break;
	default:
		v4 = "chao_chara_ss.mlt";
		break;
	}
	sub_435880(v4);
}

static const int loc_532054 = 0x532054;
__declspec(naked) void loc_532029()
{
	LoadChaoWorldSoundBank();
	__asm jmp loc_532054
}

#pragma endregion
#pragma region Chao World Voices

static const int stru_173A0B8 = 0x173A0B8;
static const int sub_459010 = 0x459010;
static const int loc_45923B = 0x45923B;
__declspec(naked) void loc_2800440()
{
	__asm
	{
		mov ecx, MainCharObj2
		movzx ecx, [ecx].CharID2
		cmp ecx, Characters_Amy
		jle	short loc_280045F
		cmp	ecx, Characters_Tikal
		jz	short loc_280045A
		cmp	ecx, Characters_Amy
		jnz	short loc_280045F
		mov	esi, stru_173A0B8
		jmp	short loc_280045F
		; -------------------------------------------------------------------------- -

		loc_280045A:
		mov	esi, (stru_173A0B8 + 20h)

			loc_280045F :
			call	sub_459010
			jmp	loc_45923B
	}
}

#pragma endregion
*/
#pragma region Upgrade Fixes

// Knuckles Sunglasses
static const int loc_72F528 = 0x72F528;
static const int loc_72F4DB = 0x72F4DB;
static NJS_OBJECT** const KnucklesSunglassesModel = &CharacterModels[161].Model;
__declspec(naked) void KnucklesSunglassesFix()
{
	__asm
	{
		mov eax, KnucklesSunglassesModel
		mov eax, [eax]
		test eax, eax
		jz label
		jmp loc_72F4DB
		label :
		jmp loc_72F528
	}
}

// Knuckles Air Necklase
static const int loc_72F564 = 0x72F564;
static const int loc_72F537 = 0x72F537;
static NJS_OBJECT** const KnucklesAirNecklaceModel = &CharacterModels[168].Model;
__declspec(naked) void KnucklesAirNecklaceFix()
{
	__asm
	{
		mov ecx, KnucklesAirNecklaceModel
		mov ecx, [ecx]
		test ecx, ecx
		jz label
		jmp loc_72F537
		label :
		jmp loc_72F564
	}
}

// Eggman Laser Blaster
static const int loc_74496B = 0x74496B;
static const int loc_74491A = 0x74491A;
static NJS_OBJECT** const EggmanLaserBlasterModel = &CharacterModels[260].Model;
__declspec(naked) void EggmanLaserBlasterFix()
{
	__asm
	{
		mov edx, EggmanLaserBlasterModel
		mov edx, [edx]
		test edx, edx
		jz label
		jmp loc_74491A
		label :
		jmp loc_74496B
	}
}

// Eggman Large Cannon
static const int loc_744E59 = 0x744E59;
static const int loc_744E08 = 0x744E08;
static NJS_OBJECT** const EggmanLargeCannonModel1 = &CharacterModels[262].Model;
static NJS_OBJECT** const EggmanLargeCannonModel2 = &CharacterModels[263].Model;
static const float* const flt_8AF004 = (const float*)0x8AF004;
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
		fld[eax]
		jmp loc_744E08
		label :
		jmp loc_744E59
	}
}

// Tails Laser Blaster
static const int loc_7481C3 = 0x7481C3;
static const int loc_74816E = 0x74816E;
static NJS_OBJECT** const TailsLaserBlasterModel = &CharacterModels[306].Model;
__declspec(naked) void TailsLaserBlasterFix()
{
	__asm
	{
		mov edx, TailsLaserBlasterModel
		mov edx, [edx]
		test edx, edx
		jz label
		jmp loc_74816E
		label :
		jmp loc_7481C3
	}
}

// Tails Bazooka
static const int loc_748717 = 0x748717;
static const int loc_748620 = 0x748620;
static NJS_OBJECT** const TailsBazookaModel = &CharacterModels[309].Model;
__declspec(naked) void TailsBazookaFix()
{
	__asm
	{
		mov esi, TailsBazookaModel
		mov esi, [esi]
		test esi, esi
		jz label
		jmp loc_748620
		label :
		jmp loc_748717
	}
}

#pragma endregion

#pragma region Emerald Manager

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

static const void* const loc_73AAC2 = (void*)0x73AAC2;
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

void LoadEmeraldManager_r_wrapper()
{
	LoadEmeraldManager_r();
}

#pragma endregion
#pragma region LevelBounds

static Trampoline* Knuckles_LevelBounds_t = nullptr;

static inline bool Knuckles_LevelBounds_origin(EntityData1* a1, KnucklesCharObj2* a2)
{
	auto target = Knuckles_LevelBounds_t->Target();

	bool result;
	__asm
	{
		mov ecx, [a2]
		mov eax, [a1]
		call target
		mov result, al
	}
	return result;
}

bool Knuckles_LevelBounds_r(EntityData1* a1, KnucklesCharObj2* a2)
{
	for (uint8_t i = 0; i < StageSelectLevels_Length; i++)
	{
		if ((CurrentLevel == StageSelectLevels[i].Level) && (StageSelectLevels[i].Character == Characters_Knuckles || StageSelectLevels[i].Character == Characters_Rouge))
		{
			return Knuckles_LevelBounds_origin(a1, a2);
		}
	}

	return 0;
}

static void __declspec(naked) Knuckles_LevelBounds_ASM()
{
	__asm
	{
		push ecx
		push eax
		call Knuckles_LevelBounds_r
		add esp, 4
		pop ecx
		retn
	}
}

#pragma endregion
#pragma region Animation Loaders

void LoadAquaticMineCharAnims_r()
{
	if (CurrentCharacter == Characters_Knuckles)
		LoadAquaticMineCharAnims();
	else
		LoadDryLagoon2PCharAnims();
}

void LoadDryLagoonCharAnims_r()
{
	if (CurrentCharacter == Characters_Rouge)
		LoadDryLagoonCharAnims();
	else
		LoadDryLagoon2PCharAnims();
}

void LoadCannonsCoreRCharAnims_r()
{
	if (CurrentCharacter == Characters_Rouge)
		LoadCannonsCoreRCharAnims();
	else
		LoadDryLagoon2PCharAnims();
}

void LoadCannonsCoreKCharAnims_r()
{
	if (CurrentCharacter == Characters_Knuckles)
		LoadCannonsCoreKCharAnims();
	else
		LoadDryLagoon2PCharAnims();
}

void LoadSandOceanCharAnims_r()
{
	if (CurrentCharacter == Characters_MechEggman)
		LoadSandOceanCharAnims();
	else if (CurrentCharacter <= Characters_Shadow && !isSonicTrickMod())
		LoadEggGolemCharAnims();
	else
		LoadSandOcean2PCharAnims();
}

void LoadHiddenBaseCharAnims_r()
{
	if (CurrentCharacter == Characters_MechTails)
		LoadHiddenBaseCharAnims();
	else if (CurrentCharacter <= Characters_Shadow && !isSonicTrickMod())
		LoadEggGolemCharAnims();
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

#pragma endregion
#pragma region Animation Lists
AnimationInfo TailsAnimList2[ChaosAnimList_Length];
AnimationInfo MechEggmanAnimList2[ChaosAnimList_Length];
AnimationInfo MechTailsAnimList2[ChaosAnimList_Length];
AnimationInfo ChaoWalkerAnimList2[ChaosAnimList_Length];
AnimationInfo DarkChaoWalkerAnimList2[ChaosAnimList_Length];
AnimationInfo EggmanAnimList2[ChaosAnimList_Length];
AnimationInfo SonicAnimList2[ChaosAnimList_Length];

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

pair<int, int> listend = { -1, 0 };

void LoadAnimations(int* character, int playerNum)
{
	int repcnt;
	pair<short, short>* replst;
	switch (*character)
	{
	case Characters_Sonic:
	default:

		LoadSonic(playerNum);
		repcnt = (int)LengthOfArray(SonicAnimReplacements);
		replst = SonicAnimReplacements;
		break;
	case Characters_Shadow:
		LoadShadow(playerNum);
		repcnt = (int)LengthOfArray(SonicAnimReplacements);
		replst = SonicAnimReplacements;
		break;
	case Characters_Tails:
		LoadTails(playerNum);
		repcnt = (int)LengthOfArray(OthersAnimReplacements);
		replst = OthersAnimReplacements;
		break;
	case Characters_Eggman:
		LoadEggman(playerNum);
		repcnt = (int)LengthOfArray(OthersAnimReplacements);
		replst = OthersAnimReplacements;
		break;
	case Characters_Knuckles:
		LoadKnuckles(playerNum);
		repcnt = (int)LengthOfArray(KnucklesAnimReplacements);
		replst = KnucklesAnimReplacements;
		break;
	case Characters_Rouge:
		LoadRouge(playerNum);
		repcnt = (int)LengthOfArray(KnucklesAnimReplacements) - 3;
		replst = KnucklesAnimReplacements;
		break;
	case Characters_MechTails:
		LoadMechTails(playerNum);
		repcnt = (int)LengthOfArray(MechAnimReplacements);
		replst = MechAnimReplacements;
		break;
	case Characters_MechEggman:
		LoadMechEggman(playerNum);
		repcnt = (int)LengthOfArray(MechAnimReplacements);
		replst = MechAnimReplacements;
		break;
	}
	InitPlayer(playerNum);
	/**AnimationInfo* anilst = MainCharObj2[playerNum]->AnimInfo.Animations;
	for (int i = 0; i < repcnt; i++)
		if (!CharacterAnimations[anilst[replst[i].key].AnimNum].Animation)
			anilst[replst[i].key] = anilst[replst[i].value];*/
}

template <size_t N>
void actionlistthing(pair<int, int>* (&order)[N], void** ptr, bool skipmagichands)
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
	pair<int, int>* buf = new pair<int, int>[tmp.size()];
	memcpy(buf, tmp.data(), tmp.size() * sizeof(pair<int, int>));
	WriteData(ptr, (void*)buf);
}

#pragma endregion

void LoadCharacterSoundBanks_r(int curChar, MLTSoundList* mltSoundList, MLTSoundList* a1)
{
	if (curChar != Characters_Tails && curChar != Characters_Eggman)
	{
		LoadCharacterSoundBanks(curChar, mltSoundList, a1);
		return;
	}

	CharacterSoundBank* SoundBank;
	char i;
	CharacterVoiceBank* soundBank2;
	char k;
	const char* v14;

	SoundBank = stru_1739F58;

	for (i = 0; i < 10; ++i)
	{
		if ((SoundBank->Character) == curChar)
		{
			break;
		}
		++SoundBank;
	}
	if (i == 10)
	{
		SoundBank = stru_1739F58;
	}

	soundBank2 = stru_173A018;
	for (k = 0; k < 10; ++k)
	{
		if (soundBank2->Character == curChar)
		{
			break;
		}
		++soundBank2;
	}
	if (k == 10)
	{
		soundBank2 = stru_173A018;
	}

	v14 = SoundBank->Name;
	if (v14)
	{
		LoadMLT(v14);
	}
	mltSoundList->Size = (int)SoundBank->SoundList;
	sub_459010(a1, soundBank2);
}

static void __declspec(naked) LoadCharacterSoundBanksASM()
{
	__asm
	{
		push[esp + 08h] // a1
		push[esp + 08h] // mltSoundList
		push ebx // curChar

		// Call your __cdecl function here:
		call LoadCharacterSoundBanks_r

		pop ebx // curChar
		add esp, 4 // mltSoundList
		add esp, 4 // a1
		retn
	}
}

void WritePatches()
{
	//fix Tails and Eggman mechless wrong sound bank
	WriteCall((void*)0x438B37, LoadCharacterSoundBanksASM);

	unsigned __int8 twobytenop[] = { 0x66, 0x90 };
	unsigned __int8 fivebytenop[] = { 0x66, 0x90, 0x66, 0x90, 0x90 };
	unsigned __int8 shortjmp[] = { 0xEB };

	WriteData((void*)0x459110, twobytenop); // 2P Sound Effects Patch
	WriteData((void*)0x45913B, twobytenop); // 2P Voice Patch
	WriteData((void*)0x44E63B, twobytenop); // Dark Chao Walker Life Icon Patch
	WriteData((void*)0x4CD255, twobytenop); // Sonic's Cannon's Core Control Patch
	WriteData((void*)0x724261, shortjmp); // Sonic Boss Special Patch
	WriteData((void*)0x736211, shortjmp); // Knuckles Boss Special Patch
	WriteData((void*)0x7374E4, shortjmp); // Dry Lagoon Turtle Grab Patch
	WriteData((void*)0x749921, shortjmp); // Mech Boss Special Patch
	WriteData((void*)0x741690, shortjmp); // Dark Chao Walker Fix
	WriteData((void*)0x7416DC, shortjmp); // Chao Walker Fix
	WriteData((void*)0x728141, fivebytenop); // Knuckles emerald manager
	WriteData((void*)0x728491, fivebytenop); // Rouge emerald manager
	WriteData((void*)0x7288B7, fivebytenop); // Tikal emerald manager
	WriteData((void*)0x728B64, fivebytenop); // Chaos emerald manager
	WriteData((void*)0x716E13, twobytenop); // Amy
	WriteData((void*)0x716F2C, twobytenop); // Sonic costume
	WriteData((void*)0x717373, twobytenop); // Metal Sonic
	WriteData((void*)0x71748C, twobytenop); // Shadow costume
	WriteData((void*)0x728123, twobytenop); // Tikal
	WriteData((void*)0x728241, twobytenop); // Knuckles costume
	WriteData((void*)0x728473, twobytenop); // Chaos
	WriteData((void*)0x728591, twobytenop); // Rouge costume
	WriteData((void*)0x740C61, twobytenop); // Dark Chao Walker
	WriteData((void*)0x740D72, twobytenop); // Eggman costume
	WriteData((void*)0x740EC1, twobytenop); // Chao Walker
	WriteData((void*)0x740FD2, twobytenop); // Tails costume
}

void WriteJumps()
{
	WriteJump((void*)0x458970, sub_458970); // Level Cutscene Function
	WriteJump((void*)0x757810, sub_757810); // Somersault Fix 1
	WriteJump((void*)0x759A18, loc_759A18); // Somersault Fix 2
	WriteJump((void*)LoadStartPositionPtr, LoadStartPosition_r); // LoadStartPosition replacement
	WriteJump((void*)0x43DF30, SetEndLevelPosition); // End position
	WriteJump((void*)Load2PIntroPos, Load2PIntroPos_r); // 2P Intro position
	WriteJump((void*)0x727E5B, loc_727E5B); // 2P Race Bar

	WriteJump((void*)0x6cff40, (void*)0x6cff83); //make all knux pieces always grabbable

	WriteJump((void*)0x43C9D0, (void*)0x43CADF); // Tails/Eggman fix
	WriteJump((void*)0x472A7D, loc_472A7D); // Title Card textures
	WriteJump((void*)0x43EE5F, loc_43EE5F); // End Level voices
	//WriteJump((void*)0x532029, loc_532029); // Chao World sounds
	//WriteJump((void*)0x459236, loc_2800440); // Chao World voices
	WriteJump((void*)0x72F4D6, KnucklesSunglassesFix);
	WriteJump((void*)0x72F531, KnucklesAirNecklaceFix);
	WriteJump((void*)0x744914, EggmanLaserBlasterFix);
	WriteJump((void*)0x744E02, EggmanLargeCannonFix);
	WriteJump((void*)0x748168, TailsLaserBlasterFix);
	WriteJump((void*)0x74861A, TailsBazookaFix);

	WriteJump((void*)0x6c63de, (void*)0x6c6431); //fix goal ring for the hunters
}

void InitBase()
{
	WritePatches();
	WriteJumps();

	// Fixing action Lists
	pair<int, int>* sonic = (pair<int, int>*)0x96EC80;
	pair<int, int>* amy = (pair<int, int>*)0x96ECD0;
	pair<int, int>* shadow = (pair<int, int>*)0x96ED18;
	pair<int, int>* knuckles = (pair<int, int>*)0x96ED58;
	pair<int, int>* tikal = (pair<int, int>*)0x96EDB8;
	pair<int, int>* rouge = (pair<int, int>*)0x96EE10;
	pair<int, int>* chaos = (pair<int, int>*)0x96EE80;
	pair<int, int>* mecheggman = (pair<int, int>*)0x96EED8;
	pair<int, int>* darkchaowalker = (pair<int, int>*)0x96EF08;
	pair<int, int>* mechtails = (pair<int, int>*)0x96EF38;
	pair<int, int>* chaowalker = (pair<int, int>*)0x96EF68;
	pair<int, int>* eggman = (pair<int, int>*)0x96EF98;
	pair<int, int>* tails = (pair<int, int>*)0x96EFA8;
	{
		pair<int, int>* order[] = { sonic, shadow, rouge, knuckles, mechtails, mecheggman };
		actionlistthing(order, (void**)0x7952E5, false);
	}
	{
		pair<int, int>* order[] = { amy, sonic, shadow, rouge, knuckles, mechtails, mecheggman };
		actionlistthing(order, (void**)0x7952EC, false);
	}
	{
		pair<int, int>* order[] = { shadow, sonic, rouge, knuckles, mechtails, mecheggman };
		actionlistthing(order, (void**)0x7952F3, false);
	}
	{
		pair<int, int>* order[] = { tails, mechtails, mecheggman, sonic, shadow, rouge, knuckles };
		actionlistthing(order, (void**)0x7952FA, true);
	}
	{
		pair<int, int>* order[] = { knuckles, rouge, sonic, shadow, mechtails, mecheggman };
		actionlistthing(order, (void**)0x795301, true);
	}
	{
		pair<int, int>* order[] = { tikal, rouge, knuckles, sonic, shadow, mechtails, mecheggman };
		actionlistthing(order, (void**)0x795308, true);
	}
	{
		pair<int, int>* order[] = { rouge, knuckles, sonic, shadow, mechtails, mecheggman };
		actionlistthing(order, (void**)0x79530F, true);
	}
	{
		pair<int, int>* order[] = { chaos, rouge, knuckles, sonic, shadow, mechtails, mecheggman };
		actionlistthing(order, (void**)0x795316, true);
	}
	{
		pair<int, int>* order[] = { eggman, mecheggman, mechtails, sonic, shadow, rouge, knuckles };
		actionlistthing(order, (void**)0x79531D, true);
	}
	{
		pair<int, int>* order[] = { mecheggman, mechtails, sonic, shadow, rouge, knuckles };
		actionlistthing(order, (void**)0x795324, true);
	}
	{
		pair<int, int>* order[] = { darkchaowalker, mecheggman, mechtails, sonic, shadow, rouge, knuckles };
		actionlistthing(order, (void**)0x79532B, true);
	}
	{
		pair<int, int>* order[] = { chaowalker, mechtails, mecheggman, sonic, shadow, rouge, knuckles };
		actionlistthing(order, (void**)0x795332, true);
	}
	{
		pair<int, int>* order[] = { mechtails, mecheggman, sonic, shadow, rouge, knuckles };
		actionlistthing(order, (void**)0x795339, true);
	}


	Knuckles_LevelBounds_t = new Trampoline((int)0x737B50, (int)0x737B5A, Knuckles_LevelBounds_ASM);
	GoalRing_t = new Trampoline((int)GoalRing_Main, (int)GoalRing_Main + 0x6, GoalRing_Main_r);

	WriteCall((void*)0x4D45F0, LoadAquaticMineCharAnims_r);
	WriteCall((void*)0x63D727, LoadDryLagoonCharAnims_r);
	WriteCall((void*)0x4DB351, LoadCannonsCoreRCharAnims_r);
	WriteCall((void*)0x65E8F1, LoadCannonsCoreKCharAnims_r);
	WriteCall((void*)0x65662A, LoadSandOceanCharAnims_r);
	WriteCall((void*)0x4DDE49, LoadHiddenBaseCharAnims_r);	//pyramid race thing
	WriteCall((void*)0x710476, LoadHiddenBaseCharAnims_r); //hidden base
	WriteCall((void*)0x4A53AC, LoadEggGolemECharAnims_r);
}