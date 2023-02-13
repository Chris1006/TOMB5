#pragma once
#include "../global/types.h"

void inject_rat(bool replace);

long GetFreeRat();
void ClearRats();
void TriggerRat(short item_number);
void UpdateRats();
void DrawRats();
void InitialiseRatGenerator(short item_number);

extern RAT_STRUCT* Rats;
