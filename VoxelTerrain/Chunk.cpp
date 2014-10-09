#include "stdafx.h"
#include "Chunk.h"


Chunk::Chunk(void)
{
	this->voxels =  new Block**[CHUNK_SIZE];
	for(int i=0; i<CHUNK_SIZE; i++)
	{
		this->voxels[i] = new Block*[CHUNK_SIZE];
		for(int j = 0; j<CHUNK_SIZE; j++)
		{
			this->voxels[i][j] = new Block[CHUNK_SIZE];
		}
	}
	this->empty = GL_FALSE;
	this->full = GL_FALSE;
}
Chunk::Chunk(glm::vec3 position, int ID)
{
	this->ChunkID = ID;
	this->chunkPosition = new glm::vec3(position.x, position.y, position.z);
	this->chunkCenter = glm::vec3(((chunkPosition->x)*Chunk::CHUNK_SIZE+CHUNK_SIZE)/2.0f,
									((chunkPosition->y)*Chunk::CHUNK_SIZE+CHUNK_SIZE)/2.0f,
									((chunkPosition->z)*Chunk::CHUNK_SIZE+CHUNK_SIZE)/2.0f);
	this->voxels =  new Block**[CHUNK_SIZE];
	for(int i=0; i<CHUNK_SIZE; i++)
	{
		this->voxels[i] = new Block*[CHUNK_SIZE];
		for(int j = 0; j<CHUNK_SIZE; j++)
		{
			this->voxels[i][j] = new Block[CHUNK_SIZE];
		}
	}
	this->empty = GL_FALSE;
	this->full = GL_FALSE;
	this->geometryID = -1;
}
Chunk::~Chunk(void)
{
	for(int i=0; i<CHUNK_SIZE; i++)
	{
		for(int j=0; j<CHUNK_SIZE; j++)
		{
			delete [] voxels[i][j];
		}
		delete [] voxels[i];
	}
	delete [] voxels;
}

GLboolean Chunk::IsVoxelActive(glm::vec3 voxelPosition)
{
	return this->voxels[(int)voxelPosition.x][(int)voxelPosition.y][(int)voxelPosition.z].IsActive();
}
void Chunk::SetVoxelToActive(glm::vec3 voxelPosition, int blockType)
{
	this->voxels[(int)voxelPosition.x][(int)voxelPosition.y][(int)voxelPosition.z].SetActive(true);
	this->voxels[(int)voxelPosition.x][(int)voxelPosition.y][(int)voxelPosition.z].SetBlockType(blockType);
}
int Chunk::GetBlockType(glm::vec3 voxelPosition)
{
	return this->voxels[(int)voxelPosition.x][(int)voxelPosition.y][(int)voxelPosition.z].GetBlockType();
}