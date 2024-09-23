#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "BKG.h"

#include "AutPI.h"
#include "cave_story.h"
#include "main.h"

bool isLoadingSave = false;

const char* gBkgSaveFileName = "BKG";

void SaveBkgFile()
{
	FILE* fp;

	char path[MAX_PATH];
	std::string sp = GetCustomSaveName();
	std::size_t id = sp.find_last_of("/\\");
	sprintf(path, "%s\\savedata\\%s.%s", sp.substr(0, id).c_str(), sp.substr(id + 1).c_str(), gBkgSaveFileName);

	fp = fopen(path, "wb");
	if (fp == NULL)
		return;

	fwrite(bkgTxT_Global, bkgTxTSize, 1, fp);

	fclose(fp);

	return;
}

void LoadBkgFile()
{
	FILE* fp;

	char path[MAX_PATH];
	std::string sp = GetCustomSaveName();
	std::size_t id = sp.find_last_of("/\\");
	sprintf(path, "%s\\savedata\\%s.%s", sp.substr(0, id).c_str(), sp.substr(id + 1).c_str(), gBkgSaveFileName);

	fp = fopen(path, "rb");
	if (fp == NULL)
		return;

	fread(bkgTxT_Global, bkgTxTSize, 1, fp);

	fclose(fp);

	isLoadingSave = true;

	return;
}

void InitializeGameCall()
{
	BKG_ResetBackgrounds();
}