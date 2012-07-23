#include "Hshade.h"

GLuint hshade_CreateShaderPair(char *vertexProgFileName,char *fragmentProgFileName)
{
	char *fs,*vs;
	GLuint p;
	GLuint v=glCreateShader(GL_VERTEX_SHADER);
	GLuint f=glCreateShader(GL_FRAGMENT_SHADER);
	vs=hfio_textFileRead(vertexProgFileName);
	fs=hfio_textFileRead(fragmentProgFileName);
	glShaderSource(v,1,(const GLchar**)&vs,NULL);
	glShaderSource(f,1,(const GLchar**)&fs,NULL);
	free(vs);free(fs);
	glCompileShader(v);
	glCompileShader(f);
	p=glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);
	glLinkProgram(p);

	return p;
}

void hshade_INIT(){}
void hshade_UPDATE(){}
