////////////Hamlib Module//////////////////////
//> Hrend - HamRenderer
//> The renderer of the HFramework
//+ Creator "Patrick Hammer"
//+ License "GPL" Language "C"
//+ Changedate "15.11.2008" State "10%"
///////////////////////////////////////////////
#ifndef Hshade
#define Hshade
///////////////////////////////////////////////
//+++++++++++++++DEPENCIES+++++++++++++++++++//
#pragma region DEPENCIES
#ifndef GLFW_DLL
#include <GL/glew.h>
#define GLFW_DLL
#define GLFW_BUILD_DLL
#include <GL/glfw.h>
#endif
#ifndef GLuint
typedef unsigned int GLuint;
#endif
#include "Hfio.h"
#include "Hrend.h"
#pragma endregion
//+++++++++++++++++++++++++++++++++++++++++++//
unsigned int hshade_CreateShaderPair(char* vertexProgFileName,char* fragmentProgFileName);
void hshade_INIT();
void hshade_UPDATE();

#endif
