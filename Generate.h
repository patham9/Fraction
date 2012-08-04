#ifndef Generator_H
#define Generator_H

#include "Hamlib/Hauto.h"
#include "Hamlib/Hutil.h" 

void Generate_World();
//perlin noise directly converted C# code from //http://devmag.org.za/2009/04/25/perlin-noise/
float **fArray2D(int width,int height);
float Generate_Interpolate(float x0, float x1, float alpha);
float **Generate_WhiteNoise(int width, int height);
float** Generate_SmoothNoise(int baseNoiseLen, int baseNoiseLen0,float** baseNoise, int octave);
float** Generate_PerlinNoise(int baseNoiseLen, int baseNoiseLen0,float** baseNoise, int octaveCount,int minOctave);

#endif
