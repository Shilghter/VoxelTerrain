#include "stdafx.h"
#include "GeometryManager.h"


GeometryManager::GeometryManager(void)
{
	this->meshArray = new std::vector<CMesh*>();
	this->pRenderer = new Renderer();
	this->statistics = glm::vec4(0,0,0,0);
}

/* ************ TODO: Add proper cleanup ************** */
GeometryManager::~GeometryManager(void)
{
	for(auto i = meshArray->begin(); i < meshArray->end(); i++)
	{
			delete [] *i;
	}
}
void GeometryManager::AddMesh(GLuint chunkID, GLint* geometryID)
{
	this->meshArray->push_back(new CMesh(chunkID));
	*geometryID = this->meshArray->size()-1; //Return index to CMesh created for ChunkID
}
void GeometryManager::CreateUnindexedTriangleGeometry(Chunk* chunk)
{
	voxelCounter = 0; 
	for(int x = 0; x < Chunk::CHUNK_SIZE; x++)
	{
		for(int y = 0; y < Chunk::CHUNK_SIZE; y++)
		{
			for(int z = 0; z < Chunk::CHUNK_SIZE; z++)
			{
				if(chunk->IsVoxelActive(glm::vec3(x,y,z)) == GL_TRUE)
				{
					CreateVoxel(chunk, glm::vec3(x,y,z));
					voxelCounter += 1;
				}	
			}
		}
	}
	statistics.x += voxelCounter; // Summing up voxels for every generated chunk
	statistics.y += voxelCounter*12; // Summing up primitives generated for every chunk
	statistics.z += voxelCounter*36; // Summing up vertexes generated for every chunk
}
void GeometryManager::CreateUnindexedQuadGeometry(Chunk* chunk)
{
	voxelCounter = 0;
	for(int x = 0; x < Chunk::CHUNK_SIZE; x++)
	{
		for(int y = 0; y < Chunk::CHUNK_SIZE; y++)
		{
			for(int z = 0; z < Chunk::CHUNK_SIZE; z++)
			{
				if(chunk->IsVoxelActive(glm::vec3(x,y,z)) == GL_TRUE)
				{
					CreateQuadSmoothVoxel(chunk, glm::vec3(x,y,z));
					voxelCounter += 1;
				}	
			}
		}
	}
	statistics.x += voxelCounter; // Summing up voxels for every generated chunk
	statistics.y += voxelCounter*6; // Summing up primitives generated for every chunk
	statistics.z += voxelCounter*24; // Summing up vertexes generated for every chunk
}
void GeometryManager::CreateIndexedQuadGeometry(Chunk* chunk)
{
	voxelCounter = 0;
	for(int x = 0; x < Chunk::CHUNK_SIZE; x++)
	{
		for(int y = 0; y < Chunk::CHUNK_SIZE; y++)
		{
			for(int z = 0; z < Chunk::CHUNK_SIZE; z++)
			{
				if(chunk->IsVoxelActive(glm::vec3(x,y,z)) == GL_TRUE)
				{
					CreateQuadVoxel(chunk, glm::vec3(x,y,z));
					voxelCounter += 1;
				}	
			}
		}
	}
	statistics.x += voxelCounter; // Summing up voxels for every generated chunk
	statistics.y += voxelCounter*6; // Summing up primitives generated for every chunk
	statistics.z += voxelCounter*8; // Summing up vertexes generated for every chunk
	statistics.w += voxelCounter*24; // Summing up indices generated for every chunk
}
void GeometryManager::CreateOptimizedIndexedGeometry(Chunk* chunk, Heightfield heightfield)
{
	voxelCounter = 0;
	for(int x = 0; x < Chunk::CHUNK_SIZE; x++)
	{
		for(int y = 0; y < Chunk::CHUNK_SIZE; y++)
		{
			for(int z = 0; z < Chunk::CHUNK_SIZE; z++)
			{
				if(chunk->IsVoxelActive(glm::vec3(x,y,z)) == GL_TRUE)
				{
					CreateOptimizedQuadVoxel(chunk, glm::vec3(x,y,z));
					voxelCounter += 1;
				}	
			}
		}
	}
	statistics.x += voxelCounter; // Summing up voxels for every generated chunk
	statistics.z += voxelCounter*8; // Summing up vertexes generated for every chunk
}
void GeometryManager::CreateVoxel(Chunk* chunk, glm::vec3 relativePosition)
{
	int stride = 36*voxelCounter;
	int blockType = chunk->GetBlockType(relativePosition);

	glm::vec3 point1(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point2(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point3(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point4(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point5(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
	glm::vec3 point6(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point7(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point8(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);

	//Front
	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	//Back
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);
	// Right
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	//Left
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	//Top
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	//Bottom
	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	//Front
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,1.0f));
	//Back
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,0.0f,-1.0f));
	//Right
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0,0.0f,0.0f));
	//Left
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0,0.0f,0.0f));
	//Top
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,1.0f,0.0f));
	//Bottom
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0,-1.0f,0.0f));
	
	if(blockType == BLOCK_GRASS)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
	}
	else if(blockType == BLOCK_DIRT)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
	}
	else if(blockType == BLOCK_STONE)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
	}

	/*//Front
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(0+stride,1+stride,2+stride));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(3+stride,4+stride,5+stride));
	//Back
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(6+stride,7+stride,8+stride));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(9+stride,10+stride,11+stride));
	//Right
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(12+stride,13+stride,14+stride));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(15+stride,16+stride,17+stride));
	//Left
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(18+stride,19+stride,20+stride));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(21+stride,22+stride,23+stride));
	//Top
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(24+stride,25+stride,26+stride));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(27+stride,28+stride,29+stride));
	//Bottom
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(30+stride,31+stride,32+stride));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(33+stride,34+stride,35+stride));
	//Front
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(0+8*voxelCounter,1+8*voxelCounter,3+8*voxelCounter));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(1+8*voxelCounter,3+8*voxelCounter,2+8*voxelCounter));
	//Back
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(1+8*voxelCounter,5+8*voxelCounter,2+8*voxelCounter));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(5+8*voxelCounter,2+8*voxelCounter,6+8*voxelCounter));
	//Right
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(5+8*voxelCounter,4+8*voxelCounter,6+8*voxelCounter));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(4+8*voxelCounter,6+8*voxelCounter,7+8*voxelCounter));
	//Left
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(4+8*voxelCounter,0+8*voxelCounter,7+8*voxelCounter));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(0+8*voxelCounter,7+8*voxelCounter,3+8*voxelCounter));
	//Top
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(3+8*voxelCounter,2+8*voxelCounter,7+8*voxelCounter));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(2+8*voxelCounter,7+8*voxelCounter,6+8*voxelCounter));
	//Bottom
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(0+8*voxelCounter,1+8*voxelCounter,4+8*voxelCounter));
	meshArray->at(chunk->geometryID)->addTriangleToMesh(glm::vec3(1+8*voxelCounter,4+8*voxelCounter,5+8*voxelCounter));
	*/
}
void GeometryManager::CreateQuadVoxel(Chunk* chunk, glm::vec3 relativePosition)
{
	int stride = 8*voxelCounter;
	int blockType = chunk->GetBlockType(relativePosition);

	glm::vec3 point1(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point2(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point3(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point4(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point5(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
	glm::vec3 point6(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point7(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point8(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);

	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	
	if(blockType == BLOCK_GRASS)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
	}
	else if(blockType == BLOCK_DIRT)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
	}
	else if(blockType == BLOCK_STONE)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
	}
	//Front
	meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(0+stride,1+stride,2+stride,3+stride));
	//Back
	meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(5+stride,6+stride,7+stride,4+stride));
	//Right
	meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(1+stride,5+stride,6+stride,2+stride));
	//Left
	meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(3+stride,7+stride,4+stride,0+stride));
	//Top
	meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(2+stride,6+stride,7+stride,3+stride));
	//Bottom
	meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(4+stride,0+stride,1+stride,5+stride));
}
void GeometryManager::CreateQuadSmoothVoxel(Chunk* chunk, glm::vec3 relativePosition)
{
	int stride = 24*voxelCounter;
	int blockType = chunk->GetBlockType(relativePosition);

	glm::vec3 point1(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point2(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point3(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point4(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point5(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
	glm::vec3 point6(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point7(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point8(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);

	//Front
	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	//Back
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);
	//Right
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	//Left
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);
	//Top
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);
	//Bottom
	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);


	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,1.0f));


	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,-1.0f));

	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));

	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0f,0.0f,0.0f));

	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,1.0f,0.0f));

	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,-1.0f,0.0f));
	
	if(blockType == BLOCK_GRASS)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
	}
	else if(blockType == BLOCK_DIRT)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
	}
	else if(blockType == BLOCK_STONE)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
	}
}
void GeometryManager::CreateOptimizedQuadVoxel(Chunk* chunk, glm::vec3 relativePosition)
{
	int stride = 8*voxelCounter;
	int blockType = chunk->GetBlockType(relativePosition);
	std::vector<GLboolean> neighbours;
	/*glm::vec3 realPosition(chunk->chunkPosition->x*Chunk::CHUNK_SIZE+relativePosition.x,
							chunk->chunkPosition->y*Chunk::CHUNK_SIZE+relativePosition.y,
							chunk->chunkPosition->z*Chunk::CHUNK_SIZE+relativePosition.z);*/
	if(relativePosition.x == 0 || relativePosition.z == 0 || relativePosition.y == 0 || 
		relativePosition.x == Chunk::CHUNK_SIZE-1 ||
		relativePosition.z == Chunk::CHUNK_SIZE-1 ||
		relativePosition.y == Chunk::CHUNK_SIZE-1)
	{
		neighbours.push_back(GL_TRUE);
		neighbours.push_back(GL_TRUE);
		neighbours.push_back(GL_TRUE);
		neighbours.push_back(GL_TRUE);
		neighbours.push_back(GL_TRUE);
		neighbours.push_back(GL_TRUE);
	}
	else
	{
		//if(realPosition.y <= heightfield.GetHeightAtPoint(realPosition.x-1,realPosition.z))
		//	neighbours.push_back(0);
		//else
		//	neighbours.push_back(1);
		if(IsNeighbourActive(chunk,relativePosition,FRONT_SIDE))
			neighbours.push_back(GL_FALSE);
		else
			neighbours.push_back(GL_TRUE);

		if(IsNeighbourActive(chunk,relativePosition,BACK_SIDE))
			neighbours.push_back(GL_FALSE);	
		else
			neighbours.push_back(GL_TRUE);

		if(IsNeighbourActive(chunk,relativePosition,LEFT_SIDE))
			neighbours.push_back(GL_FALSE);
		else
			neighbours.push_back(GL_TRUE);

		if(IsNeighbourActive(chunk,relativePosition,RIGHT_SIDE))
			neighbours.push_back(GL_FALSE);
		else
			neighbours.push_back(GL_TRUE);

		if(IsNeighbourActive(chunk,relativePosition,TOP_SIDE))
			neighbours.push_back(GL_FALSE);
		else
			neighbours.push_back(GL_TRUE);

		if(IsNeighbourActive(chunk,relativePosition,BOTTOM_SIDE))
			neighbours.push_back(GL_FALSE);
		else
			neighbours.push_back(GL_TRUE);
	}

	

	glm::vec3 point1(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point2(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point3(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point4(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z+BLOCK_SIZE);
	glm::vec3 point5(relativePosition.x-BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
	glm::vec3 point6(relativePosition.x+BLOCK_SIZE, relativePosition.y-BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point7(relativePosition.x+BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);
    glm::vec3 point8(relativePosition.x-BLOCK_SIZE, relativePosition.y+BLOCK_SIZE, relativePosition.z-BLOCK_SIZE);

	meshArray->at(chunk->geometryID)->addVertexToMesh(point1);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point2);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point3);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point4);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point5);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point6);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point7);
	meshArray->at(chunk->geometryID)->addVertexToMesh(point8);

	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(-1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,-1.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(0.0f,0.0f,-1.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	meshArray->at(chunk->geometryID)->addNormalForTriangle(glm::vec3(1.0f,0.0f,0.0f));
	
	if(blockType == BLOCK_GRASS)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.0f,1.0f,0.0f,1.0f));
	}
	else if(blockType == BLOCK_DIRT)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.435f,0.305f,0.215f,1.0f));
	}
	else if(blockType == BLOCK_STONE)
	{
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
		meshArray->at(chunk->geometryID)->addColorForVert(glm::vec4(0.45f,0.45f,0.45f,1.0f));
	}
	
	//Front
	if(neighbours.at(FRONT_SIDE))
	{
		meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(0+stride,1+stride,2+stride,3+stride));
		statistics.y++;
		statistics.w+=4;
	}
	//Back
	if(neighbours.at(BACK_SIDE))
	{
		meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(5+stride,6+stride,7+stride,4+stride));
		statistics.y++;
		statistics.w+=4;
	}
	//Right
	if(neighbours.at(RIGHT_SIDE))
	{
		meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(1+stride,5+stride,6+stride,2+stride));
		statistics.y++;
		statistics.w+=4;
	}
	//Left
	if(neighbours.at(LEFT_SIDE))
	{
		meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(3+stride,7+stride,4+stride,0+stride));
		statistics.y++;
		statistics.w+=4;
	}
	//Top
	if(neighbours.at(TOP_SIDE))
	{
		meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(2+stride,6+stride,7+stride,3+stride));
		statistics.y++;
		statistics.w+=4;
	}
	//Bottom
	if(neighbours.at(BOTTOM_SIDE))
	{
		meshArray->at(chunk->geometryID)->addQuadToMesh(glm::vec4(4+stride,0+stride,1+stride,5+stride));
		statistics.y++;
		statistics.w+=4;
	}
}
GLboolean GeometryManager::IsNeighbourActive(Chunk* chunk, glm::vec3 voxelPosition,GLint type)
{
		if(type == FRONT_SIDE)
			return chunk->IsVoxelActive(glm::vec3(voxelPosition.x,voxelPosition.y,voxelPosition.z+1));
		
		if(type == BACK_SIDE)
			return chunk->IsVoxelActive(glm::vec3(voxelPosition.x,voxelPosition.y,voxelPosition.z-1)); 
	
		if(type == LEFT_SIDE)
			//if(realPosition.y <= heightfield.GetHeightAtPoint(realPosition.x-1,realPosition.z))
			//	return GL_TRUE;
			 return chunk->IsVoxelActive(glm::vec3(voxelPosition.x-1,voxelPosition.y,voxelPosition.z));

	
		if(type == RIGHT_SIDE)
			return chunk->IsVoxelActive(glm::vec3(voxelPosition.x+1,voxelPosition.y,voxelPosition.z));
		
		if(type == TOP_SIDE)
			return chunk->IsVoxelActive(glm::vec3(voxelPosition.x,voxelPosition.y+1,voxelPosition.z));
		
		if(type == BOTTOM_SIDE)
			return chunk->IsVoxelActive(glm::vec3(voxelPosition.x,voxelPosition.y-1,voxelPosition.z)); 

		//return GL_FALSE;
}
void GeometryManager::LoadIndexedGeometry(GLint geometryID)
{
	this->meshArray->at(geometryID)->generateVertexBufferData();
	this->meshArray->at(geometryID)->generateIndexBufferData();
	this->meshArray->at(geometryID)->generateNormalBufferData();
	this->meshArray->at(geometryID)->generateColorBufferData();
}
void GeometryManager::LoadUnindexedGeometry(GLint geometryID)
{
	this->meshArray->at(geometryID)->generateVertexBufferData();
	this->meshArray->at(geometryID)->generateNormalBufferData();
	this->meshArray->at(geometryID)->generateColorBufferData();
}
void GeometryManager::RenderTriangleArrays(glm::vec3 geometryPosition,GLint geometryID)
{
	glPushMatrix();

		pRenderer->TranslateWorldMatrix(geometryPosition*(float)Chunk::CHUNK_SIZE);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->vertexBuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,BUFFER_OFFSET(0));

		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->normalBuffer);
		glNormalPointer(GL_FLOAT,0,BUFFER_OFFSET(0));

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->colorBuffer);
		glColorPointer(4,GL_FLOAT,0,BUFFER_OFFSET(0));

		glDrawArrays(GL_TRIANGLES,0,meshArray->at(geometryID)->vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}
