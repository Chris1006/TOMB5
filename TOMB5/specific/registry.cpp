#include "../tomb5/pch.h"
#include "registry.h"
#include "LoadSave.h"

static bool REG_Setup;

bool REG_OpenKey(LPCSTR lpSubKey)
{
	return RegCreateKeyEx(HKEY_CURRENT_USER, lpSubKey, 0, lpClass, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &phkResult, &dwDisposition) == ERROR_SUCCESS;
}

bool OpenRegistry(LPCSTR SubKeyName)
{
	char buffer[256];

	if (!SubKeyName)
		return REG_OpenKey("Software\\Core Design\\Tomb Raider Chronicles");

	sprintf(buffer, "%s\\%s", "Software\\Core Design\\Tomb Raider Chronicles", SubKeyName);
	return REG_OpenKey(buffer);
}

void REG_CloseKey()
{
	RegCloseKey(phkResult);
}

void CloseRegistry()
{
	REG_CloseKey();
}

bool REG_KeyWasCreated()
{
	return dwDisposition == REG_CREATED_NEW_KEY;
}

void REG_DeleteValue(char* SubKeyName)
{
	RegDeleteValue(phkResult, SubKeyName);
}

void REG_WriteLong(char* SubKeyName, ulong value)
{
	RegSetValueEx(phkResult, SubKeyName, 0, REG_DWORD, (CONST BYTE*) & value, sizeof(ulong));
}

void REG_WriteBool(char* SubKeyName, bool value)
{
	ulong Lvalue;

	Lvalue = (ulong)value;
	RegSetValueEx(phkResult, SubKeyName, 0, REG_DWORD, (CONST BYTE*) & Lvalue, sizeof(ulong));
}

void REG_WriteString(char* SubKeyName, char* string, long length)
{
	long checkLength;

	if (string)
	{
		if (length < 0)
			checkLength = strlen(string);
		else
			checkLength = length;

		RegSetValueEx(phkResult, SubKeyName, 0, REG_SZ, (CONST BYTE*)string, checkLength + 1);
	}
	else
		RegDeleteValue(phkResult, SubKeyName);
}

void REG_WriteDouble(char* SubKeyName, double value)
{
	long length;
	char buf[64];

	length = sprintf(buf, "%.5f", value);
	REG_WriteString(SubKeyName, buf, length);
}

void REG_WriteVoid(char* SubKeyName, void* value, long size)
{
	if (value)
		RegSetValueEx(phkResult, SubKeyName, 0, REG_BINARY, (const BYTE*)value, size);
	else
		RegDeleteValue(phkResult, SubKeyName);
}

bool REG_ReadLong(char* SubKeyName, ulong& value, ulong defaultValue)
{
	ulong type;
	ulong cbData;

	cbData = 4;

	if (RegQueryValueEx(phkResult, SubKeyName, 0, &type, (LPBYTE)&value, &cbData) == ERROR_SUCCESS && type == REG_DWORD && cbData == 4)
		return 1;

	REG_WriteLong(SubKeyName, defaultValue);	//value not found, write default
	value = defaultValue;
	return 0;
}

bool REG_ReadBool(char* SubKeyName, bool& value, bool defaultValue)
{
	ulong type;
	ulong cbData;
	ulong data;

	cbData = 4;

	if (RegQueryValueEx(phkResult, SubKeyName, 0, &type, (LPBYTE)&data, &cbData) == ERROR_SUCCESS && type == REG_DWORD && cbData == 4)
	{
		value = (bool)data;
		return 1;
	}

	REG_WriteBool(SubKeyName, defaultValue);
	value = defaultValue;
	return 0;
}

bool REG_ReadString(char* SubKeyName, char* value, long length, char* defaultValue)
{
	ulong type;
	ulong cbData;
	long len;

	cbData = length;

	if (RegQueryValueEx(phkResult, SubKeyName, 0, &type, (LPBYTE)value, (LPDWORD)&cbData) == ERROR_SUCCESS && type == REG_SZ)
		return 1;

	if (defaultValue)
	{
		REG_WriteBool(SubKeyName, 1);	//this feels wrong.. probably because of the original overloaded names
		len = strlen(defaultValue) + 1;

		if (len > length)
		{
			len = length - 1;
			value[len] = 0;
		}

		memcpy(value, defaultValue, len);
	}
	else
		RegDeleteValue(phkResult, SubKeyName);

	return 0;
}

