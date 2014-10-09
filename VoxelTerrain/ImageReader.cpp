#include "stdafx.h"
#include "ImageReader.h"



ImageReader::ImageReader(void)
{
	this->heightmap = new CImage();
}


ImageReader::~ImageReader(void)
{
}
void ImageReader::ReadBitmap(LPCTSTR bmpFile)
{
	this->heightmap->Load(bmpFile);
	this->mapHeight = this->heightmap->GetHeight();
	this->mapWidth = this->heightmap->GetWidth();
}
void ImageReader::Cleanup()
{
	
}
int ImageReader::getValueAt(int x, int y)
{
	COLORREF color = this->heightmap->GetPixel(x,y);
	return (GetRValue(color) + GetGValue(color) + GetBValue(color))/3;
}