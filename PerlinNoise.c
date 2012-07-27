//perlin noise directly converted C# code from //http://devmag.org.za/2009/04/25/perlin-noise/
#include "PerlinNoise.h"
float **fArray2D(int width,int height) 
{
	float **ret=(float**)calloc(width,sizeof(float*)); 
	int i;
	for(i=0; i<height; i++)
	{
		ret[i]=(float*)calloc(height,sizeof(float));
	}
	return ret;
}
float Interpolate(float x0, float x1, float alpha)
{
   return x0 * (1 - alpha) + alpha * x1;
}
float **WhiteNoise(int width, int height)
{
    float** noise = fArray2D(width, height);
	int i,j;
    for (i=0; i<width; i++)
    {
        for (j=0; j<height; j++)
        {
            noise[i][j] = frnd();
        }
    }
    return noise;
}
float** GenerateSmoothNoise(int baseNoiseLen, int baseNoiseLen0,float** baseNoise, int octave)
{
   int i,j,width = baseNoiseLen,height = baseNoiseLen0;
   float** smoothNoise = fArray2D(width, height);
   int samplePeriod = 1 << octave;
   float sampleFrequency = 1.0f / samplePeriod;
   for (i=0;i<width;i++)
   {
      //calculate the horizontal sampling indices
      int sample_i0 = (i / samplePeriod) * samplePeriod;
      int sample_i1 = (sample_i0 + samplePeriod) % width; //wrap around
      float horizontal_blend = (i - sample_i0) * sampleFrequency;
      for (j=0; j<height; j++)
      {
         //calculate the vertical sampling indices
         int sample_j0 = (j / samplePeriod) * samplePeriod;
         int sample_j1 = (sample_j0 + samplePeriod) % height; //wrap around
         float vertical_blend = (j - sample_j0) * sampleFrequency;
          //blend the top two corners
         float top = Interpolate(baseNoise[sample_i0][sample_j0],
            baseNoise[sample_i1][sample_j0], horizontal_blend);
         //blend the bottom two corners
         float bottom = Interpolate(baseNoise[sample_i0][sample_j1],
            baseNoise[sample_i1][sample_j1], horizontal_blend);
         //final blend
         smoothNoise[i][j] = Interpolate(top, bottom, vertical_blend);
      }
   }
   return smoothNoise;
}
float** GeneratePerlinNoise(int baseNoiseLen, int baseNoiseLen0,float** baseNoise, int octaveCount,int minOctave) //minOctave>=0
{
   int i,j,octave,width=baseNoiseLen,height=baseNoiseLen0;
   float ***smoothNoise=(float***)malloc(octaveCount*sizeof(float**)); //an array of 2D arrays containing
   float persistance = 0.5f;
   //generate smooth noise
   for (i=0; i<octaveCount; i++)
   {
       smoothNoise[i]=GenerateSmoothNoise(baseNoiseLen,baseNoiseLen0,baseNoise,i);
   }
   float** perlinNoise=fArray2D(width, height);
   float amplitude=1.0f;
   float totalAmplitude=0.0f;
   //blend noise together
   for (octave=octaveCount-1;octave>=minOctave;octave--)
   {
      amplitude *= persistance;
      totalAmplitude += amplitude;
      for(i=0; i<width; i++)
	  {
          for(j=0; j<height; j++)
          {
             perlinNoise[i][j]+=smoothNoise[octave][i][j] * amplitude;
          }
      }
   }
   //normalisation
   for (i=0; i<width; i++)
   {
      for (j=0; j<height; j++)
      {
         perlinNoise[i][j]/=totalAmplitude;
      }
   }
   return perlinNoise;
}
