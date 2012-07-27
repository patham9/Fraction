//perlin noise directly converted C# code from //http://devmag.org.za/2009/04/25/perlin-noise/
#define NULL 0
#include "Hutil.h" 
float **fArray2D(int width,int height);
float Interpolate(float x0, float x1, float alpha);
float **WhiteNoise(int width, int height);
float** GenerateSmoothNoise(int baseNoiseLen, int baseNoiseLen0,float** baseNoise, int octave);
float** GeneratePerlinNoise(int baseNoiseLen, int baseNoiseLen0,float** baseNoise, int octaveCount,int minOctave);
