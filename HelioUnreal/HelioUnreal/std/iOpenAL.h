#pragma once

#include "iDefine.h"

#include "al.h"
#include "alc.h"
#pragma comment(lib, "OpenAL32")

class iOpenAL
{
public:
	iOpenAL(int count);
	virtual ~iOpenAL();

	void initBuffer(int index, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	void play(int index);
	void stop(int index = -1);
	void pause(int index = -1);
	void volume(int index, float vol);

	ALuint* gBuffer;
	ALuint* gSource;
	int bufCount;
};

struct WaveHeader0
{
	char riff[4];			// “RIFF”	Marks the file as a riff file.Characters are each 1 byte long.
	unsigned int riffSize;	// File size(integer)	Size of the overall file - 8 bytes, in bytes(32 - bit integer).Typically, you’d fill this in after creation.
	char wave[4];			// “WAVE”	File Type Header.For our purposes, it always equals “WAVE”.

	char fmt[4];			// “fmt ”	Format chunk marker. Includes trailing null
	unsigned int fmtSize;	// 16 Length of format data as listed above
	unsigned short format;	// 1 Type of format(1 is PCM) - 2 byte integer
	unsigned short channels;// 2 Number of Channels - 2 byte integer
	unsigned int samplesPerSec;// 44100	 Sample Rate - 32 bit integer.Common values are 44100 (CD), 48000 (DAT).Sample Rate = Number of Samples per second, or Hertz.
	unsigned int bytesPerSec;// 176400	 (Sample Rate * BitsPerSample * Channels) / 8.
	unsigned short blockAlign;// 4 (BitsPerSample * Channels) / 8.1 - 8 bit mono2 - 8 bit stereo / 16 bit mono4 - 16 bit stereo
	unsigned short bitPerSample;// 16 Bits per sample
};

struct WaveHeader1
{
	char data[4];			// “data”	“data” chunk header.Marks the beginning of the data section.
	unsigned int dataSize;	// File size(data)	Size of the data section.
};

