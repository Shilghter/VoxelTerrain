#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "Chunk.h"
#include "Heightfield.h"
#define FRONT_SIDE 0
#define BACK_SIDE 1
#define LEFT_SIDE 2
#define RIGHT_SIDE 3
#define TOP_SIDE 4
#define BOTTOM_SIDE 5
class GeometryManager
{
	public:
		Renderer* pRenderer;
		glm::vec4 statistics; // x - VoxelCount, y - Primitive Count, z - VertexCount, w - Indices count
	private:
		std::vector<CMesh*>* meshArray;
		int voxelCounter;
	public:
		GeometryManager(void);
		~GeometryManager(void);

		void AddMesh(GLuint chunkID, GLint* geometryID);
		void CreateUnindexedTriangleGeometry(Chunk* chunk);
		void CreateUnindexedQuadGeometry(Chunk* chunk);
		void CreateIndexedQuadGeometry(Chunk* chunk);
		void CreateOptimizedIndexedGeometry(Chunk* chunk, Heightfield heightfield);
		void LoadIndexedGeometry(GLint geometryID);
		void LoadUnindexedGeometry(GLint geometryID);
		void RenderTriangleArrays(glm::vec3 geometryPosition,GLint geometryID);
		void RenderQuadArrays(glm::vec3 geometryPosition,GLint geometryID);
		void RenderQuadGeometry(glm::vec3 geometryPosition,GLint geometryID);
	private:
		void CreateVoxel(Chunk* chunk, glm::vec3 relativePosition);
		void CreateQuadVoxel(Chunk* chunk, glm::vec3 relativePosition);
		void CreateQuadSmoothVoxel(Chunk* chunk, glm::vec3 relativePosition);
		void CreateOptimizedQuadVoxel(Chunk* chunk, glm::vec3 relativePosition);
		GLboolean IsNeighbourActive(Chunk* chunk, glm::vec3 voxelPosition,GLint type);
		void VertexAdder(Chunk* chunk,glm::vec3 voxelPosition, std::vector<GLboolean> positions, GLint* activeVerts);
};

