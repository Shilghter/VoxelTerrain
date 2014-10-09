#pragma once
#include "ImageReader.h"
#include "Chunk.h"
#include <mutex>
#include <thread>
#include <future>

class Heightfield
{
public:
	glm::vec3 layersCount;
	float scale;
private:
	ImageReader* imageReader;
	std::mutex* chunk_check_mutex;
public:
	Heightfield(void);
	~Heightfield(void);
	void LoadHeightfield(LPCTSTR bmpFile);
	Chunk PrepareChunk(glm::vec3 layer, int ID);
	Chunk PrepareComplexChunk(glm::vec3 layer, int ID);
	int GetHeightAtPoint(int x, int z);
private:
	GLboolean isEmpty(glm::vec3 layer);
	GLboolean isFull(glm::vec3 layer);
	void MakeSurface(Chunk* ,glm::vec3 layer);
	void MakeComplexSurface(Chunk*,glm::vec3 layer);
	void ConstructLayer(Chunk*,glm::vec3 layerConstHeight);
	GLboolean isAnyNeighbourLower(glm::vec2 currentPosition, int pointHeight);
	GLboolean checkNeighbourForWall(glm::vec2 currentPosition, int pointHeight);
};

