#pragma once
#include "stdafx.h"
#include <vector>
class ImageReader
{
public:
	int mapWidth;
	int mapHeight;
private:
	CImage* heightmap;
public:
	ImageReader(void);
	~ImageReader(void);
	void ReadBitmap(LPCTSTR bmpFile);
	int getValueAt(int x, int y);
	void Cleanup();
};

