#include "stdafx.h"
#include <stdio.h>
#include <cstdio>
#include <array>
#include <algorithm>
#include <cmath>

using std::string;
using std::unordered_map;
using std::transform;

FastFunctionHook<void> LoadCharacters_t((intptr_t)LoadCharacters);
bool useAltCharacter;
bool useAltSkin;
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

NJS_TEXANIM NewStageMapAnims[113];
NJS_TEXNAME NewStageMapTexNames[25];

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

#pragma region mod functions
	//adjusting the characters to load, so that every character is loadable

	int defaultcharacters[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };
	int defaultcharacters2p[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };
	int defaultcharacters2palt[Characters_Amy] = { Characters_Sonic | altcharacter, Characters_Shadow | altcharacter, Characters_Tails, Characters_Eggman, Characters_Knuckles | altcharacter, Characters_Rouge | altcharacter, Characters_MechTails | altcharacter, Characters_MechEggman | altcharacter };

	void __cdecl LoadCharactersPlus()
	{
		if (!SelectingStageCharacter)
		{
			if (CurrentLevel != LevelIDs_ChaoWorld)
			{
				if ((CurrentCharacter & ~1) == Characters_Tails && !isTestSpawn()) //fix Tails and Eggman mechless not working w/ test spawn
					CurrentCharacter += Characters_MechTails - Characters_Tails;
			}
			else
			{
				if ((CurrentCharacter & ~1) == Characters_MechTails)
					CurrentCharacter -= Characters_MechTails - Characters_Tails;
			}
			if (!TwoPlayerMode)
			{
				int ch = defaultcharacters[CurrentCharacter];
				CurrentCharacter = ch & charmask;
		
				int CostumeEnabled = ch & altcostume ? 1 : 0;
				if (CostumeEnabled)
				{
					AltCostume[1] = AltCostume[0] = CostumeEnabled; //don't set costume to 0 so other mods can overwrite the value fine.
				}
			
				AltCharacter[1] = AltCharacter[0] = ch & altcharacter ? 1 : 0;
			}
			else
			{
				int ch;
				if (!AltCharacter[0])
					ch = defaultcharacters2p[CurrentCharacter];
				else
					ch = defaultcharacters2palt[CurrentCharacter];
				CurrentCharacter = ch & charmask;
				int CostumeEnabled = ch & altcostume ? 1 : 0;
				if (CostumeEnabled)
				{
					AltCostume[0] = CostumeEnabled; //don't set costume to 0 so other mods can overwrite the value fine.
				}

				AltCharacter[0] = ch & altcharacter ? 1 : 0;
				if (!AltCharacter[1])
					ch = defaultcharacters2p[CurrentCharacter2P];
				else
					ch = defaultcharacters2palt[CurrentCharacter2P];
				CurrentCharacter2P = ch & charmask;
				CostumeEnabled = ch & altcostume ? 1 : 0;
				if (CostumeEnabled)
				{
					AltCostume[1] = CostumeEnabled; //don't set costume to 0 so other mods can overwrite the value fine.
				}

				AltCharacter[1] = ch & altcharacter ? 1 : 0;
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

			LoadCharacters_t.Original();
			//LoadAnimations(character, playerNum);
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
		
		}
		else
		{
			int ch = defaultcharacters[CurrentCharacter];
			CurrentCharacter = ch & charmask;

			int CostumeEnabled = ch & altcostume ? 1 : 0;
			if (CostumeEnabled)
			{
				AltCostume[1] = AltCostume[0] = CostumeEnabled; //don't set costume to 0 so other mods can overwrite the value fine.
			}

			AltCharacter[1] = AltCharacter[0] = ch & altcharacter ? 1 : 0;
			int* character = &CurrentCharacter;

			*character = SelectedStageCharacter;

			if (useAltCharacter)
				AltCharacter[0] ^= 1;

			if (useAltSkin)
			{
				AltCostume[0] = 1;
			}
				

			SelectingStageCharacter = false;
			useAltSkin = false;
			useAltCharacter = false;

			LoadCharacters_t.Original();
			//LoadAnimations(character, 0);
		}
		end:
		LoadEmeraldManager_r_wrapper();
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
			if(selectedLevel != LevelIDs_GreenHill)
				Display_stageMapTexture(34, &BoxPosition, 91, 1);

			BoxPosition.Y += 130;
			Vector2 BoxSize;
			BoxSize.X = 1.2f;
			BoxSize.Y = 0.9f;
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

			Location_SelectionBorder.X = SelectedMissionCharacter * 50.0f + StageSelect_MissionCharSelectLoc_X - 101.0f;
			Location_SelectionBorder.Y = StageSelect_MissionCharSelectLoc_Y + 29.0f;
			Location_SelectionBorder.Z = -80;
			Display_stageMapTexture(32, &Location_SelectionBorder, 90, 0.7f);
			if (!SelectedMissionCharacter || SelectedMissionCharacter > 2 && SelectedMissionCharacter <= 4)
			{
				if (SelectingStageCharacter)
					goto SelectChar;
				useAltCharacter = false;
				for (int i = 0; i < 4; i++)
				{
					Display_stageMapTexture(32, &position, langIsJap + 94 + i, 1.0f);
					position.X += 115.0f;
					Display_stageMapTexture(32, &position, 89, 1.0f);
					position.X -= 115.0f;
					position.Y += 20.0f;
				}

				Set_NJSDATA_To(1);

				position.X += 68.0f;
				position.Y -= 79.0f;
				position.Z += 1.0f;
				DrawTimeText(&position, (int)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 24), 0.6f);


				position.X += 97.0f;
				position.Y += 20.0f;
				DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 16), 7, 0.6f); // best ring

				position.Y += 20.0f;
				DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 20), 7, 0.6f); // best score

				position.Y += 20.0f;
				DrawScoreText(&position, *(unsigned short *)(selectedStageInfo[1] + 2 * SelectedMissionCharacter + 6), 7, 0.6f); // no. of plays
				return;
			}
		LABEL_1:
			if (SelectingStageCharacter)
				goto SelectChar;
			useAltCharacter = false;
			for (int i = 0; i < 2; i++)
			{
				Display_stageMapTexture(32, &position, langIsJap + 94 + (i * 3), 1.0f);
				position.X += 115.0f;
				Display_stageMapTexture(32, &position, 89, 1.0f);
				position.X -= 115.0f;
				position.Y += 20.0f;
			}

			Set_NJSDATA_To(1);

			position.X += 68.0f;
			position.Y -= 39.0f;
			position.Z += 1.0f;

			DrawTimeText(&position, (int)(selectedStageInfo[1] + 36 * SelectedMissionCharacter + 24), 0.6f);

			position.X += 97.0;
			position.Y += 20.0;
			DrawScoreText(&position, *(unsigned short*)(selectedStageInfo[1] + 2 * SelectedMissionCharacter + 6), 7, 0.6f); //no. of plays

			return;
		SelectChar:
			for (int i = 0; i < 14; i++)
			{
				position.X = (std::floor (i / 2)) * 40.0f + StageSelect_MissionCharSelectLoc_X - 120;
				position.Y = StageSelect_MissionCharSelectLoc_Y + ((i % 2 == 0) ? 110 : 150);
				position.Z = -90;

				ToCheck1 = ToCheck2;
				ToCheck3 = ToCheck4;
				NJS_IMAGEDATA_AlphaStrength = 1;
				NJS_IMAGEDATA_RedStrength = 1;
				NJS_IMAGEDATA_GreenStrength = 1;
				NJS_IMAGEDATA_BlueStrength = 1;

				Display_stageMapTexture(34, &position, StageSelectCharacterIconIDs[i], 0.6f);

				if (i == SelectedStageCharacter)
				{
					ToCheck1 = ToCheck2;
					ToCheck3 = ToCheck4;
					NJS_IMAGEDATA_AlphaStrength = 1;
					NJS_IMAGEDATA_RedStrength = 1;
					NJS_IMAGEDATA_GreenStrength = 1;
					NJS_IMAGEDATA_BlueStrength = 1;

					Display_stageMapTexture(32, &position, useAltSkin? 112 : 90, 0.6f);
				}
			}
			return;
		case 1:
			CharOrMissionSelectionMax = 5;
			DisplayMissionRanksName(selectedStageInfo);

			Location_SelectionBorder.X = SelectedMissionCharacter * 50.0f + StageSelect_MissionCharSelectLoc_X - 101.0f;
			Location_SelectionBorder.Y = StageSelect_MissionCharSelectLoc_Y + 29.0f;
			Location_SelectionBorder.Z = -80;
			Display_stageMapTexture(32, &Location_SelectionBorder, 90, 0.7f);

			goto LABEL_1;
		case 2:
			CharOrMissionSelectionMax = 10;
			DisplayChaoNameEmblems(selectedStageInfo, 90.0f);
			useAltCharacter = (SelectedMissionCharacter > 5);

			for (int i = 0; i < 10; i++)
			{
				position.X = i * 40.0f + StageSelect_MissionCharSelectLoc_X - IconPosX[i];
				position.Y = StageSelect_MissionCharSelectLoc_Y + IconPosY[i];
				position.Z = -90;

				float IconAlpha;
				if (CheckUnlockedChar[ConvertCWCP[ChaoWorldCharactersPlus[i]]] == 0)
					IconAlpha = 0.5f;
				else IconAlpha = 1;

				ToCheck1 = ToCheck2;
				ToCheck3 = ToCheck4;
				NJS_IMAGEDATA_AlphaStrength = 1;
				NJS_IMAGEDATA_RedStrength = 1;
				NJS_IMAGEDATA_GreenStrength = 1;
				NJS_IMAGEDATA_BlueStrength = 1;

				Display_stageMapTexture(34, &position, ChaoSelectCharacterIconIDsPlus[i], 0.6f);
				if (SelectedMissionCharacter == i)
				{
					ToCheck1 = ToCheck2;
					ToCheck3 = ToCheck4;
					NJS_IMAGEDATA_AlphaStrength = 1.0f;
					NJS_IMAGEDATA_RedStrength = 1.0f;
					NJS_IMAGEDATA_GreenStrength = 1.0f;
					NJS_IMAGEDATA_BlueStrength = 1.0f;

					Display_stageMapTexture(32, &position, useAltSkin ? 112 : 90, 0.6f);
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
		int SelectDirection = 0;
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
				PlaySoundProbably(0x8009, 0, 0, 0);
				return 0;
			}
			if (InputDown & 0xC) // on start or jump
			{
				if (!StageUnlocked[StageSelectLevelSelection])
				{
					PlaySoundProbably(0x800A, 0, 0, 0);
					return 0;
				}
				PlaySoundProbably(0x8001, 0, 0, 0);
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
					UI_State1 = 5; // gets into closing state*/
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
					PlaySoundProbably(0x8000, 0, 0, 0);
			return 0;
		case 3: // Select Mission or Character
			if (InputDown & 2) // on back
			{
				if (SelectingStageCharacter)
				{
					SelectingStageCharacter = false;
					useAltSkin = false;
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
				PlaySoundProbably(0x8009, 0, 0, 0);
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
							PlaySoundProbably(0x8001, 0, 0, 0);
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
					else PlaySoundProbably(0x800A, 0, 0, 0);
				}
				else
				{ 
					if (!selectedStageInfo[0])
					{
						useAltCharacter = StageCharacterAlts[SelectedStageCharacter];
						SelectedStageCharacter = StageCharacters[SelectedStageCharacter];
					}
					if (selectedStageInfo[0] == 2)
					{
						SelectedStageCharacter = ChaoWorldCharactersPlus[SelectedMissionCharacter];
						SelectingStageCharacter = true;
					}
					PlaySoundProbably(0x8001, 0, 0, 0);
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
			if (InputDown & Buttons_X || InputDown & Buttons_Y || InputDown & Buttons_Z || InputDown & Buttons_D)
			{
				useAltSkin = !useAltSkin;
				PlaySoundProbably(0x8000, 0, 0, 0);
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
						PlaySoundProbably(0x8000, 0, 0, 0);
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
						PlaySoundProbably(0x8000, 0, 0, 0);
						break;
					}
				}
				else if (InputDown & 0x10) // on up
				{
					if (SelectedMissionCharacter > 5)
					{
						SelectedMissionCharacter -= 5;
						PlaySoundProbably(0x8000, 0, 0, 0);
					}
				}
				else if (InputDown & 0x20) // on down
				{
					if (SelectedMissionCharacter > 0 && SelectedMissionCharacter < 5)
					{
						SelectedMissionCharacter += 5;
						PlaySoundProbably(0x8000, 0, 0, 0);
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
							PlaySoundProbably(0x8000, 0, 0, 0);
						}
					}
					else if (InputDown & 0x80) // on right
					{
						if (SelectedMissionCharacter < CharOrMissionSelectionMax - 1)
						{
							SelectedMissionCharacter++;
							PlaySoundProbably(0x8000, 0, 0, 0);
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
						PlaySoundProbably(0x8000, 0, 0, 0);
					}
					else if (InputDown & 0x80) // on right
					{
						if (SelectedStageCharacter > 11)
							SelectedStageCharacter -= 12;
						else SelectedStageCharacter += 2;
						PlaySoundProbably(0x8000, 0, 0, 0);
					}
					else if (InputDown & 0x10) // on up
					{
						if (SelectedStageCharacter % 2 != 0)
							SelectedStageCharacter--;
						PlaySoundProbably(0x8000, 0, 0, 0);
					}
					else if (InputDown & 0x20) // on down
					{
						if (SelectedStageCharacter % 2 == 0)
							SelectedStageCharacter++;
						PlaySoundProbably(0x8000, 0, 0, 0);
					}
				}
			}
			return 0;
		case 4:
			if (!StageSelectFlags[2])
			{
				sub_664B60();
				sub_664DC0(0);
				(*((unsigned int*)&(dword_1D7BB10))) = 1;
			}
			return 0;
		case 5:
			if ( StageSelectFlags[2] || AdjustCharacter() > 3)
				return 0;

			byte_1DEB31E = 0;
			word_1DEB31F = 0;
			byte_1DEB321 = 1;
			return 1;
		default:
			return 0;
		}
	}

	FastFunctionHook<int> ReleasePointers_t(0x439C80);
	//called when you quit a stage
	int ReleasePointers_r()
	{
		AltCostume[0] = 0;
		AltCostume[1] = 0;
		return ReleasePointers_t.Original();
	}

	
	// fixing the sound loading
	void LoadChaoWorldSoundBank()
	{
		char *v4;
		int v2 = MainCharObj2[0]->CharID2;

		switch (v2)
		{
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
			mov ecx, MainCharObj2
			movzx ecx, [ecx].CharID2
			cmp ecx, Characters_Amy
			jne notAmy
				mov	esi, 173A0B8h // these addresses are for the multiplayerVoiceBank, but it doesnt work if i use the variable for some reason, so i use the addresses directly
				jmp done
		notAmy :
			cmp ecx, Characters_MetalSonic
			jne notMetal
				mov	esi, 173A0B8h + 10h
				jmp done
		notMetal :
			cmp ecx, Characters_Tikal
			jne notTikal
				mov	esi, 173A0B8h + 20h
				jmp done
		notTikal :
			cmp ecx, Characters_Chaos
			jne done
				mov	esi, 173A0B8h + 30h 

		done :
			call LoadVoiceBank
			jmp	return2
		}
	}

	
