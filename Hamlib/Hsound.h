////////////Hamlib Module//////////////////////
//> Hsound - HamSound
//> A universal sound module based on openAL
//+ Creator "Patrick Hammer"
//+ License "GPL" Language "C80"
//+ Changedate "08.09.2008" State "80%"
///////////////////////////////////////////////
#ifndef Hsound
#define Hsound
///////////////////////////////////////////////

//---------------DEFINES---------------------//
#pragma region DEFINES
#define MODE 0 //0=speed 1=safeness/quality
#define EXT  1 //not only std
#define maxsounds 255
#define maxsources 25500
#pragma endregion
//-------------------------------------------//

//+++++++++++++++DEPENCIES+++++++++++++++++++//
#pragma region DEPENCIES
//STD:
#include <stdio.h>
#include <stdlib.h>
//OTHER:
#if EXT==1
#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>
#endif
//SAME:
#pragma endregion
//+++++++++++++++++++++++++++++++++++++++++++//

/////////////////MODULE-DATA///////////////////
struct Hsound
{
	int mode; //will be set to MODE on init 
	int nSources; // 0
	int nSounds; // 0
	int init; //0
}
hsound;
///////////////////////////////////////////////

//:::::::::::::::MODULE-IO::::::::::::::::::://
void hsound_INIT();
void hsound_KILL();
void hsound_UPDATE();
void hsound_CHECK();
//::
int hsound_AddSound(const char * filename);
int hsound_AddSource(int sound,ALboolean loop,float SourcePosX,float SourcePosY,float SourcePosZ,float SourcevX,float SourcevY,float SourcevZ);
void hsound_Play(int source);
void hsound_Stop(int source);
void hsound_Pause(int source);
void hsound_Rewind(int source);
void hsound_SetListenerPosition(float lpx,float lpy,float lpz, 
								float lvx, float lvy,float lvz,
								float orix, float oriy, float oriz,
								float orix2, float oriy2, float oriz2);

//::::::::::::::::::::::::::::::::::::::::::://
#endif
/*................COMMENTS...................//
>>todo:	HamOS compatibility

>>done:

>>others:

//..........................................*/
