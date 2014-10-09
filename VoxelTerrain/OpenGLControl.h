#pragma once
#include "afxwin.h"

#include <glm.hpp>
#include <GL\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Player.h"
#include "GeometryManager.h"
#include "ChunkManager.h"
#include "Text.h"

#define R_QUAD 0
#define R_TRIANGLE 1
#define R_INDEXED 2
#define R_OPTIMIZED 4
#define R_CULLING 8
#define R_PROXIMITY 16
class COpenGLControl :
	public CWnd
{
public:
	UINT_PTR timer;
	bool isMaximized;
	bool mapLoaded, geometryLoaded;
	GLint renderingFlags;
private:
	CWnd *windowHandle;
	HDC handleDeviceContext;
	HGLRC hrc;
	int pixelFormat,viewportW,viewportH;
	CRect rect;
	CRect oldWindow;
	CRect originalRect;
	Player* player;
	ChunkManager* chunkManager;
	GeometryManager* geometryManager;
	Text text;
	unsigned int font;
public:
	COpenGLControl(void);
	virtual ~COpenGLControl(void);

	void oglCreate(CRect rect,CWnd *parentWindow);
	void LoadHeightmap(LPCTSTR);
	void LoadAllMap(bool complex);
	void findSurface();
	void LoadGeometry();
	void LoadBuffers();
	glm::vec4 GetStatistics();
	glm::vec2 GetSizes();
	glm::vec3 GetCameraPosition();
	void MovePlayer(int direction);
	void oglInitialize();
	void StartupCamera();

	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDraw(CDC *pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT type, int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
protected:
	void renderScene(GLint flags);
};

