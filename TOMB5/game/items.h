#pragma once
#include "../global/types.h"

void inject_items();

#define ItemNewRoom	( (void(__cdecl*)(short, short)) 0x00440DA0 )
#define KillItem	( (void(__cdecl*)(short)) 0x00440620 )
#define AddActiveItem	( (void(__cdecl*)(short)) 0x00440D10 )
#define	RemoveActiveItem	( (short(__cdecl*)(short)) 0x00440B60 )
#define CreateItem	( (short(__cdecl*)()) 0x00440840)
#define InitialiseItem	( (void(__cdecl*)(short)) 0x004408B0 )
