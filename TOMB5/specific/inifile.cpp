#include "../tomb5/pch.h"
#include "inifile.h"
#include "LoadSave.h"
#include "winmain.h"
#include "input.h"
#include "setupdlg.h"

static HKEY phkResult;
static bool REG_Setup;
static LPCSTR lpSubKey = (char*)"";
static LPCSTR INIFILE = (char*)".\\tomb5.ini";


bool OpenRegistry(LPCSTR SubKeyName)
{
	lpSubKey = SubKeyName;
	return 1;
}

LPCSTR toString(ulong l)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%lu", l);
	return buffer;
}

LPCSTR toString(bool b)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%d", b);
	return buffer;
}

LPCSTR toString(float f)
{
	char buffer[64];
	snprintf(buffer, sizeof(buffer), "%.5f", f);
	return (char*)"";
}

ulong strToUl(LPCSTR str) {
	return atol(str);
}

bool strToB(LPCSTR str) {

	return atoi(str);
}

float strToF(LPCSTR str) {
	return atof(str);
}


void REG_WriteLong(char* SubKeyName, ulong value)
{
	WritePrivateProfileString(lpSubKey, SubKeyName, toString(value), INIFILE);
}


void REG_WriteBool(char* SubKeyName, bool value)
{
	ulong Lvalue;


	Lvalue = (ulong)value;
	WritePrivateProfileString(lpSubKey, SubKeyName, toString(Lvalue), INIFILE);
}

bool REG_ReadLong(char* SubKeyName, ulong& value, ulong defaultValue)
{
	char buffer[64];
	DWORD Read = GetPrivateProfileString(lpSubKey, SubKeyName, toString(defaultValue), buffer, sizeof(buffer), INIFILE);


	if (Read > 0) {

		value = strToUl(buffer);
		return 1;
	}

	REG_WriteLong(SubKeyName, defaultValue);
	value = defaultValue;
	return 0;
}

bool REG_ReadBool(char* SubKeyName, bool& value, bool defaultValue)
{
	char buffer[64];
	DWORD Read = GetPrivateProfileString(lpSubKey, SubKeyName, toString(defaultValue), buffer, sizeof(buffer), INIFILE);


	if (Read > 0) {

		value = strToB(buffer);
		return 1;
	}

	REG_WriteBool(SubKeyName, defaultValue);
	value = defaultValue;
	return 0;
}

