#include "stdafx.h"
#include "SA2ModLoader.h"
#include <stdio.h>
#include <array>
#include <cmath>

/*

#pragma region declaring and pointing to variables
DataArray(char*, Camera_Character_Data, 0x0173388C, 10);
DataPointer(char, CharOrMissionSelectionMax, 0x01D1BF04);

DataArray(char, CheckUnlockedChar, 0x01DEF829, 11);
DataArray(int, ChaoSelectCharacterIconIDs, 0x00C75310, 6);
DataPointer(float, StageSelect_MissionCharSelectLoc_X, 0x01A4A968);
DataPointer(float, StageSelect_MissionCharSelectLoc_Y, 0x01A4A96C);
DataPointer(float, NJS_IMAGEDATA_AlphaStrength, 0x025EFFD0);
DataPointer(float, NJS_IMAGEDATA_RedStrength, 0x025EFFD4);
DataPointer(float, NJS_IMAGEDATA_GreenStrength, 0x025EFFD8);
DataPointer(float, NJS_IMAGEDATA_BlueStrength, 0x025EFFDC);
DataArray(NJS_TEXANIM, StageMapAnims, 0x00C68C50, 104);
DataArray(NJS_TEXNAME, StageMapTexNames, 0x00C68AA8, 16);
DataArray(CharacterVoiceBank, stru_173A018, 0x0173A018, 10);

DataPointer(int, UI_State1, 0x01D7BB14);
DataPointer(int*, StageSelectFlags, 0x00C77F08);
DataPointer(int, InputDown, 0x01DEFAB0);
DataPointer(int, ShowButtonGuide, 0x01AEE2FC);
DataPointer(int, IngameFlag, 0x01A559E8);
DataPointer(char, byte_1DEB31E, 0x01DEB31E);
DataPointer(short, word_1DEB31F, 0x01DEB31F);
DataPointer(char, byte_1DEB321, 0x01DEB321);
DataPointer(float, dword_1D7BB10, 0x01D7BB10);
DataArray(int, StageUnlocked, 0x01A4A974, 1);

FunctionPointer(void, DisplayMissionRanksName, (int*), 0x00676690);
FunctionPointer(void, Set_NJSDATA_To, (float), 0x00433D00);
FunctionPointer(int, DisplayChaoNameEmblems, (int*, float), 0x006768A0);
ThiscallFunctionPointer(void, LoadSoundBank, (char*), 0x435880);
FunctionPointer(void, LoadVoiceBank, (int, CharacterVoiceBank*), 0x459010);
FunctionPointer(unsigned int, CheckIfCharMissionUnlocked, (), 0x00678460);
FunctionPointer(void, sub_673AE0, (), 0x00673AE0);
FunctionPointer(int, AdjustCharacter, (), 0x00678250);

FunctionPointer(void, sub_664B60, (), 0x00664B60);


//hardcoded values i guess
DataPointer(float, ToCheck1, 0x01DEB6A8);
DataPointer(float, ToCheck2, 0x025F0268);
DataPointer(int, ToCheck3, 0x01DEB6A0);
DataPointer(int, ToCheck4, 0x025F02D4);

struct Vertex
{
	float X;
	float Y;
	float Z;
};
struct Vector2
{
	float X;
	float Y;
};

bool altCharacter;
int ChaoWorldCharactersPlus[10] = {
	Characters::Characters_Sonic,
	Characters::Characters_Tails,
	Characters::Characters_Knuckles,
	Characters::Characters_Shadow,
	Characters::Characters_Eggman,
	Characters::Characters_Rouge,
	Characters::Characters_Sonic,
	Characters::Characters_Knuckles,
	Characters::Characters_Shadow,
	Characters::Characters_Rouge
};
int ConvertCWCP[17] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	0,
	9,
	10,
	11,
	1,
	13,
	14,
	4,
	5
};
int ChaoSelectCharacterIconIDsPlus[10] = {
	5,
	6,
	1,
	4,
	0,
	3,
	104,
	106,
	105,
	107
};
int StageSelectCharacterIconIDs[14] = {
	5,
	4,
	104,
	105,
	1,
	3,
	106,
	107,
	6,
	0,
	110,
	111,
	108,
	109
};
float IconPosX[10] = {
	102,
	102,
	102,
	102,
	102,
	102,
	302,
	302,
	302,
	302,
};
float IconPosY[10] = {
	0,
	0,
	0,
	0,
	0,
	0,
	40,
	40,
	40,
	40
};
NJS_TEXANIM NewStageMapAnims[112];
NJS_TEXNAME NewStageMapTexNames[20];

bool SelectingStageCharacter;
int SelectedStageCharacter;
int StageCharacters[14] = {
	Characters_Sonic,
	Characters_Shadow,
	Characters_Sonic,
	Characters_Shadow,
	Characters_Knuckles,
	Characters_Rouge,
	Characters_Knuckles,
	Characters_Rouge,
	Characters_Tails,
	Characters_Eggman,
	Characters_MechTails,
	Characters_MechEggman,
	Characters_MechTails,
	Characters_MechEggman
};
bool StageCharacterAlts[14] = {
	false,
	false,
	true,
	true,
	false,
	false,
	true,
	true,
	false,
	false,
	false,
	false,
	true,
	true,
};
#pragma endregion

#pragma region usercalls
static const void* const CheckSelectedStageTypePtr = (void*)0x00676640;
static inline void CheckSelectedStageType(int stageID, int UiSelectionID, int* output)
{
	__asm
	{
		mov eax, [stageID]
		mov edx, [UiSelectionID]
		mov ecx, [output]
		call CheckSelectedStageTypePtr
	}
}

static const void* const Display_stageMapTexturePtr = (void*)0x00675C90;
static inline void Display_stageMapTexture(int UIFlags, Vertex* Position, int TexID, float ESize)
{
	__asm
	{
		push[ESize]
		push[TexID]
		mov eax, [Position]
		mov ecx, [UIFlags]
		call Display_stageMapTexturePtr
	}
}

static const void* const Display_stageMapTextureSPtr = (void*)0x00675CF0;
static inline void Display_stageMapTextureS(Vertex* Position, Vector2* ESize, int TexID, int Flags)
{
	__asm
	{
		push[Flags];
		push[TexID];
		mov ecx, [ESize];
		mov eax, [Position];
		call Display_stageMapTextureSPtr;
	}
}

static const void* const DrawTimeTextPtr = (void*)0x00674F70;
static inline void DrawTimeText(Vertex* Position, int Time, float ESize)
{
	__asm
	{
		push[ESize]
		mov esi, [Time]
		mov eax, [Position]
		call DrawTimeTextPtr
	}
}

static const void* const DrawScoreTextPtr = (void*)0x00675580;
static inline void DrawScoreText(Vertex* Position, int number, int CharCount, float ESize)
{
	__asm
	{
		push[ESize]
		push[CharCount]
		push[number]
		mov eax, [Position]
		call DrawScoreTextPtr
	}
}

static const void* const PlayGuideVoicePtr = (void*)0x006727E2;
static inline void PlayGuideVoice(int ID)
{
	__asm
	{
		mov eax, [ID]
		call PlayGuideVoicePtr
	}
}

static const void* const PlaySoundEffectPtr = (void*)0x00437260;
static inline void PlaySoundEffect(int SoundID, int a2, char a3, char a4)
{
	__asm
	{
		push[a4]
		push[a3]
		push[a2]
		mov esi, [SoundID]
		call PlaySoundEffectPtr
	}
}

static const int* const SelectNextStagePtr = (int*)0x00677BF0;
static inline int SelectNextStage(int direction)
{
	int result;
	__asm
	{
		mov eax, [direction]
		call SelectNextStagePtr
		mov result, eax
	}
	return result;
}

static const void* const sub_664DC0Ptr = (void*)0x00664DC0;
static inline void sub_664DC0(int a1)
{
	__asm
	{
		mov eax, [a1]
		call sub_664DC0Ptr
	}
}
#pragma endregion


#pragma region mod functions
//adjusting the characters to load, so that every character is loadable
void __cdecl LoadCharactersPlus()
{
	if (!SelectingStageCharacter)
	{
		if (!TwoPlayerMode)
		{
			int ch = defaultcharacters[CurrentCharacter];
			CurrentCharacter = ch & charmask;
			AltCostume[1] = AltCostume[0] = ch & altcostume ? 1 : 0;
			AltCharacter[1] = AltCharacter[0] = ch & altcharacter ? 1 : 0;
		}
		int playerNum = 0;
		int* character = &CurrentCharacter;
		int buttons = MenuButtons_Held[0];
	LoopStart:
		if (buttons & Buttons_Left)
			*character = Characters_Sonic;
		if (buttons & Buttons_Right)
			*character = Characters_Shadow;
		if (buttons & Buttons_Down)
			*character = Characters_Knuckles;
		if (buttons & Buttons_Up)
			*character = Characters_Rouge;
		if (buttons & Buttons_R)
			*character = Characters_MechTails;
		if (buttons & Buttons_L)
			*character = Characters_MechEggman;
		if (buttons & Buttons_Y)
			*character = Characters_Tails;
		if (buttons & Buttons_X)
			*character = Characters_Eggman;
		if (buttons & Buttons_B)
			AltCharacter[playerNum] ^= 1;
		if (buttons & Buttons_A)
			AltCostume[playerNum] ^= 1;
		int repcnt;
		pair<short, short>* replst;
		switch (*character)
		{
		case Characters_Sonic:
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
		AnimationInfo* anilst = MainCharObj2[playerNum]->AnimInfo.Animations;
		for (int i = 0; i < repcnt; i++)
			if (!CharacterAnimations[anilst[replst[i].key].AnimNum].Animation)
				anilst[replst[i].key] = anilst[replst[i].value];
		if (playerNum == 1)
			goto end;
		playerNum++;
		buttons = MenuButtons_Held[1];
		if (buttons & Buttons_Start)
			CurrentCharacter2P = CurrentCharacter ^ 1;
		else if (!TwoPlayerMode)
			goto end;
		character = &CurrentCharacter2P;
		goto LoopStart;
	end:
		LoadEmeraldManager_r();
	}
	else
	{
		int repcnt;
		pair<short, short>* replst;
		switch (SelectedStageCharacter)
		{
		case Characters_Sonic:
			CurrentCharacter = Characters_Sonic;
			if (!altCharacter)
				LoadSonic(0);
			else LoadAmy(0);
			repcnt = (int)LengthOfArray(SonicAnimReplacements);
			replst = SonicAnimReplacements;
			break;
		case Characters_Shadow:
			CurrentCharacter = Characters_Shadow;
			if (!altCharacter)
				LoadShadow(0);
			else LoadMetalSonic(0);
			repcnt = (int)LengthOfArray(SonicAnimReplacements);
			replst = SonicAnimReplacements;
			break;
		case Characters::Characters_Knuckles:
			CurrentCharacter = Characters_Knuckles;
			if (!altCharacter)
				LoadKnuckles(0);
			else LoadTikal(0);
			repcnt = (int)LengthOfArray(KnucklesAnimReplacements);
			replst = KnucklesAnimReplacements;
			break;
		case Characters::Characters_Rouge:
			CurrentCharacter = Characters_Rouge;
			if (!altCharacter)
				LoadRouge(0);
			else LoadChaos(0);
			repcnt = (int)LengthOfArray(KnucklesAnimReplacements) - 3;
			replst = KnucklesAnimReplacements;
			break;
		case Characters::Characters_Tails:
			CurrentCharacter = Characters_Tails;
			LoadTails(0);
			repcnt = (int)LengthOfArray(OthersAnimReplacements);
			replst = OthersAnimReplacements;
			break;
		case Characters::Characters_Eggman:
			CurrentCharacter = Characters_Eggman;
			LoadEggman(0);
			repcnt = (int)LengthOfArray(OthersAnimReplacements);
			replst = OthersAnimReplacements;
			break;
		case Characters::Characters_MechEggman:
			CurrentCharacter = Characters_MechEggman;
			if (!altCharacter)
				LoadMechEggman(0);
			else LoadDarkChaoWalker(0);
			repcnt = (int)LengthOfArray(MechAnimReplacements);
			replst = MechAnimReplacements;
			break;
		case Characters::Characters_MechTails:
			CurrentCharacter = Characters_MechTails;
			if (!altCharacter)
				LoadMechTails(0);
			else LoadChaoWalker(0);
			repcnt = (int)LengthOfArray(MechAnimReplacements);
			replst = MechAnimReplacements;
			break;
		default:
			break;
		}
		SelectingStageCharacter = false;
		SelectedStageCharacter = 0;
		InitPlayer(0);
		AnimationInfo* anilst = MainCharObj2[0]->AnimInfo.Animations;
		for (int i = 0; i < repcnt; i++)
			if (!CharacterAnimations[anilst[replst[i].key].AnimNum].Animation)
				anilst[replst[i].key] = anilst[replst[i].value];

		LoadEmeraldManager_r();
	}
} 

//changing the layout of the character select menu
void DisplaySelectMissionCharMenu()
{

	int selectedLevel;
	int selectedStageInfo[3];
	int langIsJap;

	Vertex position;
	Vertex Location_SelectionBorder;
	//variables declaration ends here

	selectedLevel = StageSelectLevels[StageSelectLevelSelection].Level;
	if (selectedLevel == LevelIDs::LevelIDs_GreenHill && !SelectingStageCharacter)
		return;
	CheckSelectedStageType(selectedLevel, StageSelectLevelSelection, selectedStageInfo);

	if (TextLanguage != 0)
		langIsJap = 4;
	else langIsJap = 0;

	position.X = StageSelect_MissionCharSelectLoc_X - 128;
	position.Y = StageSelect_MissionCharSelectLoc_Y - 64;
	position.Z = -95;

	ToCheck1 = ToCheck4;
	ToCheck3 = ToCheck4;
	NJS_IMAGEDATA_AlphaStrength = 0.5;
	NJS_IMAGEDATA_RedStrength = 0.5;
	NJS_IMAGEDATA_GreenStrength = 0.5;
	NJS_IMAGEDATA_BlueStrength = 0.5;

	if (SelectingStageCharacter)
	{

		Vertex BoxPosition;
		BoxPosition.X = StageSelect_MissionCharSelectLoc_X;
		BoxPosition.Y = StageSelect_MissionCharSelectLoc_Y;
		BoxPosition.Z = -95;
		if (selectedLevel != LevelIDs_GreenHill)
			Display_stageMapTexture(34, &BoxPosition, 91, 1);

		BoxPosition.Y += 130;
		Vector2 BoxSize;
		BoxSize.X = 1.2;
		BoxSize.Y = 0.9;
		Display_stageMapTextureS(&BoxPosition, &BoxSize, 91, 34);
		if (selectedLevel == LevelIDs_GreenHill)
			goto SelectChar;
	}
	else if (selectedStageInfo[0])
	{
		if (selectedStageInfo[0] == 1)
			Display_stageMapTexture(34, &position, 92, 1);
		else Display_stageMapTexture(34, &position, 93, 1);
	}
	else
	{
		int t;
		switch (SelectedMissionCharacter)
		{
		case 0:
		case 3:
		case 4:
			t = 93;
			break;
		default:
			t = 92;
			break;
		}
		Display_stageMapTexture(34, &position, t, 1);
	}

	ToCheck2 = ToCheck1;
	ToCheck4 = ToCheck3;
	NJS_IMAGEDATA_AlphaStrength = 0.0;
	NJS_IMAGEDATA_RedStrength = 0.0;
	NJS_IMAGEDATA_GreenStrength = 0.0;
	NJS_IMAGEDATA_BlueStrength = 0.0;

	position.X += 55.0;
	position.Y += 140.0;
	position.Z += 1.0;

	switch (selectedStageInfo[0])
	{
	case 0:
		CharOrMissionSelectionMax = 5;
		DisplayMissionRanksName(selectedStageInfo);

		Location_SelectionBorder.X = SelectedMissionCharacter * 50.0 + StageSelect_MissionCharSelectLoc_X - 101.0;
		Location_SelectionBorder.Y = StageSelect_MissionCharSelectLoc_Y + 29.0;
		Location_SelectionBorder.Z = -80;
		Display_stageMapTexture(32, &Location_SelectionBorder, 90, 0.7);
		if (!SelectedMissionCharacter || SelectedMissionCharacter > 2 && SelectedMissionCharacter <= 4)
		{
			if (SelectingStageCharacter)
				goto SelectChar;
			altCharacter = false;
			for (int i = 0; i < 4; i++)
			{
				Display_stageMapTexture(32, &position, langIsJap + 94 + i, 1.0);
				position.X += 115.0;
				Display_stageMapTexture(32, &position, 89, 1.0);
				position.X -= 115.0;
				position.Y += 20.0;
			}

			Set_NJSDATA_To(1);

			position.X += 68.0;
			position.Y -= 79.0;
			position.Z += 1.0;
			DrawTimeText(&position, (int)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 24), 0.6);


			position.X += 97.0;
			position.Y += 20.0;
			DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 16), 7, 0.6); // best ring

			position.Y += 20.0;
			DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 20), 7, 0.6); // best score

			position.Y += 20.0;
			DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 2 * SelectedMissionCharacter + 6), 7, 0.6); // no. of plays
			return;
		}
	LABEL_1:
		if (SelectingStageCharacter)
			goto SelectChar;
		altCharacter = false;
		for (int i = 0; i < 2; i++)
		{
			Display_stageMapTexture(32, &position, langIsJap + 94 + (i * 3), 1.0);
			position.X += 115.0;
			Display_stageMapTexture(32, &position, 89, 1.0);
			position.X -= 115.0;
			position.Y += 20.0;
		}

		Set_NJSDATA_To(1);

		position.X += 68.0;
		position.Y -= 39.0;
		position.Z += 1.0;

		DrawTimeText(&position, (int)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 24), 0.6);

		position.X += 97.0;
		position.Y += 20.0;
		DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 2 * SelectedMissionCharacter + 6), 7, 0.6); //no. of plays

		return;
	SelectChar:
		for (int i = 0; i < 14; i++)
		{
			position.X = (std::floor(i / 2)) * 40.0 + StageSelect_MissionCharSelectLoc_X - 120;
			position.Y = StageSelect_MissionCharSelectLoc_Y + ((i % 2 == 0) ? 110 : 150);
			position.Z = -90;

			ToCheck1 = ToCheck2;
			ToCheck3 = ToCheck4;
			NJS_IMAGEDATA_AlphaStrength = 1;
			NJS_IMAGEDATA_RedStrength = 1;
			NJS_IMAGEDATA_GreenStrength = 1;
			NJS_IMAGEDATA_BlueStrength = 1;

			Display_stageMapTexture(34, &position, StageSelectCharacterIconIDs[i], 0.6);

			if (i == SelectedStageCharacter)
			{
				ToCheck1 = ToCheck2;
				ToCheck3 = ToCheck4;
				NJS_IMAGEDATA_AlphaStrength = 1;
				NJS_IMAGEDATA_RedStrength = 1;
				NJS_IMAGEDATA_GreenStrength = 1;
				NJS_IMAGEDATA_BlueStrength = 1;

				Display_stageMapTexture(32, &position, 90, 0.6);
			}
		}
		return;
	case 1:
		CharOrMissionSelectionMax = 5;
		DisplayMissionRanksName(selectedStageInfo);

		Location_SelectionBorder.X = SelectedMissionCharacter * 50.0 + StageSelect_MissionCharSelectLoc_X - 101.0;
		Location_SelectionBorder.Y = StageSelect_MissionCharSelectLoc_Y + 29.0;
		Location_SelectionBorder.Z = -80;
		Display_stageMapTexture(32, &Location_SelectionBorder, 90, 0.7);

		goto LABEL_1;
	case 2:
		CharOrMissionSelectionMax = 10;
		DisplayChaoNameEmblems(selectedStageInfo, 90.0);
		altCharacter = (SelectedMissionCharacter > 5);

		for (int i = 0; i < 10; i++)
		{
			position.X = i * 40.0 + StageSelect_MissionCharSelectLoc_X - IconPosX[i];
			position.Y = StageSelect_MissionCharSelectLoc_Y + IconPosY[i];
			position.Z = -90;

			float IconAlpha;
			if (CheckUnlockedChar[ConvertCWCP[ChaoWorldCharactersPlus[i]]] == 0)
				IconAlpha = 0.5;
			else IconAlpha = 1;

			ToCheck1 = ToCheck2;
			ToCheck3 = ToCheck4;
			NJS_IMAGEDATA_AlphaStrength = 1;
			NJS_IMAGEDATA_RedStrength = 1;
			NJS_IMAGEDATA_GreenStrength = 1;
			NJS_IMAGEDATA_BlueStrength = 1;

			Display_stageMapTexture(34, &position, ChaoSelectCharacterIconIDsPlus[i], 0.6);
			if (SelectedMissionCharacter == i)
			{
				ToCheck1 = ToCheck2;
				ToCheck3 = ToCheck4;
				NJS_IMAGEDATA_AlphaStrength = 1.0;
				NJS_IMAGEDATA_RedStrength = 1.0;
				NJS_IMAGEDATA_GreenStrength = 1.0;
				NJS_IMAGEDATA_BlueStrength = 1.0;

				Display_stageMapTexture(32, &position, 90, 0.6);
			}
		}
		break;
	}

}
__declspec(naked) void DisplaySelectMissionCharMenu_Wrapper()
{
	__asm
	{
		push esi
		push ebx
		push edi
		push ecx
		call DisplaySelectMissionCharMenu
		pop ecx
		pop edi
		pop ebx
		pop esi
		ret
	}
}

//changing Input Handler
signed int HandleStageSelectInputPlus()
{
	int SelectDirection;
	int selectedLevel;
	int selectedStageInfo[3];

	selectedLevel = StageSelectLevels[StageSelectLevelSelection].Level;
	CheckSelectedStageType(selectedLevel, StageSelectLevelSelection, selectedStageInfo);

	switch (UI_State1)
	{
	case 0:	// initializing Menu
		*StageSelectFlags = 1; // enable stage select ui
		StageSelectFlags[4] = 1; // Enable stage select background
		UI_State1 = 1;
		return 0;
	case 1:
		if (StageSelectFlags[2]) // check if textures are loaded
			return 0;
		UI_State1 = 2;	// enable stage selection
		StageSelectFlags[9] = 1;
		StageSelectFlags[10] = 1; // display the selected level
		StageSelectFlags[13] = 1; // display the ranks box
		StageSelectFlags[14] = 1;
		PlayGuideVoice(3); // plays the "select a stage" text
		return 0;
	case 2:	// Select Level
		if (StageSelectFlags[6])
			return 0;
		if (InputDown & 2) // on back
		{
			*StageSelectFlags = 0; // disables the stage select menu
			StageSelectFlags[1] = 0;
			ShowButtonGuide = 0;
			UI_State1 = 4;	//goes into loading state, to close the menu
			PlaySoundEffect(0x8009, 0, 0, 0);
			return 0;
		}
		if (InputDown & 0xC) // on start or jump
		{
			if (!StageUnlocked[StageSelectLevelSelection])
			{
				PlaySoundEffect(0x800A, 0, 0, 0);
				return 0;
			}
			PlaySoundEffect(0x8001, 0, 0, 0);
			if (StageSelectLevels[StageSelectLevelSelection].Level == LevelIDs_GreenHill) // if level is green hill
			{
				StageSelectFlags[13] = 0; // hide the ranks box
				StageSelectFlags[10] = 0; // hide the selected level
				StageSelectFlags[21] = 1; // display  the character select screen
				StageSelectFlags[22] = 1; // display "Select your character" from the top of the screen
				UI_State1 = 3;	//goes into the mission select screen
				SelectingStageCharacter = true;
				PlayGuideVoice(2);
				/*
				sub_673AE0(); //seems to reset loads of flags, probably because we are leaving the menu at this point
				IngameFlag = 3; //flags the game as "In level"
				*StageSelectFlags = 0; // disables the stage select menu
				StageSelectFlags[1] = 0;
				ShowButtonGuide = 0; //hides the button guide at the bottom of the screen
				UI_State1 = 5; // gets into closing state
			}
			else
			{
				StageSelectFlags[13] = 0; // hide the ranks box
				StageSelectFlags[10] = 0; // hide the selected level
				StageSelectFlags[21] = 1; // display  the character select screen
				StageSelectFlags[22] = 1; // display "Select your character" from the top of the screen
				SelectedMissionCharacter = 0; // set the selected mission to 0
				UI_State1 = 3;	//goes into the mission select screen
				if (StageSelectLevels[StageSelectLevelSelection].Level == LevelIDs_ChaoWorld)
					PlayGuideVoice(2);	// plays the "select your character" text
			}
		}
		if (InputDown & 0x10) // on up
			SelectDirection = 1;
		else if (InputDown & 0x20) // on down
			SelectDirection = 2;
		else if (InputDown & 0x40) // on left
			SelectDirection = 3;
		else if (InputDown & 0x80) // on right
			SelectDirection = 4;
		if (SelectDirection);
		if (SelectNextStage(SelectDirection - 1)) // if a new stage was selected, play a sound
			PlaySoundEffect(0x8000, 0, 0, 0);
		return 0;
	case 3: // Select Mission or Character
		if (InputDown & 2) // on back
		{
			if (SelectingStageCharacter)
			{
				SelectingStageCharacter = false;
				if (StageSelectLevels[StageSelectLevelSelection].Level == LevelIDs_GreenHill) // if level is green hill
				{
					StageSelectFlags[13] = 1;
					StageSelectFlags[10] = 1;
					StageSelectFlags[21] = 0;
					StageSelectFlags[22] = 0;
					UI_State1 = 2;
					SelectedMissionCharacter = 0;
				}
			}
			else
			{
				UI_State1 = 2;	// set ui stae back to 2 (level select)
				StageSelectFlags[13] = 1; // display the ranks box again
				StageSelectFlags[10] = 1; // display the selected level again
				StageSelectFlags[21] = 0; // hide the character select screen
				StageSelectFlags[22] = 0; // remove "Select your character" from the top of the screen
				SelectedMissionCharacter = 0; // set the selected mission to 0
			}
			PlaySoundEffect(0x8009, 0, 0, 0);
			return 0;
		}
		if (InputDown & 0xC)
		{
			if (!SelectingStageCharacter && !selectedStageInfo[0])
			{
				unsigned int CharMissionUnlocked = CheckIfCharMissionUnlocked();
				if (CharMissionUnlocked)
				{
					if (CharMissionUnlocked < 4)
					{
						PlaySoundEffect(0x8001, 0, 0, 0);
						SelectingStageCharacter = true;
						switch (StageSelectLevels[StageSelectLevelSelection].Character)
						{
						case Characters_MechTails:
						case Characters_MechEggman:
							SelectedStageCharacter = StageSelectLevels[StageSelectLevelSelection].Character + 4;
							break;
						default:
							SelectedStageCharacter = StageSelectLevels[StageSelectLevelSelection].Character;
							break;
						}
						PlayGuideVoice(2);
					}
				}
				else PlaySoundEffect(0x800A, 0, 0, 0);
			}
			else
			{
				if (!selectedStageInfo[0])
				{
					altCharacter = StageCharacterAlts[SelectedStageCharacter];
					SelectedStageCharacter = StageCharacters[SelectedStageCharacter];
				}
				if (selectedStageInfo[0] == 2)
				{
					SelectedStageCharacter = ChaoWorldCharactersPlus[SelectedMissionCharacter];
					SelectingStageCharacter = true;
				}
				PlaySoundEffect(0x8001, 0, 0, 0);
				sub_673AE0(); //seems to reset loads of flags, probably because we are leaving the menu at this point
				IngameFlag = 3;
				*StageSelectFlags = 0;
				StageSelectFlags[1] = 0;
				StageSelectFlags[22] = 0;
				ShowButtonGuide = 0; //hides the button guide at the bottom of the screen
				UI_State1 = 5;
			}
			return 0;
		}
		if (StageSelectLevels[StageSelectLevelSelection].Level == LevelIDs_ChaoWorld)
		{
			if (InputDown & 0x40) // on left
			{
				switch (SelectedMissionCharacter)
				{
				case 0:
					SelectedMissionCharacter = 5;
					goto PlaySound1;
				case 6:
					SelectedMissionCharacter = 9;
					goto PlaySound1;
				default:
					SelectedMissionCharacter--;
				PlaySound1:
					PlaySoundEffect(0x8000, 0, 0, 0);
					break;
				}
			}
			else if (InputDown & 0x80) // on right
			{
				switch (SelectedMissionCharacter)
				{
				case 5:
					SelectedMissionCharacter = 0;
					goto PlaySound2;
				case 9:
					SelectedMissionCharacter = 6;
					goto PlaySound2;
				default:
					SelectedMissionCharacter++;
				PlaySound2:
					PlaySoundEffect(0x8000, 0, 0, 0);
					break;
				}
			}
			else if (InputDown & 0x10) // on up
			{
				if (SelectedMissionCharacter > 5)
				{
					SelectedMissionCharacter -= 5;
					PlaySoundEffect(0x8000, 0, 0, 0);
				}
			}
			else if (InputDown & 0x20) // on down
			{
				if (SelectedMissionCharacter > 0 && SelectedMissionCharacter < 5)
				{
					SelectedMissionCharacter += 5;
					PlaySoundEffect(0x8000, 0, 0, 0);
				}
			}
		}
		else
		{
			if (!SelectingStageCharacter)
			{
				if (InputDown & 0x40) // on left
				{
					if (SelectedMissionCharacter > 0)
					{
						SelectedMissionCharacter--;
						PlaySoundEffect(0x8000, 0, 0, 0);
					}
				}
				else if (InputDown & 0x80) // on right
				{
					if (SelectedMissionCharacter < CharOrMissionSelectionMax - 1)
					{
						SelectedMissionCharacter++;
						PlaySoundEffect(0x8000, 0, 0, 0);
					}
				}
			}
			else
			{
				if (InputDown & 0x40) // on left
				{
					if (SelectedStageCharacter < 2)
						SelectedStageCharacter += 12;
					else SelectedStageCharacter -= 2;
					PlaySoundEffect(0x8000, 0, 0, 0);
				}
				else if (InputDown & 0x80) // on right
				{
					if (SelectedStageCharacter > 11)
						SelectedStageCharacter -= 12;
					else SelectedStageCharacter += 2;
					PlaySoundEffect(0x8000, 0, 0, 0);
				}
				else if (InputDown & 0x10) // on up
				{
					if (SelectedStageCharacter % 2 != 0)
						SelectedStageCharacter--;
					PlaySoundEffect(0x8000, 0, 0, 0);
				}
				else if (InputDown & 0x20) // on down
				{
					if (SelectedStageCharacter % 2 == 0)
						SelectedStageCharacter++;
					PlaySoundEffect(0x8000, 0, 0, 0);
				}
			}
		}
		return 0;
	case 4:
		if (!StageSelectFlags[2])
		{
			sub_664B60();
			sub_664DC0(0);
			(*((unsigned int*) & (dword_1D7BB10))) = 1;
		}
		return 0;
	case 5:
		if (StageSelectFlags[2] || AdjustCharacter() > 3)
			return 0;

		byte_1DEB31E = 0;
		word_1DEB31F = 0;
		byte_1DEB321 = 1;
		return 1;
	default:
		return 0;
	}
}

// fixing the sound loading
void LoadChaoWorldSoundBank()
{
	char* v4;
	int v2 = CurrentCharacter;
	if (altCharacter)
	{
		switch (CurrentCharacter)
		{
		case 0:
			v2 = Characters::Characters_Amy;
			break;
		case 1:
			v2 = Characters::Characters_MetalSonic;
			break;
		case 4:
			v2 = Characters::Characters_Tikal;
			break;
		case 5:
			v2 = Characters::Characters_Chaos;
			break;
		default:
			break;
		}
	}

	switch (v2)
	{
	case Characters_Sonic:
	case Characters_Shadow:
	case Characters_Amy:
	case Characters_MetalSonic:
		v4 = (char*)"chao_chara_ss.mlt";
		break;
	case Characters_Knuckles:
	case Characters_Rouge:
		v4 = (char*)"chao_chara_kr.mlt";
		break;
	case Characters_Tikal:
	case Characters_Chaos:
		v4 = (char*)"se_ch_kn_BATTLE.mlt";
		break;
	case Characters_Tails:
	case Characters_Eggman:
		v4 = (char*)"chao_chara_te.mlt";
		break;
	case Characters_MechTails:
	case Characters_MechEggman:
		v4 = (char*)"se_ch_wk.mlt";
		break;
	case Characters_ChaoWalker:
	case Characters_DarkChaoWalker:
		v4 = (char*)"se_ch_wk_BATTLE.mlt";
		break;
	default:
		v4 = (char*)"chao_chara_ss.mlt";
		break;
	}
	LoadSoundBank(v4);
}

static const int return1 = 0x532054;
__declspec(naked) void loc_532029()
{
	LoadChaoWorldSoundBank();
	__asm jmp return1
}

static const int MultiPlayerVoiceBank = 0x173A0B8;
static const int return2 = 0x45923B;
__declspec(naked) void loc_2800440()
{
	__asm
	{
		mov ecx, CurrentCharacter
		mov ecx, [ecx]
		cmp altCharacter, 0
		je done
		cmp ecx, Characters_Sonic
		jne notAmy
		mov ecx, Characters_Amy
		mov	esi, 173A0B8h // these addresses are for the multiplayerVoiceBank, but it doesnt work if use the variable for some reason, so i use the addresses directly
		jmp done
		notAmy :
		cmp ecx, Characters_Shadow
			jne notMetal
			mov ecx, Characters_MetalSonic
			mov	esi, 173A0B8h + 10h
			jmp done
			notMetal :
		cmp ecx, Characters_Knuckles
			jne notKnuckles
			mov ecx, Characters_Tikal
			mov	esi, 173A0B8h + 20h
			jmp done
			notKnuckles :
		cmp ecx, Characters_Rouge
			jne done
			mov ecx, Characters_Chaos
			mov	esi, 173A0B8h + 30h

			done :
		call LoadVoiceBank
			jmp	return2
	}
}
#pragma endregion
/*
extern "C"
{
	//inserting the mods
	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{
#pragma region SA2CharSel stuff
		AnimationInfo* buf = TailsAnimList2;
		WriteData((AnimationInfo**)0x74CFD7, buf);
		memcpy(TailsAnimList2, TailsAnimList, TailsAnimList_Length * sizeof(AnimationInfo));

		buf = MechEggmanAnimList2;
		WriteData((AnimationInfo**)0x740D50, buf);
		memcpy(MechEggmanAnimList2, MechEggmanAnimList, MechEggmanAnimList_Length * sizeof(AnimationInfo));

		buf = MechTailsAnimList2;
		WriteData((AnimationInfo**)0x740FB0, buf);
		memcpy(MechTailsAnimList2, MechTailsAnimList, MechTailsAnimList_Length * sizeof(AnimationInfo));

		buf = ChaoWalkerAnimList2;
		WriteData((AnimationInfo**)0x7411DC, buf);
		memcpy(ChaoWalkerAnimList2, ChaoWalkerAnimList, ChaoWalkerAnimList_Length * sizeof(AnimationInfo));

		buf = DarkChaoWalkerAnimList2;
		WriteData((AnimationInfo**)0x7413BC, buf);
		memcpy(DarkChaoWalkerAnimList2, DarkChaoWalkerAnimList, DarkChaoWalkerAnimList_Length * sizeof(AnimationInfo));

		buf = EggmanAnimList2;
		WriteData((AnimationInfo**)0x73C2F2, buf);
		memcpy(EggmanAnimList2, EggmanAnimList, EggmanAnimList_Length * sizeof(AnimationInfo));

		buf = SonicAnimList2;
		WriteData((AnimationInfo**)0x716F0A, buf);
		memcpy(SonicAnimList2, SonicAnimList, SonicAnimList_Length * sizeof(AnimationInfo));

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

		WriteCall((void*)0x729D16, Knuckles_LevelBounds_r);
		WriteCall((void*)0x729DC5, Knuckles_LevelBounds_r);
		WriteCall((void*)0x72B0F1, Knuckles_LevelBounds_r);
		WriteCall((void*)0x72B2E8, Knuckles_LevelBounds_r);
		WriteCall((void*)0x4D45F0, LoadAquaticMineCharAnims_r);
		WriteCall((void*)0x63D727, LoadDryLagoonCharAnims_r);
		WriteCall((void*)0x4DB351, LoadCannonsCoreRCharAnims_r);
		WriteCall((void*)0x65E8F1, LoadCannonsCoreKCharAnims_r);
		WriteCall((void*)0x65662A, LoadSandOceanCharAnims_r);
		WriteCall((void*)0x4DDE49, LoadHiddenBaseCharAnims_r);
		WriteCall((void*)0x4A53AC, LoadEggGolemECharAnims_r);

		const IniFile* settings = new IniFile(std::string(path) + "\\config.ini");
		for (int i = 0; i < Characters_Amy; i++)
			defaultcharacters[i] = ParseCharacterID(settings->getString("1Player", charnames[i]), (Characters)i);
		delete settings;
#pragma endregion

		std::memcpy(NewStageMapAnims, StageMapAnims, sizeof(NJS_TEXANIM) * 104); // copy the texanims
		//std::memcpy()
		NewStageMapAnims[104] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 16, 0 }; // add the new textures
		NewStageMapAnims[105] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 17, 0 };
		NewStageMapAnims[106] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 18, 0 };
		NewStageMapAnims[107] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 19, 0 };
		NewStageMapAnims[108] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 20, 0 };
		NewStageMapAnims[109] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 21, 0 };
		NewStageMapAnims[110] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 22, 0 };
		NewStageMapAnims[111] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 23, 0 };

		WriteData((int*)0x00675CBB, (int)&NewStageMapAnims); // re-assign the texanims
		WriteData((int*)0x00675D20, (int)&NewStageMapAnims);
		WriteData((int*)0x006765F6, (int)&NewStageMapAnims);
		WriteData((int*)0x006766B2, (int)&NewStageMapAnims);
		WriteData((int*)0x006769B5, (int)&NewStageMapAnims);
		WriteData((int*)0x00676A0E, (int)&NewStageMapAnims);
		WriteData((int*)0x00677A53, (int)&NewStageMapAnims->cx);
		WriteData((int*)0x00677A8B, (int)&NewStageMapAnims[112].cx);
		WriteData((int*)0x00677A8B, (int)&NewStageMapAnims[112].cx);
		WriteData((int*)0x00C68B6C, 0x18); // raise spritecount to 24
		WriteCall((int*)0x0067792F, DisplaySelectMissionCharMenu_Wrapper);

		WriteJump((void*)0x0043D630, LoadCharactersPlus);
		WriteData((int*)0x006784A0, (int)&ChaoWorldCharactersPlus);
		WriteData((int*)0x006782A6, (int)&ChaoWorldCharactersPlus);
		WriteJump((void*)0x532029, loc_532029);
		WriteJump((void*)0x459236, loc_2800440);
		WriteData((int*)0x01739FD4, 0x008B9428);
		WriteData((int*)0x01739FE0, 0x008B9428);

		WriteCall((int*)0x006662AD, HandleStageSelectInputPlus);
	}

	unsigned __int8 twobytenop[] = { 0x66, 0x90 };
	unsigned __int8 fivebytenop[] = { 0x66, 0x90, 0x66, 0x90, 0x90 };
	unsigned __int8 shortjmp[] = { 0xEB };

	PatchInfo patches[] = {
		patchdecl(0x44E63B, twobytenop), // Dark Chao Walker Life Icon Patch
		patchdecl(0x459110, twobytenop), // 2P Sound Effects Patch
		patchdecl(0x45913B, twobytenop), // 2P Voice Patch
		patchdecl(0x4CD255, twobytenop), // Sonic's Cannon's Core Control Patch
		patchdecl(0x724261, shortjmp), // Sonic Boss Special Patch
		patchdecl(0x736211, shortjmp), // Knuckles Boss Special Patch
		patchdecl(0x7374E4, shortjmp), // Dry Lagoon Turtle Grab Patch
		patchdecl(0x749921, shortjmp), // Mech Boss Special Patch
		patchdecl(0x741690, shortjmp), // Dark Chao Walker Fix
		patchdecl(0x7416DC, shortjmp), // Chao Walker Fix
		patchdecl(0x728141, fivebytenop), // Knuckles emerald manager
		patchdecl(0x728491, fivebytenop), // Rouge emerald manager
		patchdecl(0x7288B7, fivebytenop), // Tikal emerald manager
		patchdecl(0x728B64, fivebytenop), // Chaos emerald manager
		patchdecl(0x716E13, twobytenop), // Amy
		patchdecl(0x716F2C, twobytenop), // Sonic costume
		patchdecl(0x717373, twobytenop), // Metal Sonic
		patchdecl(0x71748C, twobytenop), // Shadow costume
		patchdecl(0x728123, twobytenop), // Tikal
		patchdecl(0x728241, twobytenop), // Knuckles costume
		patchdecl(0x728473, twobytenop), // Chaos
		patchdecl(0x728591, twobytenop), // Rouge costume
		patchdecl(0x740C61, twobytenop), // Dark Chao Walker
		patchdecl(0x740D72, twobytenop), // Eggman costume
		patchdecl(0x740EC1, twobytenop), // Chao Walker
		patchdecl(0x740FD2, twobytenop) // Tails costume
	};

	__declspec(dllexport) PatchList Patches = { arrayptrandlength(patches) };

	PointerInfo jumps[] = {
		ptrdecl(0x458970, sub_458970), // Level Cutscene Function
									   // ptrdecl(LoadCharacters, LoadCharacters_r), // LoadCharacters replacement
									   ptrdecl(0x757810, sub_757810), // Somersault Fix 1
									   ptrdecl(0x759A18, loc_759A18), // Somersault Fix 2
									   ptrdecl(LoadStartPositionPtr, LoadStartPosition_r), // LoadStartPosition replacement
									   ptrdecl(0x43DF30, sub_43DF30), // End position
									   ptrdecl(Load2PIntroPos, Load2PIntroPos_r), // 2P Intro position
									   ptrdecl(0x6193D0, sub_6193D0), // Sonic vs Shadow 1
									   ptrdecl(0x4C7100, sub_4C7100), // Sonic vs Shadow 2
									   ptrdecl(0x648690, sub_648690), // Knuckles vs Rouge
									   ptrdecl(0x626680, sub_626680), // Tails vs Eggman 1
									   ptrdecl(0x661CF0, sub_661CF0), // Tails vs Eggman 2
									   ptrdecl(0x727E5B, loc_727E5B), // 2P Race Bar
									   ptrdecl(0x6C63E7, loc_6C63E7), // Goal Ring
									   ptrdecl(0x43C9D0, 0x43CADF), // Tails/Eggman fix
									   ptrdecl(0x472A7D, loc_472A7D), // Title Card textures
									   ptrdecl(0x43EE5F, loc_43EE5F), // End Level voices
									   ptrdecl(0x72F4D6, KnucklesSunglassesFix),
									   ptrdecl(0x72F531, KnucklesAirNecklaceFix),
									   ptrdecl(0x744914, EggmanLaserBlasterFix),
									   ptrdecl(0x744E02, EggmanLargeCannonFix),
									   ptrdecl(0x748168, TailsLaserBlasterFix),
									   ptrdecl(0x74861A, TailsBazookaFix),
									   //ptrdecl(0x4EB2B0, InitSplitscreenPlus)
	};

	__declspec(dllexport) PointerList Jumps = { arrayptrandlength(jumps) };

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
*/