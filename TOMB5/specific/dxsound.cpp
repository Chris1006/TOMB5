#include "../tomb5/pch.h"
#include "dxsound.h"
#include "function_stubs.h"
#include "dxshell.h"

#define DSPrimary	VAR_U_(0x0086CAF0, LPDIRECTSOUNDBUFFER)
#define DS_Samples	ARRAY_(0x0086CAF8, DS_SAMPLE, [32])

bool DXChangeOutputFormat(long nSamplesPerSec, bool force)
{
	WAVEFORMATEX pcfxFormat;
	static long lastSPC;

	if (!force && lastSPC == nSamplesPerSec)
		return 1;

	lastSPC = nSamplesPerSec;
	pcfxFormat.wFormatTag = WAVE_FORMAT_PCM;
	pcfxFormat.nChannels = 2;
	pcfxFormat.nSamplesPerSec = nSamplesPerSec;
	pcfxFormat.nAvgBytesPerSec = 4 * nSamplesPerSec;
	pcfxFormat.nBlockAlign = 4;
	pcfxFormat.wBitsPerSample = 16;
	pcfxFormat.cbSize = 0;
	S_SoundStopAllSamples();

	if (DSPrimary && DXAttempt(DSPrimary->SetFormat(&pcfxFormat)) != DS_OK)
	{
		Log(1, "Can't set sound output format to %d", pcfxFormat.nSamplesPerSec);
		return 0;
	}

	return 1;
}

void DSChangeVolume(long num, long volume)
{
	if (DS_Samples[num].buffer)
		DS_Samples[num].buffer->SetVolume(volume);
}

void DSAdjustPitch(long num, long pitch)
{
	ulong frequency;

	if (DS_Samples[num].buffer)
	{
		frequency = ulong((float)pitch / 65536.0F * 22050.0F);

		if (frequency < 100)
			frequency = 100;
		else if (frequency > 100000)
			frequency = 100000;

		DS_Samples[num].buffer->SetFrequency(frequency);
	}
}

void DSAdjustPan(long num, long pan)
{
	if (DS_Samples[num].buffer)
	{
		if (pan < 0)
		{
			if (pan < -0x4000)
				pan = -0x4000 - pan;
		}
		else if (pan > 0 && pan > 0x4000)
			pan = 0x8000 - pan;

		pan >>= 4;
		DS_Samples[num].buffer->SetPan(pan);
	}
}

void inject_dxsound(bool replace)
{
	INJECT(0x004A2E30, DXChangeOutputFormat, replace);
	INJECT(0x004A2F10, DSChangeVolume, replace);
	INJECT(0x004A2F40, DSAdjustPitch, replace);
	INJECT(0x004A2FB0, DSAdjustPan, replace);
}