void GeometryManager::RenderQuadArrays(glm::vec3 geometryPosition,GLint geometryID)
{
	glPushMatrix();

		pRenderer->TranslateWorldMatrix(geometryPosition*(float)Chunk::CHUNK_SIZE);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->vertexBuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,BUFFER_OFFSET(0));

		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->normalBuffer);
		glNormalPointer(GL_FLOAT,0,BUFFER_OFFSET(0));

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->colorBuffer);
		glColorPointer(4,GL_FLOAT,0,BUFFER_OFFSET(0));

		glDrawArrays(GL_QUADS,0,meshArray->at(geometryID)->vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();
}
void GeometryManager::RenderQuadGeometry(glm::vec3 geometryPosition,GLint geometryID)
{
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	glPushMatrix();

		pRenderer->TranslateWorldMatrix(geometryPosition*(float)Chunk::CHUNK_SIZE);

		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->vertexBuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,0,BUFFER_OFFSET(0));
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,meshArray->at(geometryID)->indexBuffer);
	
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->normalBuffer);
		glNormalPointer(GL_FLOAT,0,BUFFER_OFFSET(0));

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,meshArray->at(geometryID)->colorBuffer);
		glColorPointer(4,GL_FLOAT,0,BUFFER_OFFSET(0));

		glDrawElements(GL_QUADS,meshArray->at(geometryID)->indexCount,GL_UNSIGNED_INT,BUFFER_OFFSET(0));
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	glPopMatrix();
}
