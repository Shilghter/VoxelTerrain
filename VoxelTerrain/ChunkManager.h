#pragma once
#include "Chunk.h"
#include "GeometryManager.h"
#include "Heightfield.h"
#include "Frustum.h"
#include <vector>

#define TRIS 0
#define QUADS 1

class ChunkManager
{
public:
	Heightfield* heightfield;
private:
	std::vector<Chunk*>* chunkLoadList; 
	std::vector<GLint>* chunkSurfaceList; 
	std::vector<GLint>* chunkVisibleList; 
	std::vector<GLint>* chunkUnloadList;
	GeometryManager* pGeometryManager;
	Frustum frustum;
	std::mutex* mutex;
public:
	ChunkManager(GeometryManager* pManager);
	~ChunkManager(void);
	void SetupLights();
	void LoadAllMap(bool complex);
	void LoadAllMapThreaded();
	void LoadIndexedSurface();
	void LoadUnindexedSurface();
	void ScanForSurface();
	void CreateUnindexedSurfaceMesh(GLint);
	void CreateIndexedSurfaceMesh(GLboolean);
	void RenderUnindexedSurface(GLint);
	void RenderIndexedQuadSurface();
	void RenderVisibleSurface();
	void RenderVisibleQuadSurface();
	void ObtainFrustum();
	void UpdateVisibility(GLint flags, glm::vec3 playerPosition);
	void UpdateProximityVisiblity(glm::vec3 playerPosition);
	void UpdateLights();
	void GetActiveChunksCount(GLint* result);
	void GetVisibleChunksCount(GLint* result);
private:
	void AddLayerToLoadList(int layerZ);
};