bool LoadSettings()
{
	ulong key;
	bool val;

	if (!OpenRegistry("System"))
		return 0;

	REG_ReadBool((char*)"Setup", REG_Setup, 0);

	if (REG_Setup)
	{
		App.TextureSize = 256;
		App.BumpMapSize = 256;
		App.StartFlags = DXF_FPUSETUP;

		REG_ReadLong((char*)"DD", (ulong&)App.DXInfo.nDD, 0);
		REG_ReadLong((char*)"D3D", (ulong&)App.DXInfo.nD3D, 0);
		REG_ReadLong((char*)"VMode", (ulong&)App.DXInfo.nDisplayMode, 0);
		REG_ReadLong((char*)"TFormat", (ulong&)App.DXInfo.nTexture, 0);
		REG_ReadLong((char*)"DS", (ulong&)App.DXInfo.nDS, 0);
		App.BumpMapping = 0;
		REG_ReadBool((char*)"Filter", App.Filtering, 1);
		REG_ReadBool((char*)"DisableSound", App.SoundDisabled, 0);
		REG_ReadBool((char*)"Volumetric", App.Volumetric, 1);
		REG_ReadBool((char*)"NoFMV", fmvs_disabled, 0);

		REG_ReadBool((char*)"TextLow", val, 0);

		if (val)
			App.TextureSize = 128;

		App.StartFlags |= DXF_HWR | DXF_ZBUFFER;

		REG_ReadBool((char*)"Window", val, 0);

		if (val)
			App.StartFlags |= DXF_WINDOWED;
		else
			App.StartFlags |= DXF_FULLSCREEN;
	}


	OpenRegistry("Game");

	REG_ReadLong((char*)"Key0", key, layout[0][0]);
	layout[1][0] = (short)key;

	REG_ReadLong((char*)"Key1", key, layout[0][1]);
	layout[1][1] = (short)key;

	REG_ReadLong((char*)"Key2", key, layout[0][2]);
	layout[1][2] = (short)key;

	REG_ReadLong((char*)"Key3", key, layout[0][3]);
	layout[1][3] = (short)key;

	REG_ReadLong((char*)"Key4", key, layout[0][4]);
	layout[1][4] = (short)key;

	REG_ReadLong((char*)"Key5", key, layout[0][5]);
	layout[1][5] = (short)key;

	REG_ReadLong((char*)"Key6", key, layout[0][6]);
	layout[1][6] = (short)key;

	REG_ReadLong((char*)"Key7", key, layout[0][7]);
	layout[1][7] = (short)key;

	REG_ReadLong((char*)"Key8", key, layout[0][8]);
	layout[1][8] = (short)key;

	REG_ReadLong((char*)"Key9", key, layout[0][9]);
	layout[1][9] = (short)key;

	REG_ReadLong((char*)"Key10", key, layout[0][10]);
	layout[1][10] = (short)key;

	REG_ReadLong((char*)"Key11", key, layout[0][11]);
	layout[1][11] = (short)key;

	REG_ReadLong((char*)"Key12", key, layout[0][12]);
	layout[1][12] = (short)key;

	REG_ReadLong((char*)"Key13", key, layout[0][13]);
	layout[1][13] = (short)key;

	REG_ReadLong((char*)"Key14", key, layout[0][14]);
	layout[1][14] = (short)key;

	REG_ReadLong((char*)"Key15", key, layout[0][15]);
	layout[1][15] = (short)key;

	REG_ReadLong((char*)"Key16", key, layout[0][16]);
	layout[1][16] = (short)key;

	REG_ReadLong((char*)"Key17", key, layout[0][17]);
	layout[1][17] = (short)key;

	REG_ReadLong((char*)"JDck", (ulong&)jLayout[0], 5);
	REG_ReadLong((char*)"JDsh", (ulong&)jLayout[1], 3);
	REG_ReadLong((char*)"JWlk", (ulong&)jLayout[2], 4);
	REG_ReadLong((char*)"JJmp", (ulong&)jLayout[3], 0);
	REG_ReadLong((char*)"JAct", (ulong&)jLayout[4], 1);
	REG_ReadLong((char*)"JDrw", (ulong&)jLayout[5], 2);
	REG_ReadLong((char*)"JFlr", (ulong&)jLayout[6], 9);
	REG_ReadLong((char*)"JLok", (ulong&)jLayout[7], 6);
	REG_ReadLong((char*)"JRol", (ulong&)jLayout[8], 7);
	REG_ReadLong((char*)"JInv", (ulong&)jLayout[9], 8);

	REG_ReadLong((char*)"MusicVolume", (ulong&)MusicVolume, 80);
	REG_ReadLong((char*)"SFXVolume", (ulong&)SFXVolume, 90);
	REG_ReadLong((char*)"ControlMethod", (ulong&)ControlMethod, 0);
	REG_ReadLong((char*)"SoundQuality", (ulong&)SoundQuality, 1);
	REG_ReadLong((char*)"AutoTarget", (ulong&)App.AutoTarget, 1);
	REG_ReadLong((char*)"WindowX", (ulong&)App.dx.rScreen.left, 0);
	REG_ReadLong((char*)"WindowY", (ulong&)App.dx.rScreen.top, 0);

	CheckKeyConflicts();
	return REG_Setup;
}

