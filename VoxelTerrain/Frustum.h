#pragma once
#include "stdafx.h"
#include <GL\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cmath>

#define HALF_ANG2RAD 3.14159265358979323846/360.0

class Frustum
{

private:
	GLfloat frustum[6][4];
	GLfloat projection[16];
	GLfloat model[16];
	GLfloat clip[16];
	GLfloat t;
public:
	Frustum(void);
	~Frustum(void);
	void ObtainFrustum();
	bool IsChunkInFrustum(glm::vec3 chunkPosition, float chunkSize);
	GLboolean isChunkInProximityFrustum(glm::vec3 chunkCenter, glm::vec3 playerPosition, float radius);
};

