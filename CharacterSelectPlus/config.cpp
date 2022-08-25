#include "stdafx.h"
#include <array>
#include <algorithm>
#include <cmath>

using std::string;
using std::unordered_map;


#pragma region mod setting stuff

static string trim(const string& s)
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

static uint8_t ParseCharacterID(const string& str, Characters def)
{
	string s = trim(str);
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	auto ch = charnamemap.find(s);
	if (ch != charnamemap.end())
		return ch->second;
	return def;
}

const string charnames[Characters_Amy] = { "Sonic", "Shadow", "Tails", "Eggman", "Knuckles", "Rouge", "MechTails", "MechEggman" };
const string charnamesalt[Characters_Amy] = { "Amy", "MetalSonic", "Tails", "Eggman", "Tikal", "Chaos", "ChaoWalker", "DarkChaoWalker" };

#pragma endregion

void init_Config(const char* path)
{
	const IniFile* settings = new IniFile(std::string(path) + "\\config.ini");
	for (int i = 0; i < Characters_Amy; i++)
		defaultcharacters[i] = ParseCharacterID(settings->getString("1Player", charnames[i]), (Characters)i);
	for (int i = 0; i < Characters_Amy; i++)
		defaultcharacters2p[i] = ParseCharacterID(settings->getString("2Player", charnames[i]), (Characters)i);
	for (int i = 0; i < Characters_Amy; i++)
		defaultcharacters2palt[i] = ParseCharacterID(settings->getString("2Player", charnamesalt[i]), (Characters)(i | altcharacter));
	for (int i = 0; i < Characters_Amy; i++)
		bosscharacters[i] = ParseCharacterID(settings->getString("Boss", charnames[i]), (Characters)i);

	delete settings;

}