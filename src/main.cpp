#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "AutPI.h"
#include "mod_loader.h"
#include "cave_story.h"
#include "BKG.h"
#include "lua/Lua.h"
#include "Profile.h"
#include "TSC.h"

char gSavePath[MAX_PATH];
char gBkgPath[MAX_PATH];

void LoadBKG_TxT_Background()
{
	if (!(bkgTxT_Global[0] == 0)) // if it doesnt == 0
		BKG_LoadBackground(bkgTxT_Global);
}

void BkgTransferStageInit()
{
	if (isLoadingSave == false)
		BKG_ResetBackgrounds();
	else
		LoadBKG_TxT_Background();
}

void InitElement()
{
	isLoadingSave = false;
	BKG_ResetBackgrounds();
}

void ActElement()
{
	isLoadingSave = false;
}

void InitMod_TSCBkg()
{
	// Be able to inject code here? AutPI addition for later to consider
	ModLoader_WriteCall((void*)0x402339, (void*)Replacement_InitBack_ReloadBitmap_File_Call); // Release Surface + MakeSurface_File instead of reloading bitmap
	RegisterOpeningBelowPutStage_BackElement(PutBKGBack);
	RegisterOpeningAbovePutStage_FrontElement(PutBKGFront);
	RegisterBelowPutStage_BackElement(PutBKGBack);
	RegisterAbovePutStage_FrontElement(PutBKGFront);
	RegisterTransferStageInitElement(BkgTransferStageInit);
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));

	// Get path of the Save folder
	strcpy(gSavePath, exeModulePath);
	strcat(gSavePath, "\\savedata");

	// Get path of the Bkg folder
	strcpy(gBkgPath, exeDataPath);
	strcat(gBkgPath, "\\bkg");

	RegisterLuaFuncElement(SetModLua);
	RegisterOpeningInitElement(InitElement);
	RegisterInitElement(InitElement);
	RegisterSaveProfilePostCloseElement(SaveBkgFile);
	RegisterLoadProfilePostCloseElement(LoadBkgFile);
	RegisterInitializeGameInitElement(InitializeGameCall);
	InitMod_TSC();
	RegisterActionElement(ActElement);
}

void InitMod(void)
{
	// Load the autpi dll. Feel free to remove all of the "Lua" and "AutPI" files if you dont plan to use AutPI, but its recommended for most cases, as you can do a lot with it.
	LoadAutPiDll();
	RegisterPreModeElement(InitMod_TSCBkg);
}