void SaveSettings()
{
	OpenRegistry("Game");
	REG_WriteLong((char*)"Key0", layout[1][0]);
	REG_WriteLong((char*)"Key1", layout[1][1]);
	REG_WriteLong((char*)"Key2", layout[1][2]);
	REG_WriteLong((char*)"Key3", layout[1][3]);
	REG_WriteLong((char*)"Key4", layout[1][4]);
	REG_WriteLong((char*)"Key5", layout[1][5]);
	REG_WriteLong((char*)"Key6", layout[1][6]);
	REG_WriteLong((char*)"Key7", layout[1][7]);
	REG_WriteLong((char*)"Key8", layout[1][8]);
	REG_WriteLong((char*)"Key9", layout[1][9]);
	REG_WriteLong((char*)"Key10", layout[1][10]);
	REG_WriteLong((char*)"Key11", layout[1][11]);
	REG_WriteLong((char*)"Key12", layout[1][12]);
	REG_WriteLong((char*)"Key13", layout[1][13]);
	REG_WriteLong((char*)"Key14", layout[1][14]);
	REG_WriteLong((char*)"Key15", layout[1][15]);
	REG_WriteLong((char*)"Key16", layout[1][16]);
	REG_WriteLong((char*)"Key17", layout[1][17]);

	REG_WriteLong((char*)"JDck", jLayout[0]);
	REG_WriteLong((char*)"JDsh", jLayout[1]);
	REG_WriteLong((char*)"JWlk", jLayout[2]);
	REG_WriteLong((char*)"JJmp", jLayout[3]);
	REG_WriteLong((char*)"JAct", jLayout[4]);
	REG_WriteLong((char*)"JDrw", jLayout[5]);
	REG_WriteLong((char*)"JFlr", jLayout[6]);
	REG_WriteLong((char*)"JLok", jLayout[7]);
	REG_WriteLong((char*)"JRol", jLayout[8]);
	REG_WriteLong((char*)"JInv", jLayout[9]);

	REG_WriteLong((char*)"ControlMethod", ControlMethod);
	REG_WriteLong((char*)"MusicVolume", MusicVolume);
	REG_WriteLong((char*)"SFXVolume", SFXVolume);
	REG_WriteLong((char*)"SoundQuality", SoundQuality);
	REG_WriteLong((char*)"AutoTarget", App.AutoTarget);
	REG_WriteLong((char*)"WindowX", App.dx.rScreen.left);
	REG_WriteLong((char*)"WindowY", App.dx.rScreen.top);

	OpenRegistry("System");
	REG_WriteLong((char*)"VMode", App.DXInfo.nDisplayMode);
	REG_WriteBool((char*)"Window", (App.dx.Flags & DXF_WINDOWED) != 0);

}

bool SaveSetup(HWND hDlg)
{
	OpenRegistry("System");

	REG_WriteLong((char*)"DD", SendMessage(GetDlgItem(hDlg, IDC_GRAPHICS_ADAPTER), CB_GETCURSEL, 0, 0));
	REG_WriteLong((char*)"D3D", SendMessage(GetDlgItem(hDlg, IDC_D3D), CB_GETCURSEL, 0, 0));
	REG_WriteLong((char*)"VMode", SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), CB_GETITEMDATA, SendMessage(GetDlgItem(hDlg, IDC_RESOLUTION), CB_GETCURSEL, 0, 0), 0));
	REG_WriteLong((char*)"DS", SendMessage(GetDlgItem(hDlg, IDC_SOUND_ADAPTER), CB_GETCURSEL, 0, 0));
	REG_WriteLong((char*)"TFormat", SendMessage(GetDlgItem(hDlg, IDC_TFORMAT), CB_GETCURSEL, 0, 0));

	REG_WriteBool((char*)"Filter", SendMessage(GetDlgItem(hDlg, IDC_BILINEAR), BM_GETCHECK, 0, 0));
	REG_WriteBool((char*)"DisableSound", SendMessage(GetDlgItem(hDlg, IDC_DISABLE_SOUND), BM_GETCHECK, 0, 0));
	REG_WriteBool((char*)"TextLow", SendMessage(GetDlgItem(hDlg, IDC_LOW_QUALITY_TEX), BM_GETCHECK, 0, 0));
	REG_WriteBool((char*)"Window", SendMessage(GetDlgItem(hDlg, IDC_WINDOWED), BM_GETCHECK, 0, 0));
	REG_WriteBool((char*)"NoFMV", SendMessage(GetDlgItem(hDlg, IDC_DISABLE_FMV), BM_GETCHECK, 0, 0));
	REG_WriteBool((char*)"Setup", 1);


	return 1;
}
