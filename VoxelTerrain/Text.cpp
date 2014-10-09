#include "stdafx.h"
#include "Text.h"


Text::Text(void)
{
	textPosition = glm::vec3(0.0f,0.0f,0.0f);
}


Text::~Text(void)
{
}

int Text::InnitializeText()
{
	if(FT_Init_FreeType(&this->fontLib))
		return 1; //Cannot load library

	if(FT_New_Face(this->fontLib,"G:\\Dropbox\\Dropbox\\Seminarium\\sourcetest.ttf",0,&this->fontFace))
		return 2; //Cannot load Font Face

	if(FT_Set_Char_Size(this->fontFace,0,16*64,300,300))
		return 3;
	return 0;
}
unsigned int Text::createBitmapFont(HDC hDC,char* fontName, int fontSize)
{
	unsigned int font;
	HFONT fontHandle;
	LOGFONT logFont;

	logFont.lfHeight = fontSize;
	logFont.lfEscapement = 0;
	logFont.lfWeight = FW_BOLD;
	logFont.lfItalic = FALSE;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfCharSet = DEFAULT_CHARSET;
	_tcscpy_s(logFont.lfFaceName,_countof(_T("Times New Roman")),_T("Times New Roman"));

	fontHandle = CreateFontIndirect(&logFont);

	font = glGenLists(256);
	SelectObject(hDC,fontHandle);
	wglUseFontBitmaps(hDC,0,256,font);

	return font;
}
void Text::deleteBitmapFont(unsigned int font)
{
	glDeleteLists(font, 256);
}
void Text::printString(unsigned int font,char* text)
{
	glListBase(font);
	glCallLists(strlen(text),GL_UNSIGNED_BYTE,text);
}
void Text::UpdateTextPosition()
{
	textPosition = glm::vec3(0.0f,0.0f,0.0f);
}