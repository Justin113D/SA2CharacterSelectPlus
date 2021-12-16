#include "stdafx.h"
#include <shellapi.h>

bool isTestSpawn() {
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	for (int i = 1; i < argc; i++)
	{
		if (!wcscmp(argv[i], L"--level") || !wcscmp(argv[i], L"-l"))
		{
			return true;
		}
	}

	return false;
}

bool isSonicTrickMod() {
	HMODULE sonicMod = GetModuleHandle(L"SA2-Sonic-Tricks");

	if (sonicMod)
		return true;

	return false;
}