bool REG_ReadDouble(char* SubKeyName, double& value, double defaultValue)
{
	char buf[64];

	if (REG_ReadString(SubKeyName, buf, sizeof(buf), 0))
	{
		value = atof(buf);
		return 1;
	}

	REG_WriteDouble(SubKeyName, defaultValue);
	value = defaultValue;
	return 0;
}

bool REG_ReadVoid(char* SubKeyName, void* value, long size, void* defaultValue)
{
	ulong type;
	ulong cbData;

	cbData = size;

	if (RegQueryValueEx(phkResult, SubKeyName, 0, &type, (LPBYTE)value, &cbData) == ERROR_SUCCESS && type == REG_BINARY && size == cbData)
		return 1;

	if (defaultValue)
		REG_WriteVoid(SubKeyName, value, size);
	else
		RegDeleteValue(phkResult, SubKeyName);

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
		App.StartFlags = 32;

		REG_ReadLong((char*)"DD", (ulong&)App.DXInfo.nDD, 0);
		REG_ReadLong((char*)"D3D", (ulong&)App.DXInfo.nD3D, 0);
		REG_ReadLong((char*)"VMode", (ulong&)App.DXInfo.nDisplayMode, 0);
		REG_ReadLong((char*)"TFormat", (ulong&)App.DXInfo.nTexture, 0);
		REG_ReadLong((char*)"DS", (ulong&)App.DXInfo.nDS, 0);
		REG_ReadBool((char*)"BumpMap", App.BumpMapping, 1);
		REG_ReadBool((char*)"Filter", App.Filtering, 1);
		REG_ReadBool((char*)"DisableSound", App.SoundDisabled, 0);
		REG_ReadBool((char*)"Volumetric", App.Volumetric, 1);
		REG_ReadBool((char*)"NoFMV", fmvs_disabled, 0);

		REG_ReadBool((char*)"TextLow", val, 0);

		if (val)
			App.TextureSize = 128;

		REG_ReadBool((char*)"BumpLow", val, 0);

		if (val)
			App.BumpMapSize = 128;

		REG_ReadBool((char*)"HardWare", val, 0);

		if (val)
			App.StartFlags |= 0x90;

		REG_ReadBool((char*)"Window", val, 0);

		if (val)
			App.StartFlags |= 2;
		else
			App.StartFlags |= 1;
	}

	CloseRegistry();

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

	CloseRegistry();
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
	CloseRegistry();

	OpenRegistry("System");
	REG_WriteLong((char*)"VMode", App.DXInfo.nDisplayMode);
	REG_WriteBool((char*)"Window", (App.dx.Flags & 2) != 0);
	CloseRegistry();
}

void inject_registry(bool replace)
{
	INJECT(0x004BD860, REG_OpenKey, replace);
	INJECT(0x004BDD90, OpenRegistry, replace);
	INJECT(0x004BD8C0, REG_CloseKey, replace);
	INJECT(0x004BDE00, CloseRegistry, replace);
	INJECT(0x004BD8A0, REG_KeyWasCreated, replace);
	INJECT(0x004BDA50, REG_DeleteValue, replace);
	INJECT(0x004BD8E0, REG_WriteLong, replace);
	INJECT(0x004BD910, REG_WriteBool, replace);
	INJECT(0x004BD9E0, REG_WriteString, replace);
	INJECT(0x004BD950, REG_WriteDouble, replace);
	INJECT(0x004BD990, REG_WriteVoid, replace);
	INJECT(0x004BDA80, REG_ReadLong, replace);
	INJECT(0x004BDB00, REG_ReadBool, replace);
	INJECT(0x004BDCC0, REG_ReadString, replace);
	INJECT(0x004BDBA0, REG_ReadDouble, replace);
	INJECT(0x004BDC20, REG_ReadVoid, replace);
	INJECT(0x004BDE20, LoadSettings, replace);
	INJECT(0x004BE7E0, SaveSettings, replace);
}
