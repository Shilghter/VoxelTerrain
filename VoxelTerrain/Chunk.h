#pragma once
#include "stdafx.h"
#include "Block.h"
#include "Renderer.h"

class Chunk
{
public:
	static const int CHUNK_SIZE = 16;
	GLboolean empty,full;
	glm::vec3* chunkPosition;
	glm::vec3 chunkCenter;
	GLuint ChunkID;
	GLint geometryID;
public:
	Chunk(void);
	Chunk(glm::vec3, int ID);
	~Chunk(void);
	int GetBlockType(glm::vec3 voxelPosition);
	GLboolean IsVoxelActive(glm::vec3 voxelPosition);
	void SetVoxelToActive(glm::vec3 voxelPosition,int blockType);
private:
	Block*** voxels;
};

