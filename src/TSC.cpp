

#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TSC.h"

#include "main.h"

#include "BKG.h"
#include "mod_loader.h"
#include "mod_loader_hooks.h"
#include "cave_story.h"
#include "Profile.h"

// JakeV wrote this in "TSC Extended"
void GetTextScriptString(char returnData[])
{
	int i = 0;
	while (gTS.data[gTS.p_read] != '$') {
		returnData[i] = gTS.data[gTS.p_read];
		gTS.p_read++;
		i++;
	}
	//Insert the null terminator overtop the $
	returnData[i] = '\0';
	//Skip over the '$'
	gTS.p_read++;
}

static int CustomTSC_BKG(MLHookCPURegisters* regs, void* ud)
{
	(void)ud;
	int w, x, y, z;

	char* where = TextScriptBuffer + gTS.p_read;
	if (where[0] != '<')
		return 0;
	if (strncmp(where + 1, "BKG", 3) == 0) // BacKGround
	{
		char bkPath[bkgTxTSize];
		gTS.p_read += 4;

		memset(bkPath, 0, sizeof(bkPath));

		GetTextScriptString(bkPath);
		BKG_LoadBackground(bkPath);
	}
	else if (strncmp(where + 1, "BKP", 3) == 0) // BacKground Parameter
	{
		w = GetTextScriptNo(gTS.p_read + 4);
		x = GetTextScriptNo(gTS.p_read + 9);
		y = GetTextScriptNo(gTS.p_read + 14);

		BKG_SetParameter(w, x, y); // Set parameter X for layer W to value Y
		gTS.p_read += 18;
	}
	else if (strncmp(where + 1, "BKR", 3) == 0) // BacKground Reset
	{
		BKG_ResetBackgrounds();
		gTS.p_read += 4;
	}
	else if (strncmp(where + 1, "BKE", 3) == 0) // BacKground Enable
	{
		z = GetTextScriptNo(gTS.p_read + 4);
		bkList[z].isActive = true;
		gTS.p_read += 8;
	}
	else if (strncmp(where + 1, "BKD", 3) == 0) // BacKground Disable
	{
		z = GetTextScriptNo(gTS.p_read + 4);
		bkList[z].isActive = false;
		gTS.p_read += 8;
	}
	else
		return 0;

	regs->eip = CSJ_tsc_done;
	return 1;
}

void InitMod_TSC()
{
	ModLoader_AddStackableHook(CSH_tsc_start, CustomTSC_BKG, (void*)0);
}