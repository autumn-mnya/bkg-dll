#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "BKG.h"

#include "cave_story.h"
#include "main.h"

bool isLoadingSave = false;

const char* gBkgSaveFileName = "Profile.BKG.dat";

void SaveBkgFile()
{
	FILE* fp;

	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gSavePath, gBkgSaveFileName);

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
	sprintf(path, "%s\\%s", gSavePath, gBkgSaveFileName);

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