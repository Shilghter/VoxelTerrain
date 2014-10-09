#include "stdafx.h"
#include "Heightfield.h"


Heightfield::Heightfield(void)
{
	this->imageReader = new ImageReader();
	this->chunk_check_mutex = new std::mutex();
	this->scale = 0.5f;
}


Heightfield::~Heightfield(void)
{
}
/* zmieniamy odczyt mapy y i z, z powodu niezgodnosci z ukladem oGL */
void Heightfield::LoadHeightfield(LPCTSTR bmpFile)
{
	this->imageReader->ReadBitmap(bmpFile);
	this->layersCount.x = this->imageReader->mapWidth/Chunk::CHUNK_SIZE;
	this->layersCount.y  = 128/Chunk::CHUNK_SIZE; // Wysokosc jest zawsze rozmiaru char
	this->layersCount.z  = this->imageReader->mapHeight/Chunk::CHUNK_SIZE;
}
Chunk Heightfield::PrepareChunk(glm::vec3 layer,int ID)
{
	Chunk* chunk = new Chunk(layer, ID);
	//Optymalizacja: je¿eli Chunk jest full, to nie sprawdzamy dalej tlyko od razu zwracamy
	chunk->full = isFull(layer);
	chunk->empty = isEmpty(layer);
	if(!chunk->full && !chunk->empty)
	{
		MakeSurface(chunk,layer);
		//MakeComplexSurface(chunk,layer);
	}
	return *chunk;
}
Chunk Heightfield::PrepareComplexChunk(glm::vec3 layer, int ID)
{
	Chunk* chunk = new Chunk(layer, ID);
	//Optymalizacja: je¿eli Chunk jest full, to nie sprawdzamy dalej tlyko od razu zwracamy
	chunk->full = isFull(layer);
	chunk->empty = isEmpty(layer);
	if(!chunk->full && !chunk->empty)
	{
		MakeComplexSurface(chunk,layer);
	}
	return *chunk;
}
GLboolean Heightfield::isEmpty(glm::vec3 layer)
{
	for(int x=layer.x*Chunk::CHUNK_SIZE; x < (layer.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); x++)
	{
		for(int z=layer.z*Chunk::CHUNK_SIZE; z < (layer.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); z++)
		{
			int surfaceHeight = this->imageReader->getValueAt(x,z)*scale;
			//Check if bottom of the chunk is higher than map surface at this x,y(x,z) coordinates
			if(surfaceHeight > layer.y*Chunk::CHUNK_SIZE)
				return GL_FALSE;
		}
	}
	return GL_TRUE;
}
GLboolean Heightfield::isFull(glm::vec3 layer)
{
	for(int x=layer.x*Chunk::CHUNK_SIZE; x < (layer.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); x++)
	{
		for(int z=layer.z*Chunk::CHUNK_SIZE; z < (layer.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); z++)
		{
			int surfaceHeight = this->imageReader->getValueAt(x,z)*scale;
			//Check if top of the chunk is lower than map surface at this x,y coordinates
			if(surfaceHeight < (layer.y*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE))
				return GL_FALSE;
			
		}
	}
	return GL_TRUE;
}
void Heightfield::MakeSurface(Chunk* chunk, glm::vec3 layer)
{
	for(int x=layer.x*Chunk::CHUNK_SIZE; x < (layer.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); x++)
	{
		for(int z=layer.z*Chunk::CHUNK_SIZE; z < (layer.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); z++)
		{
			int surfaceHeight = GetHeightAtPoint(x,z);
			for(int y=layer.y*Chunk::CHUNK_SIZE; y < surfaceHeight; y++)
			{
				
				if(y > layer.y*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE)
					break;
				//We are back into chunk coordinates so we have to divide by 16
				glm::vec3 relativePosition(x%Chunk::CHUNK_SIZE,y%Chunk::CHUNK_SIZE,z%Chunk::CHUNK_SIZE);
				if(y==surfaceHeight-1 && y < 100)
					chunk->SetVoxelToActive(relativePosition,BLOCK_GRASS);
				else if(y > 100)
					chunk->SetVoxelToActive(relativePosition,BLOCK_STONE);
				else
					chunk->SetVoxelToActive(relativePosition,BLOCK_DIRT);
			}
		}
	}
}
void Heightfield::MakeComplexSurface(Chunk* chunk,glm::vec3 layer)
{
	
	//std::vector<std::thread> loadingThreads;
	for(int y=layer.y*Chunk::CHUNK_SIZE; y < layer.y*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE; y++)
	{
		for(int x=layer.x*Chunk::CHUNK_SIZE; x < (layer.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); x++)
		{
			for(int z=layer.z*Chunk::CHUNK_SIZE; z < (layer.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); z++)
			{
				int surfaceHeight = GetHeightAtPoint(x,z);
				if( y == surfaceHeight)
				{
						glm::vec3 relativePosition(x%Chunk::CHUNK_SIZE,y%Chunk::CHUNK_SIZE,z%Chunk::CHUNK_SIZE);
						if(y==surfaceHeight && y < 100)
							chunk->SetVoxelToActive(relativePosition,BLOCK_GRASS);
						else if(y > 100)
							chunk->SetVoxelToActive(relativePosition,BLOCK_STONE);
						else
							chunk->SetVoxelToActive(relativePosition,BLOCK_DIRT);
				}
				else if( y < surfaceHeight)
				{
					if(checkNeighbourForWall(glm::vec2(x,z),y))
					{
						glm::vec3 relativePosition(x%Chunk::CHUNK_SIZE,y%Chunk::CHUNK_SIZE,z%Chunk::CHUNK_SIZE);
						if(y > 100)
							chunk->SetVoxelToActive(relativePosition,BLOCK_STONE);
						else
							chunk->SetVoxelToActive(relativePosition,BLOCK_DIRT);
					}
					/*if(x == layer.x*Chunk::CHUNK_SIZE || x == (layer.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE)-1 ||
						z == layer.z*Chunk::CHUNK_SIZE || z == (layer.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE)-1)
					{
						if(checkNeighbourForWall(glm::vec2(x,z),y))
						{
							glm::vec3 relativePosition(x%Chunk::CHUNK_SIZE,y%Chunk::CHUNK_SIZE,z%Chunk::CHUNK_SIZE);
							//if(y==surfaceHeight && y < 100)
							//	chunk->SetVoxelToActive(relativePosition,BLOCK_GRASS);
							if(y > 100)
								chunk->SetVoxelToActive(relativePosition,BLOCK_STONE);
							else
								chunk->SetVoxelToActive(relativePosition,BLOCK_DIRT);
						}
					}
					else
					{
						if(isAnyNeighbourLower(glm::vec2(x,z),y))
						{
							glm::vec3 relativePosition(x%Chunk::CHUNK_SIZE,y%Chunk::CHUNK_SIZE,z%Chunk::CHUNK_SIZE);
							//if(y==surfaceHeight && y < 100)
							//	chunk->SetVoxelToActive(relativePosition,BLOCK_GRASS);
							if(y > 100)
								chunk->SetVoxelToActive(relativePosition,BLOCK_STONE);
							else
								chunk->SetVoxelToActive(relativePosition,BLOCK_DIRT);
						}
					}*/
				}
			}
		}
	}
		//loadingThreads.push_back(std::thread(&Heightfield::ConstructLayer, this, chunk,glm::vec3(layer.x,y,layer.z)));
	//for(auto& th : loadingThreads)
	//	th.join();
}
void Heightfield::ConstructLayer(Chunk* chunk,glm::vec3 layerConstHeight)
{
	int workingHeight = layerConstHeight.y;
	for(int x=layerConstHeight.x*Chunk::CHUNK_SIZE; x < (layerConstHeight.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); x++)
	{
		for(int z=layerConstHeight.z*Chunk::CHUNK_SIZE; z < (layerConstHeight.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE); z++)
		{
			if(x == layerConstHeight.x*Chunk::CHUNK_SIZE || x == (layerConstHeight.x*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE)-1 ||
			   z == layerConstHeight.z*Chunk::CHUNK_SIZE || z == (layerConstHeight.z*Chunk::CHUNK_SIZE+Chunk::CHUNK_SIZE)-1 )
			{
				int surfaceHeight = GetHeightAtPoint(x,z);
				if(workingHeight < surfaceHeight)
				{
					//We are back into chunk coordinates so we have to divide by 16
					glm::vec3 relativePosition(x%Chunk::CHUNK_SIZE,workingHeight%Chunk::CHUNK_SIZE,z%Chunk::CHUNK_SIZE);
					if(workingHeight==surfaceHeight-1 && workingHeight < 100)
						chunk->SetVoxelToActive(relativePosition,BLOCK_GRASS);
					else if(workingHeight > 100)
						chunk->SetVoxelToActive(relativePosition,BLOCK_STONE);
					else
						chunk->SetVoxelToActive(relativePosition,BLOCK_DIRT);
				}
			}
		}
	}
}
int Heightfield::GetHeightAtPoint(int x, int z)
{
	return imageReader->getValueAt(x,z)*scale;
}
GLboolean Heightfield::isAnyNeighbourLower(glm::vec2 pos, int pointHeight)
{
	//This is only 2D vector so it uses (x,y) notation, but is supplied the actual (x,z) world coordinates
	int test;
	test = GetHeightAtPoint(pos.x-1,pos.y);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x-1,pos.y+1);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x,pos.y+1);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x+1,pos.y+1);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x+1,pos.y);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x+1,pos.y-1);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x,pos.y-1);
	if(test <= pointHeight)
		return GL_TRUE;
	test = GetHeightAtPoint(pos.x-1,pos.y-1);
	if(test <= pointHeight)
		return GL_TRUE;

	return GL_FALSE;
}
GLboolean Heightfield::checkNeighbourForWall(glm::vec2 pos, int pointHeight)
{
	if(pos.x == 0 || pos.y == 0 || pos.x == (layersCount.x*Chunk::CHUNK_SIZE)-1 || pos.y == (layersCount.z*Chunk::CHUNK_SIZE)-1)
	{
		int test;
		if(pos.x == 0 && pos.y == 0)
		{
			test = GetHeightAtPoint(pos.x,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.x == 0 && pos.y == (layersCount.z*Chunk::CHUNK_SIZE)-1)
		{
			test = GetHeightAtPoint(pos.x+1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.x == (layersCount.x*Chunk::CHUNK_SIZE)-1 && pos.y == 0)
		{
			test = GetHeightAtPoint(pos.x-1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x-1,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.x == (layersCount.x*Chunk::CHUNK_SIZE)-1 && pos.y == (layersCount.z*Chunk::CHUNK_SIZE)-1)
		{
			test = GetHeightAtPoint(pos.x-1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x-1,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.x == 0)
		{
			test = GetHeightAtPoint(pos.x,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.x == (layersCount.x*Chunk::CHUNK_SIZE)-1)
		{
			test = GetHeightAtPoint(pos.x-1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x-1,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x-1,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.y == 0)
		{
			test = GetHeightAtPoint(pos.x-1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x-1,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y+1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
		else if(pos.y == (layersCount.z*Chunk::CHUNK_SIZE)-1)
		{
			test = GetHeightAtPoint(pos.x-1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x+1,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;
			test = GetHeightAtPoint(pos.x-1,pos.y-1);
			if(test <= pointHeight)
				return GL_TRUE;

			return GL_FALSE;
		}
	}
	else
	{
		return isAnyNeighbourLower(pos,pointHeight);
	}
}