#pragma endregion

#pragma region Boss Characters

int bosscharacters[Characters_Amy] = { Characters_Sonic, Characters_Shadow, Characters_Tails, Characters_Eggman, Characters_Knuckles, Characters_Rouge, Characters_MechTails, Characters_MechEggman };
void __cdecl LoadBossCharacter()
{
	int character = bosscharacters[CurrentCharacter ^ 1];
	AltCostume[1] = character & altcostume ? 1 : 0;
	AltCharacter[1] = character & altcharacter ? 1 : 0;
	character &= charmask;
	int buttons = MenuButtons_Held[1];
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
		AltCharacter[1] ^= 1;
	if (buttons & Buttons_A)
		AltCostume[1] ^= 1;
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

const void* const loc_6193F5 = (void*)0x6193F5;
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

const void* const loc_4C7120 = (void*)0x4C7120;
__declspec(naked) void sub_4C7100()
{
	LoadBossCharacter();
	__asm jmp loc_4C7120
}

const void* const loc_6486B2 = (void*)0x6486B2;
__declspec(naked) void sub_648690()
{
	LoadBossCharacter();
	__asm jmp loc_6486B2
}

const void* const loc_6266A2 = (void*)0x6266A2;
__declspec(naked) void sub_626680()
{
	LoadBossCharacter();
	__asm jmp loc_6266A2
}

const void* const loc_661D12 = (void*)0x661D12;
__declspec(naked) void sub_661CF0()
{
	LoadBossCharacter();
	__asm jmp loc_661D12
}

#pragma endregion



extern "C"
{
	//inserting the mods
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		InitBase();
		InitBossTitles();
		InitRankVoice();

		WriteJump((void*)0x6193D0, sub_6193D0); // Sonic vs Shadow 1
		WriteJump((void*)0x4C7100, sub_4C7100), // Sonic vs Shadow 2
		WriteJump((void*)0x648690, sub_648690), // Knuckles vs Rouge
		WriteJump((void*)0x626680, sub_626680), // Tails vs Eggman 1
		WriteJump((void*)0x661CF0, sub_661CF0), // Tails vs Eggman 2

		std::memcpy(NewStageMapAnims, StageMapAnims, sizeof(NJS_TEXANIM) * 104); // copy the texanims
		NewStageMapAnims[104] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 16, 0 }; // add the new textures
		NewStageMapAnims[105] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 17, 0 };
		NewStageMapAnims[106] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 18, 0 };
		NewStageMapAnims[107] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 19, 0 };
		NewStageMapAnims[108] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 20, 0 }; 
		NewStageMapAnims[109] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 21, 0 };
		NewStageMapAnims[110] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 22, 0 };
		NewStageMapAnims[111] = { 0x3B, 0x3B, 0, 0, 8, 8, 0x0F4, 0x0F4, 23, 0 };
		NewStageMapAnims[112] = { 0x4C, 0x4C, 0, 0, 0, 0, 0x98, 0x98, 24, 0 };

		WriteData((int*)0x00675CBB, (int)&NewStageMapAnims); // re-assign the texanims
		WriteData((int*)0x00675D20, (int)&NewStageMapAnims);
		WriteData((int*)0x006765F6, (int)&NewStageMapAnims);
		WriteData((int*)0x006766B2, (int)&NewStageMapAnims);
		WriteData((int*)0x006769B5, (int)&NewStageMapAnims);
		WriteData((int*)0x00676A0E, (int)&NewStageMapAnims);
		WriteData((int*)0x00677A53, (int)&NewStageMapAnims->cx);
		WriteData((int*)0x00677A8B, (int)&NewStageMapAnims[113].cx); //SA2 Optimization, they do check if the array is out of range.

		StageMap_Texlist.textures = NewStageMapTexNames;
		StageMap_Texlist.nbTexture = LengthOfArray(NewStageMapTexNames);
		//WriteData((int*)0x00C68B6C, 0x19); // raise spritecount to 25 //overflow memory

		WriteCall((int*)0x0067792F, DisplaySelectMissionCharMenu_Wrapper);
		LoadCharacters_t.Hook(LoadCharactersPlus);
		ReleasePointers_t.Hook(ReleasePointers_r);
		WriteCall((int*)0x006662AD, HandleStageSelectInputPlus);

		WriteData((int*)0x006784A0, (int)&ChaoWorldCharactersPlus);
		WriteData((int*)0x006782A6, (int)&ChaoWorldCharactersPlus);

		WriteJump((void*)0x532029, loc_532029);
		WriteJump((void*)0x459236, loc_2800440);
		WriteData((int*)0x01739FD4, 0x01739FE0);
		WriteData((int*)0x01739FE0, 0x008B9428);

		init_Config(path);
		init_SetFiles();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
