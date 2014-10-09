#include "stdafx.h"
#include "ChunkManager.h"


ChunkManager::ChunkManager(GeometryManager* pManager)
{
	this->pGeometryManager = pManager;
	this->heightfield = new Heightfield();
	this->chunkLoadList = new std::vector<Chunk*>();
	this->chunkSurfaceList = new std::vector<GLint>();
	this->chunkVisibleList = new std::vector<GLint>();
	this->chunkUnloadList = new std::vector<GLint>();
	this->mutex = new std::mutex();
}
ChunkManager::~ChunkManager(void)
{
}
void ChunkManager::SetupLights()
{
	this->pGeometryManager->pRenderer->SetupLights(heightfield->layersCount,Chunk::CHUNK_SIZE);
}
void ChunkManager::LoadAllMap(bool complex)
{
	if(complex)
	{
		for(int x=0; x <this->heightfield->layersCount.x; x++)
		{
			for(int z=0; z < this->heightfield->layersCount.z; z++)
			{
				for(int y=0; y < this->heightfield->layersCount.y; y++)
				{
					glm::vec3 currentLayer(x,y,z);
					chunkLoadList->push_back(new Chunk(this->heightfield->PrepareComplexChunk(currentLayer,x+y+z)));
				}
			}
		}
	}
	else
	{
		for(int x=0; x <this->heightfield->layersCount.x; x++)
		{
			for(int z=0; z < this->heightfield->layersCount.z; z++)
			{
				for(int y=0; y < this->heightfield->layersCount.y; y++)
				{
					glm::vec3 currentLayer(x,y,z);
					chunkLoadList->push_back(new Chunk(this->heightfield->PrepareChunk(currentLayer,x+y+z)));
				}
			}
		}
	}
}
void ChunkManager::LoadAllMapThreaded()
{
	std::vector<std::thread> loadingThreads;
	for(int y=0; y < this->heightfield->layersCount.y; y++)
	{
		loadingThreads.push_back(std::thread(&ChunkManager::AddLayerToLoadList, this, y));
	}
	for(auto& th : loadingThreads)
		th.join();
}
void ChunkManager::AddLayerToLoadList(int layerY)
{
	
	for(int x=0; x <this->heightfield->layersCount.x; x++)
	{
		for(int z=0; z < this->heightfield->layersCount.z; z++)
		{
			glm::vec3 currentLayer(x,layerY,z);
			//Mutex zadzia³a³ :)
			this->mutex->lock();
			chunkLoadList->push_back(new Chunk(this->heightfield->PrepareChunk(currentLayer,(x+z+layerY))));
			this->mutex->unlock();
		}
	}
}
void ChunkManager::ScanForSurface()
{
	for(auto i = 0; i < this->chunkLoadList->size(); i++)
	{
		if(!chunkLoadList->at(i)->empty && !chunkLoadList->at(i)->full)
		{
			this->chunkSurfaceList->push_back(i);
		}
	}
}
void ChunkManager::ObtainFrustum()
{
	frustum.ObtainFrustum();
}
void ChunkManager::UpdateVisibility(GLint flags, glm::vec3 playerPosition)
{
	if(flags & 8 && !(flags & 16))
	{
		frustum.ObtainFrustum();
		glm::vec3 chunkCenter;
		chunkVisibleList = new std::vector<GLint>();
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{
			int workingChunk = chunkSurfaceList->at(i);
			GLboolean visible = frustum.IsChunkInFrustum(chunkLoadList->at(workingChunk)->chunkCenter,(Chunk::CHUNK_SIZE/2.0f));
			if(visible)
				chunkVisibleList->push_back(workingChunk);
		}
	}
	else if(flags & 16)
	{
		UpdateProximityVisiblity(playerPosition);
	}
}
void ChunkManager::UpdateProximityVisiblity(glm::vec3 playerPosition)
{
	glm::vec3 chunkCenter;
	chunkVisibleList = new std::vector<GLint>();
	for(int i=0; i < this->chunkSurfaceList->size(); i++)
	{
		int workingChunk = chunkSurfaceList->at(i);
		GLboolean visible = frustum.isChunkInProximityFrustum(chunkLoadList->at(workingChunk)->chunkCenter,playerPosition,Chunk::CHUNK_SIZE*2);
		if(visible)
			chunkVisibleList->push_back(workingChunk);
	}
}
void ChunkManager::CreateUnindexedSurfaceMesh(GLint primitive)
{
	if(primitive)
	{
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{ 
			int workingChunk = chunkSurfaceList->at(i);
			pGeometryManager->AddMesh(chunkLoadList->at(workingChunk)->ChunkID,&chunkLoadList->at(workingChunk)->geometryID);
			pGeometryManager->CreateUnindexedQuadGeometry(chunkLoadList->at(workingChunk));
		}
	}
	else
	{
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{ 
			int workingChunk = chunkSurfaceList->at(i);
			pGeometryManager->AddMesh(chunkLoadList->at(workingChunk)->ChunkID,&chunkLoadList->at(workingChunk)->geometryID);
			pGeometryManager->CreateUnindexedTriangleGeometry(chunkLoadList->at(workingChunk));
		}
	}
}
void ChunkManager::CreateIndexedSurfaceMesh(GLboolean optimized)
{
	if(optimized)
	{
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{ 
			int workingChunk = chunkSurfaceList->at(i);
			pGeometryManager->AddMesh(chunkLoadList->at(workingChunk)->ChunkID,&chunkLoadList->at(workingChunk)->geometryID);
			pGeometryManager->CreateOptimizedIndexedGeometry(chunkLoadList->at(workingChunk), *heightfield);
		}
	}
	else
	{
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{ 
			int workingChunk = chunkSurfaceList->at(i);
			pGeometryManager->AddMesh(chunkLoadList->at(workingChunk)->ChunkID,&chunkLoadList->at(workingChunk)->geometryID);
			pGeometryManager->CreateIndexedQuadGeometry(chunkLoadList->at(workingChunk));
		}
	}
}
void ChunkManager::LoadIndexedSurface()
{
	for(int i=0; i < this->chunkSurfaceList->size(); i++)
	{
		int workingChunk = chunkSurfaceList->at(i);
		pGeometryManager->LoadIndexedGeometry(chunkLoadList->at(workingChunk)->geometryID);
	}
}
void ChunkManager::LoadUnindexedSurface()
{
	for(int i=0; i < this->chunkSurfaceList->size(); i++)
	{
		int workingChunk = chunkSurfaceList->at(i);
		pGeometryManager->LoadUnindexedGeometry(chunkLoadList->at(workingChunk)->geometryID);
	}
}
void ChunkManager::RenderUnindexedSurface(GLint primitive)
{
	
	if(primitive)
	{
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{
			int workingChunk = chunkSurfaceList->at(i);
			glm::vec3 position = *chunkLoadList->at(workingChunk)->chunkPosition;
			pGeometryManager->RenderQuadArrays(position,chunkLoadList->at(workingChunk)->geometryID);
		}
	}
	else
	{
		for(int i=0; i < this->chunkSurfaceList->size(); i++)
		{
			int workingChunk = chunkSurfaceList->at(i);
			glm::vec3 position = *chunkLoadList->at(workingChunk)->chunkPosition;
			pGeometryManager->RenderTriangleArrays(position,chunkLoadList->at(workingChunk)->geometryID);
		}
	}
}
void ChunkManager::RenderIndexedQuadSurface()
{

	for(int i=0; i < this->chunkSurfaceList->size(); i++)
	{
		int workingChunk = chunkSurfaceList->at(i);
		glm::vec3 position = *chunkLoadList->at(workingChunk)->chunkPosition;
		pGeometryManager->RenderQuadGeometry(position,chunkLoadList->at(workingChunk)->geometryID);
	}
}
void ChunkManager::RenderVisibleSurface()
{
	pGeometryManager->pRenderer->SetRenderMode(GL_FILL);
	for(int i=0; i < this->chunkVisibleList->size(); i++)
	{
		int workingChunk = chunkVisibleList->at(i);
		glm::vec3 position = *chunkLoadList->at(workingChunk)->chunkPosition;
		pGeometryManager->RenderQuadArrays(position,chunkLoadList->at(workingChunk)->geometryID);
	}
}
void ChunkManager::RenderVisibleQuadSurface()
{
	pGeometryManager->pRenderer->SetRenderMode(GL_FILL);
	for(int i=0; i < this->chunkVisibleList->size(); i++)
	{
		int workingChunk = chunkVisibleList->at(i);
		glm::vec3 position = *chunkLoadList->at(workingChunk)->chunkPosition;
		pGeometryManager->RenderQuadGeometry(position,chunkLoadList->at(workingChunk)->geometryID);
	}
}
void ChunkManager::UpdateLights()
{
	glm::vec3 layers = this->heightfield->layersCount;
	layers.x = (layers.x*Chunk::CHUNK_SIZE)/2.0f;
	layers.y = 70.0f;
	layers.z = (layers.z*Chunk::CHUNK_SIZE)/2.0f;
	this->pGeometryManager->pRenderer->UpdateLights(layers);
}
void ChunkManager::GetActiveChunksCount(GLint* result)
{
	*result = this->chunkSurfaceList->size();
}
void ChunkManager::GetVisibleChunksCount(GLint* result)
{
	*result = this->chunkVisibleList->size();
}