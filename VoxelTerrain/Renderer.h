#pragma once

#include <GL\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>
#include "Mesh.h"

#define BUFFER_OFFSET(i) ((void*)(i))

class Renderer
{
private:
	enum{lightPosSize = 4};
	GLfloat lightpos[lightPosSize];
public: 
	float voxelLength;
	float voxelHeight;
	float voxelWidth;
public:
	Renderer(void);
	~Renderer(void);
	void TranslateWorldMatrix(glm::vec3 translationVector);
	void TranslateWorldMatrix(float x, float y, float z);
	void RotateWorldMatrix(glm::vec3 rotationVector);
	void SetRenderMode(UINT mode);
	void EnableImmediateDraw(UINT mode);
	void DisableImmediateDraw();
	void ImmediateColorAlpha(float r,float g, float b, float a);
	void ImmediateNormal(float x, float y, float z);
	void ImmediateVertex(float x, float y, float z);
	void SetupLights(glm::vec3 layersCount,GLint chunkSize);
	void UpdateLights(glm::vec3 newPosition);
};

