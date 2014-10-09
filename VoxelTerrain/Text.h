#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <gl\GL.h>

class Text
{
private:
	FT_Library fontLib;
	FT_Face fontFace;
public:
	glm::vec3 textPosition;
	Text(void);
	~Text(void);
	
	int InnitializeText();
	unsigned int createBitmapFont(HDC hDC,char* fontName, int fontSize);
	void deleteBitmapFont(unsigned int font);
	void printString(unsigned int font,char* text);
	void UpdateTextPosition();
};

