#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh(void)
{
	meshID = 0;
}
CMesh::CMesh(GLuint meshID)
{
	this->meshID = meshID;
	this->meshVertexData = new std::vector<GLfloat>();
	this->meshIndexData = new std::vector<GLuint>();

}
/* ********************* TODO: Add proper cleanup ********************* */
CMesh::~CMesh(void)
{
	delete meshVertexData;
	delete meshIndexData;
	glDeleteBuffers(1,&vertexBuffer);
	glDeleteBuffers(1,&indexBuffer);
	glDeleteBuffers(1,&normalBuffer);
	glDeleteBuffers(1,&colorBuffer);
}
void CMesh::addVertexToMesh(glm::vec3 vertexPosition)
{
	this->meshVertexData->push_back((GLfloat)vertexPosition.x);
	this->meshVertexData->push_back((GLfloat)vertexPosition.y);
	this->meshVertexData->push_back((GLfloat)vertexPosition.z);
}
void CMesh::addTriangleToMesh(glm::vec3 indexVector)
{
	this->meshIndexData->push_back((GLuint)indexVector.x);
	this->meshIndexData->push_back((GLuint)indexVector.y);
	this->meshIndexData->push_back((GLuint)indexVector.z);
}
void CMesh::addQuadToMesh(glm::vec4 indexVector)
{
	this->meshIndexData->push_back((GLuint)indexVector.x);
	this->meshIndexData->push_back((GLuint)indexVector.y);
	this->meshIndexData->push_back((GLuint)indexVector.z);
	this->meshIndexData->push_back((GLuint)indexVector.w);
}
void CMesh::addNormalForTriangle(glm::vec3 normalVector)
{
	this->meshNormalData.push_back((GLfloat)normalVector.x);
	this->meshNormalData.push_back((GLfloat)normalVector.y);
	this->meshNormalData.push_back((GLfloat)normalVector.z);
}
void CMesh::addColorForVert(glm::vec4 colorVector)
{
	this->meshColorData.push_back((GLfloat)colorVector.r);
	this->meshColorData.push_back((GLfloat)colorVector.g);
	this->meshColorData.push_back((GLfloat)colorVector.b);
	this->meshColorData.push_back((GLfloat)colorVector.a);
}
void CMesh::generateVertexBufferData()
{
	glGenBuffers(1,&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*meshVertexData->size(),&(meshVertexData->at(0)),GL_STATIC_DRAW);
	this->vertexCount = meshVertexData->size();
}
void CMesh::generateIndexBufferData()
{
	glGenBuffers(1,&indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*meshIndexData->size(),&(meshIndexData->at(0)),GL_STATIC_DRAW);
	this->indexCount = meshIndexData->size();
}
void CMesh::generateNormalBufferData()
{
	glGenBuffers(1,&normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*meshNormalData.size(),&meshNormalData[0],GL_STATIC_DRAW);
}
void CMesh::generateColorBufferData()
{
	glGenBuffers(1,&colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,colorBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*meshColorData.size(),&meshColorData[0],GL_STATIC_DRAW);
}
GLuint* CMesh::getBuffer(int bufferType)
{
	if(bufferType == VERTEX_BUFFER)
		return &vertexBuffer;
	else if(bufferType == INDEX_BUFFER)
		return &indexBuffer;
	else if(bufferType == NORMAL_BUFFER)
		return &normalBuffer;
	else
		return NULL;
}
