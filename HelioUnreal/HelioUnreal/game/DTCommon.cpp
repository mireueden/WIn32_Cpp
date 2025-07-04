#include "DTCommon.h"

#if 1

void loadAudio() {}
void freeAudio() {}
void playAudio(int index) {}
void stopAudio(int index) {}
void volumeAudio(float sfx, float bgm) {}

#else

iOpenAL* openal;

struct SndInfo
{
	const char* path;
	bool repeat;
	float vol;
};

void loadAudio()
{
	SndInfo si[3] = {
		//{ "btn0", false, 1.0f },
		//{ "btn1", false, 1.0f },
		//{ "bgm", true, 0.5f },
		{ "btn0", false, 0.0f },
		{ "btn1", false, 0.0f },
		{ "bgm", true, 0.0f },
	};

	openal = new iOpenAL(3);
	for (int i = 0; i < 3; i++)
	{
		SndInfo* info = &si[i];
		openal->initBuffer(i, "assets/snd/%s.wav", info->path);
		openal->initSource(i, info->repeat);
		openal->volume(i, info->vol);
	}
}

void freeAudio()
{
	delete openal;
}

void playAudio(int index)
{
	openal->play(index);
}

void stopAudio(int index)
{
	if (index == -1)
	{
		for (int i = 0; i < openal->bufCount; i++)
			openal->stop(i);
		return;
	}
	openal->stop(index);
}

void volumeAudio(float sfx, float bgm)
{
}

#endif