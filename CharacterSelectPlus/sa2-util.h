#pragma once

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

DataArray(CharacterSoundBank, stru_1739F58, 0x1739F58, 8);

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
		add esp, 8
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
		add esp, 8
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
		add esp, 4
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
		add esp, 12
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

//void __usercall sub_459010(MLTSoundList* a1@<edi>, CharacterVoiceBank* a2@<esi>)
static const void* const sub_459010Ptr = (void*)0x459010;
static inline void sub_459010(MLTSoundList* a1, CharacterVoiceBank* a2)
{
	__asm
	{
		mov esi, [a2]
		mov edi, [a1]
		call sub_459010Ptr
	}
}
#pragma endregion