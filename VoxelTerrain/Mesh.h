#pragma once
#include <GL\glew.h>
#include <GL\GL.h>
#include <glm.hpp>
#include <vector>

#define VERTEX_BUFFER 0
#define INDEX_BUFFER 1
#define NORMAL_BUFFER 2
#define COLOR_BUFFER 3

class CMesh
{
public:
	GLuint meshID,indexCount,vertexCount;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
private:
	std::vector<GLfloat>* meshVertexData; 
	std::vector<GLuint>* meshIndexData;
	std::vector<GLfloat> meshNormalData;
	std::vector<GLfloat> meshColorData;
public:
	CMesh(void);
	CMesh(GLuint meshID);
	~CMesh(void);
	void addVertexToMesh(glm::vec3 vertexPosition);
	void addTriangleToMesh(glm::vec3 indexVector);
	void addQuadToMesh(glm::vec4 indexVector);
	void addNormalForTriangle(glm::vec3 normalVector);
	void addColorForVert(glm::vec4 colorVector);
	void generateVertexBufferData();
	void generateIndexBufferData();
	void generateNormalBufferData();
	void generateColorBufferData();
	GLuint* getBuffer(int bufferType);
};

