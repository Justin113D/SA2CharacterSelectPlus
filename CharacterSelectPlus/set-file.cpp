#include "stdafx.h"

static Trampoline* LoadStageSetFile_t = nullptr;

static inline void* LoadStageSETFile_origin(char* filename, int buffersize)
{
	auto target = LoadStageSetFile_t->Target();
	void* result;

	__asm
	{
		push[buffersize]
		mov ecx, [filename]
		call target
		add esp, 4
		mov result, eax
	}

	return result;
}

static void* LoadStageSetFile_r(char* filename, int size)
{
	std::string setS = "\\set\\set00";
	std::string setU = setS;

	if (CurrentCharacter == Characters_Knuckles || CurrentCharacter == Characters_Rouge || MissionNum == 2 || MissionNum >= 4)
	{
		return LoadStageSETFile_origin(filename, size);
	}

	switch (CurrentLevel)
	{
	case LevelIDs_WildCanyon:
		setS += "16_s.bin";
		setU += "16_u.bin";
		break;
	case LevelIDs_PumpkinHill:
		setS += "05_s.bin";
		setU += "05_u.bin";
		break;
	case LevelIDs_AquaticMine:
		setS += "07_s.bin";
		setU += "07_u.bin";
		break;
	case LevelIDs_MeteorHerd:
		setS += "32_s.bin";
		setU = "set0032_u.bin";
		break;
	case LevelIDs_DryLagoon:
		setS += "18_s.bin";
		setU += "18_u.bin";
		break;
	case LevelIDs_EggQuarters:
		setS += "26_s.bin";
		setU += "26_u.bin";
		break;
	case LevelIDs_SecurityHall:
		setS += "08_s.bin";
		setU += "08_u.bin";
		break;
	case LevelIDs_MadSpace:
		setS += "44_s.bin";
		setU = "set0044_u.bin";
		break;
	default:
		return LoadStageSETFile_origin(filename, size);
	}

	return LoadSETFile(size, (char*)setS.c_str(), (char*)setU.c_str());
}


static void __declspec(naked) LoadStageSETFileASM()
{
	__asm
	{
		push[esp + 04h] // size
		push ecx // filename
		call LoadStageSetFile_r

		pop ecx // filename
		add esp, 4 // size
		retn
	}
}

void init_SetFiles()
{
	if (!customSet)
		return;

	LoadStageSetFile_t = new Trampoline((int)0x488F60, (int)0x488F66, LoadStageSETFileASM);
}