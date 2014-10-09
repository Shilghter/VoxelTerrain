#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer(void)
{
	this->voxelHeight = 1.0f;
	this->voxelWidth  = 1.0f;
	this->voxelLength = 1.0f;
}


Renderer::~Renderer(void)
{
	
}

void Renderer::TranslateWorldMatrix(glm::vec3 translationVector)
{
	glTranslatef((float)translationVector.x,(float)translationVector.y,(float)translationVector.z);
}
void Renderer::TranslateWorldMatrix(float x, float y, float z)
{
	glTranslatef(x,y,z);
}
void Renderer::RotateWorldMatrix(glm::vec3 rotationVector)
{
	glRotatef((float)rotationVector.y,1.0f,0.0f,0.0f);
	glRotatef((float)rotationVector.x,0.0f,1.0f,0.0f);
	glRotatef((float)rotationVector.z,0.0f,0.0f,1.0f);
}
void Renderer::SetRenderMode(UINT mode)
{
	glPolygonMode(GL_FRONT_AND_BACK,mode);
}
void Renderer::EnableImmediateDraw(UINT mode)
{
	glBegin(mode);
}
void Renderer::DisableImmediateDraw()
{
	glEnd();
}
void Renderer::ImmediateVertex(float x, float y, float z)
{
	glVertex3f(x,y,z);
}
void Renderer::ImmediateColorAlpha(float r,float g, float b, float a)
{
	glColor4f(r,g,b,a);
}
void Renderer::ImmediateNormal(float x, float y, float z)
{
	glNormal3f(x,y,z);
}
void Renderer::SetupLights(glm::vec3 layersCount, GLint chunkSize)
{
	glShadeModel(GL_FLAT);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	//int width = layersCount.x*chunkSize;
	//int height = layersCount.z*chunkSize;
	//lightpos[0] = 0.0f;
	//lightpos[1] = 0.0f;
	//lightpos[2] = 40.0f;
	lightpos[3] = 1.0f;
	//glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
}
void Renderer::UpdateLights(glm::vec3 newPosition)
{
	lightpos[0] = newPosition.x;
	lightpos[1] = newPosition.y;
	lightpos[2] = newPosition.z;
	glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
}