#include "iOpenAL.h"

#include "iStd.h"

iOpenAL::iOpenAL(int count)
{

	ALCdevice* newDevice = alcOpenDevice(NULL);
	if (newDevice == NULL)
		return;

	ALCint attr[] = {
		ALC_FREQUENCY, 22050,
		0
	};
	ALCcontext* newContext = alcCreateContext(newDevice, attr);
	if (newContext == NULL)
		return;
	alcMakeContextCurrent(newContext);

	bufCount = count;

	gBuffer = new ALuint[bufCount];
	alGenBuffers(bufCount, gBuffer);
	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("Error alGenBuffers\n");
		return;
	}

	gSource = new ALuint[bufCount];
	alGenSources(bufCount, gSource);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		printf("Error alGenSources\n");
		return;
	}

	//alcMakeContextCurrent(NULL);

	// clear any errors
	alGetError();
}

iOpenAL::~iOpenAL()
{
	//ALCuint* returnNames = new ALCuint[bufCount];
	//alDeleteBuffers(bufCount, returnNames);
	//alDeleteSources(bufCount, returnNames);
	//delete returnNames;
	alDeleteBuffers(bufCount, gBuffer);
	alDeleteSources(bufCount, gSource);

	ALCcontext* context = alcGetCurrentContext();
	ALCdevice* device = alcGetContextsDevice(context);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void iOpenAL::initBuffer(int index, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	int bufLen;
	char* buf = loadFile(bufLen, szText);

	WaveHeader0* head0 = (WaveHeader0*)buf;
	WaveHeader1* head1 = (WaveHeader1*)&buf[sizeof(WaveHeader0) + (head0->fmtSize - 16)];

	ALuint buffer = 0;
	ALuint format = 0;
	switch (head0->bitPerSample) {
	case 8:	format = (head0->channels == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8); break;
	case 16:format = (head0->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16); break;
	default: delete buf; return;
	}

	ALsizei samplePerSec = head0->samplesPerSec;

	char* data = &buf[sizeof(WaveHeader0) + (head0->fmtSize - 16) + sizeof(WaveHeader1)];
	int dataLength = head1->dataSize;

	alBufferData(gBuffer[index], format, data, dataLength, samplePerSec);
	
	delete buf;

	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error initBuffer %s\n", szText);
}

void iOpenAL::initSource(int index, bool repeat)
{
	ALCenum error;
	alGetError();// clear any errors

	alSourcei(gSource[index], AL_LOOPING, repeat);

	const ALfloat sourcePosition[] = { 0.0f, 0.0f, 0.0f };// x, y, distance
	alSourcefv(gSource[index], AL_POSITION, sourcePosition);

	alSourcef(gSource[index], AL_REFERENCE_DISTANCE, 50);

	alSourcei(gSource[index], AL_BUFFER, gBuffer[index]);

	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error initSource %d\n", index);
}


void iOpenAL::play(int index)
{
	alSourcePlay(gSource[index]);
	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error play %d\n", index);
}

void iOpenAL::stop(int index)
{
	alSourceStop(gSource[index]);
	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error stop %d\n", index);
}

void iOpenAL::pause(int index)
{
	alSourcePause(gSource[index]);
	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error pause %d\n", index);
}

void iOpenAL::volume(int index, float vol)
{
	alSourcef(gSource[index], AL_GAIN, vol);
	ALCenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
		printf("Error volume %d\n", index);
}
