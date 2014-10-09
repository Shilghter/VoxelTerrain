#include "stdafx.h"
#include "OpenGLControl.h"


COpenGLControl::COpenGLControl(void)
{
	player = new Player();
	isMaximized = false;
	this->geometryManager = new GeometryManager();
	this->chunkManager = new ChunkManager(geometryManager);
	this->mapLoaded = false;
	this->geometryLoaded = false;
	this->renderingFlags = 0;
	//this->chunkManager->heightfield->LoadHeightfield(L"G:\\Dropbox\\Dropbox\\Seminarium\\Heightmaps\\heightmap64.bmp");
	//StartupCamera();
}
COpenGLControl::~COpenGLControl(void)
{
	this->text.deleteBitmapFont(this->font);
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::oglCreate(CRect rect,CWnd *parentWindow)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL,(HBRUSH)GetStockObject(BLACK_BRUSH),NULL);
	CreateEx(0,className,L"OpenGL",WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rect,parentWindow,0,(LPVOID) 0);
	this->oldWindow = rect;
	this->originalRect = rect;
	this->windowHandle = parentWindow;
}
void COpenGLControl::oglInitialize()
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // 32 bity
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		16, //z-buffer depth
		0,0,0,0,0,0,0,
	};

	handleDeviceContext = GetDC()->m_hDC;

	pixelFormat = ChoosePixelFormat(handleDeviceContext,&pfd);
	SetPixelFormat(handleDeviceContext,pixelFormat,&pfd);

	//Rendering context
	hrc = wglCreateContext(handleDeviceContext);
	wglMakeCurrent(handleDeviceContext,hrc);

	/* ******** OpenGL Setup ****** */
	glClearColor(0.529f,0.807f,0.921f,1.0f);
	glClearDepth(1.0f);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	this->chunkManager->SetupLights();

	glDepthFunc(GL_LEQUAL);

	glewExperimental = GL_TRUE;
	GLenum test = glewInit();

	/*if(int z = text.InnitializeText())
	{
		wchar_t error[256];
		wsprintfW(error , L"%d", z);
		MessageBoxEx(0,error,L"Error",0,0);
	}*/
