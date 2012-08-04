#include "Hsound.h"

static ALCdevice *curDevice = NULL;
static ALCcontext *curContext = NULL;
struct Sounds
{
	ALuint buffer;
}sounds[maxsounds];
struct Sources
{
	ALuint source;
	ALfloat SourcePos[3];
	ALfloat SourceVel[3];
}sources[maxsources];

ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

void hsound_SetListenerPosition(float lpx,float lpy,float lpz, 
								float lvx, float lvy,float lvz,
								float orix, float oriy, float oriz,
								float orix2, float oriy2, float oriz2)
{
	ListenerPos[0]=lpx; ListenerPos[1]=lpy; ListenerPos[2]=lpz;
	ListenerVel[0]=lvx; ListenerVel[1]=lvy; ListenerVel[2]=lvz;
	ListenerOri[0]=orix; ListenerOri[1]=oriy; ListenerOri[2]=oriz;
	ListenerOri[3]=orix2; ListenerOri[4]=oriy2; ListenerOri[5]=oriz2;

	alListenerfv(AL_POSITION,ListenerPos);
	alListenerfv(AL_ORIENTATION,ListenerOri);
	alListenerfv(AL_VELOCITY,ListenerVel);
}
/*	own device context
unsigned char ContextInit(ALchar *device)
{
curDevice = alcOpenDevice(device);
if(curDevice==NULL){return 0; exit(-1);}
alcGetError(curDevice);
curContext=alcCreateContext(curDevice,NULL);
if(curContext==NULL){return 0; exit(-1);}
alcMakeContextCurrent(curContext);
return 1;
}
*/
int hsound_AddSound(const char * filename)
{
	sounds[hsound.nSounds].buffer=alutCreateBufferFromFile(filename); 
	if(alGetError()!=AL_NO_ERROR){return -1;}
	return hsound.nSounds++;
}
int hsound_AddEmptySound(int lenght)
{
	sounds[hsound.nSounds].buffer=alutCreateBufferWaveform(0,0,0,lenght);
	if(alGetError()!=AL_NO_ERROR){return -1;}
	return hsound.nSounds++;
}

int hsound_AddSource(int sound,ALboolean loop,float SourcePosX,float SourcePosY,float SourcePosZ,float SourcevX,float SourcevY,float SourcevZ)
{
	alGenSources(1,&sources[hsound.nSources].source);

	if(alGetError()!=AL_NO_ERROR){return -1; exit(-1);}
	sources[hsound.nSources].SourceVel[0]=SourcevX;
	sources[hsound.nSources].SourceVel[1]=SourcevY;
	sources[hsound.nSources].SourceVel[2]=SourcevZ;
	sources[hsound.nSources].SourcePos[0]=SourcePosX;
	sources[hsound.nSources].SourcePos[1]=SourcePosY;
	sources[hsound.nSources].SourcePos[2]=SourcePosZ;
	alSourcei(sources[hsound.nSources].source,AL_BUFFER,sounds[sound].buffer);
	alSourcef(sources[hsound.nSources].source,AL_GAIN,1.0);	//--
	alSourcef(sources[hsound.nSources].source,AL_PITCH,1.0);
	alSourcefv(sources[hsound.nSources].source,AL_VELOCITY,sources[hsound.nSources].SourceVel);
	alSourcefv(sources[hsound.nSources].source,AL_POSITION,sources[hsound.nSources].SourcePos);
	alSourcei(sources[hsound.nSources].source,AL_LOOPING,loop);
	if(alGetError()!=AL_NO_ERROR){return -1; exit(-1);}

	return hsound.nSources++;
}
void hsound_Play(int source)
{
	alSourcePlay(sources[source].source);
}
void hsound_Stop(int source)
{
	alSourceStop(sources[source].source);
}
void hsound_Pause(int source)
{
	alSourcePause(sources[source].source);
}
void hsound_Rewind(int source)
{
	alSourceRewind(sources[source].source);
}

///////////STANDARD-MODULE-FUNCTIONS///////////
void hsound_KILL(void)
{
	alcDestroyContext(curContext);
	alcCloseDevice(curDevice);
}
void hsound_INIT()
{
	hsound.mode=MODE;
	hsound.nSources=0;
	hsound.nSounds=0;
	alutInit(NULL, 0);
	//ContextInit(0);

	alGetError();
	atexit(hsound_KILL);
	hsound_SetListenerPosition(0,0,0,0,0,0,0,0,-1.0,0,1.0,0);
	hsound.init=1;
}
void hsound_UPDATE(){if(hsound.init){}}
void hsound_CHECK(){}
///////////////////////////////////////////////

