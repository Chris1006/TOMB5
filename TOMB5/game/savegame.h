#pragma once
#include "../global/vars.h"

void inject_savegame(bool replace);

void WriteSG(void* pointer, long size);
void ReadSG(void* pointer, long size);
long CheckSumValid(char* buffer);
static void SaveLaraData();
static void RestoreLaraData(long FullSave);
static void SaveLevelData(long FullSave);
static void RestoreLevelData(long FullSave);
void sgSaveGame();
void sgRestoreGame();