//	font = text.createBitmapFont(handleDeviceContext,"Times New Roman",48);

	//double executionTime;
	//GLint surfaceSize;
	//glm::vec4 stats;
	//clock_t startTime = clock();
	//this->chunkManager->LoadAllMap();
	//executionTime = double(clock()-startTime)/(double)CLOCKS_PER_SEC;

	//startTime = clock();
	//chunkManager->ScanForSurface();
	//executionTime += double(clock()-startTime)/(double)CLOCKS_PER_SEC;

	//startTime = clock();
	////chunkManager->CreateUnindexedSurfaceMesh(QUADS);
	//chunkManager->CreateIndexedSurfaceMesh(GL_FALSE);
	//executionTime += double(clock()-startTime)/(double)CLOCKS_PER_SEC;

	//startTime = clock();
	////chunkManager->LoadUnindexedSurface();
	//chunkManager->LoadIndexedSurface();
	//executionTime += double(clock()-startTime)/(double)CLOCKS_PER_SEC;

	//stats = geometryManager->statistics;
	//chunkManager->GetActiveChunksCount(&surfaceSize);
	OnDraw(NULL);
}
void COpenGLControl::OnPaint()
{
	ValidateRect(NULL);
}
int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	oglInitialize();
	return 0;
}
void COpenGLControl::OnDraw(CDC *pDC)
{
	glLoadIdentity();
	glm::vec3 position = player->GetCamera(CAMERA_POSITION);
	//position.y = chunkManager->heightfield->GetHeightAtPoint(position.x,position.z);
	this->geometryManager->pRenderer->RotateWorldMatrix(player->GetCamera(CAMERA_ROTATION));
	gluLookAt(position.x,position.y,position.z,position.x,position.y,position.z-1.0f,0.0f,1.0f,0.0f);
	chunkManager->UpdateLights();
	chunkManager->UpdateVisibility(renderingFlags,player->GetCamera(CAMERA_POSITION));
	//chunkManager->UpdateProximityVisiblity(player->GetCamera(CAMERA_POSITION));
	//chunkManager->UpdateVisibility();
}
void COpenGLControl::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderScene(renderingFlags);
			//chunkManager->UpdateVisibility();
			//chunkManager->RenderVisibleQuadSurface();
			//chunkManager->RenderVisibleSurface();
			//chunkManager->RenderUnindexedSurface(QUADS);
			//chunkManager->RenderIndexedQuadSurface();
			SwapBuffers(handleDeviceContext);
			break;
		}
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
void COpenGLControl::OnSize(UINT type, int x, int y)
{
	CWnd::OnSize(type,x,y);
	viewportW = x;
	viewportH = y;
	if( 0 >= x || 0 >= y || type == SIZE_MINIMIZED)
		return;

	glViewport(0,0,x,y);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60.0f,(float)x/(float)y,1.0f,400.0f);

	glMatrixMode(GL_MODELVIEW);
	switch (type)
	{
		// If window resize token is "maximize"
		case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(rect);

			// Move the window accordingly
			MoveWindow(6, 6, x - 14, y - 14);

			// Get the new window rect
			GetWindowRect(rect);

			// Store our old window as the new rect
			oldWindow = rect;

			break;
		}

		// If window resize token is "restore"
		case SIZE_RESTORED:
		{
			// If the window is currently maximized
			if (isMaximized)
			{
				// Get the current window rect
				GetWindowRect(rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(oldWindow.left, oldWindow.top - 18, originalRect.Width() - 4, originalRect.Height() - 4);

				// Get the new window rect
				GetWindowRect(rect);

				// Store our old window as the new rect
				oldWindow = rect;
			}
		
			break;
		}
	}
}
void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	player->UpdateLook(point);   
	OnDraw(NULL);
	CWnd::OnMouseMove(nFlags,point);
}
void COpenGLControl::StartupCamera()
{
	SetCursorPos(960,540);
	glLoadIdentity();
	glm::vec3 layers = chunkManager->heightfield->layersCount;
	int width = layers.x*Chunk::CHUNK_SIZE;
	int height = layers.z*Chunk::CHUNK_SIZE;
	int startHeight = chunkManager->heightfield->GetHeightAtPoint(width/2.0f,height/2.0f);
	//player->SetCamera(CAMERA_POSITION, glm::vec3(width/2.0f,startHeight+1.0f,height/2.0f));
	player->SetCamera(CAMERA_POSITION, glm::vec3(0.0f,startHeight+1.0f,0.0f));
}
void COpenGLControl::MovePlayer(int direction)
{
	player->Move(direction);
	OnDraw(NULL);
}
void COpenGLControl::LoadHeightmap(LPCTSTR mapName)
{
	this->chunkManager->heightfield->LoadHeightfield(mapName);
}
void COpenGLControl::LoadAllMap(bool complex)
{
	this->chunkManager->LoadAllMap(complex);
}
void COpenGLControl::findSurface()
{
	this->chunkManager->ScanForSurface();
}
void COpenGLControl::renderScene(GLint flags)
{
	if(flags & R_CULLING)
	{
		if(flags & R_INDEXED)
		{
			chunkManager->RenderVisibleQuadSurface();
		}
		else
		{
			chunkManager->RenderVisibleSurface();
		}
	}
	else
	{
		if(geometryLoaded && flags == R_QUAD)
		{
			chunkManager->RenderUnindexedSurface(QUADS);
		}
		else if(flags == R_TRIANGLE)
		{
			chunkManager->RenderUnindexedSurface(TRIS);
		}
		else if(flags & R_INDEXED)
		{
			chunkManager->RenderIndexedQuadSurface();
		}
	}
	
}
void COpenGLControl::LoadGeometry()
{
	if(!renderingFlags)
	{
		chunkManager->CreateUnindexedSurfaceMesh(QUADS);
	}
	else if(renderingFlags == R_TRIANGLE)
	{
		chunkManager->CreateUnindexedSurfaceMesh(TRIS);
	}
	else if(renderingFlags == R_QUAD | R_INDEXED)
	{
		chunkManager->CreateIndexedSurfaceMesh(GL_FALSE);
	}
	else if(renderingFlags == R_QUAD | R_INDEXED | R_OPTIMIZED)
	{
		chunkManager->CreateIndexedSurfaceMesh(GL_TRUE);
	}
}
void COpenGLControl::LoadBuffers()
{
	if(renderingFlags & 2)
		chunkManager->LoadIndexedSurface();
	else
		chunkManager->LoadUnindexedSurface();
}
glm::vec4 COpenGLControl::GetStatistics()
{
	return geometryManager->statistics;
}
glm::vec2 COpenGLControl::GetSizes()
{
	glm::vec2 stats;
	GLint activeChunks, visibleChunks;
	chunkManager->GetActiveChunksCount(&activeChunks);
	chunkManager->GetVisibleChunksCount(&visibleChunks);
	stats.x = activeChunks;
	stats.y = visibleChunks;
	return stats;
}
glm::vec3 COpenGLControl::GetCameraPosition()
{
	return player->GetCamera(CAMERA_POSITION);
}