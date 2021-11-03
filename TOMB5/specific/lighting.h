#pragma once
#include "../global/vars.h"

void inject_lighting(bool replace);

void InitObjectLighting(ITEM_INFO* item);

#define SuperResetLights	( (void(__cdecl*)()) 0x004AAF00 )
#define CalcAmbientLight	( (void(__cdecl*)(ITEM_INFO*)) 0x004A9E60 )
#define CreateLightList		( (void(__cdecl*)(ITEM_INFO*)) 0x004AA5A0 )
#define ClearObjectLighting	( (void(__cdecl*)()) 0x004AB910 )//empty
#define ClearDynamicLighting	( (void(__cdecl*)()) 0x004AB9D0 )//empty
#define InitDynamicLighting_noparams	( (void(__cdecl*)()) 0x004AB930 )
#define InitDynamicLighting	( (void(__cdecl*)(ITEM_INFO*)) 0x004AB950 )
#define InitObjectFogBulbs	( (void(__cdecl*)()) 0x004AB580 )
#define S_CalculateStaticMeshLight	( (void(__cdecl*)(long, long, long, long, ROOM_INFO*)) 0x004A9DF0 )
#define ShowOmni	( (void(__cdecl*)(long, long, long, long)) 0x004AA0C0 )
#define SuperSetupLight	( (void(__cdecl*)(PCLIGHT*, ITEM_INFO*, long*)) 0x004AAFE0 )
