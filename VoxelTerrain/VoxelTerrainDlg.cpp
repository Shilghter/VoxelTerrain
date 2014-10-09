
// VoxelTerrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VoxelTerrain.h"
#include "VoxelTerrainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVoxelTerrainDlg dialog



CVoxelTerrainDlg::CVoxelTerrainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVoxelTerrainDlg::IDD, pParent)
	, filePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVoxelTerrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, filePath);
	DDX_Control(pDX, IDC_EDIT1, filePathControl);
	DDX_Control(pDX, IDC_SURFACE_TIME, surfaceLoadTime);
	DDX_Control(pDX, IDC_ALGORITHM, loadingAlgorithm);
	DDX_Control(pDX, IDC_SCANTIME, scanTime);
	DDX_Control(pDX, IDC_INDEXED, indexedCheck);
	DDX_Control(pDX, IDC_PRIMITIVE, renderingPrimitive);
	DDX_Control(pDX, IDC_CULLING, culling);
	DDX_Control(pDX, IDC_OPTIMIZED, optimizedCheck);
	DDX_Control(pDX, IDC_GEOMETRYTIME, geometryTime);
	DDX_Control(pDX, IDC_VBOTIME, vboTime);
	DDX_Control(pDX, IDC_VOXELCOUNT, voxelCount);
	DDX_Control(pDX, IDC_PRIMITIVECOUNT, primitiveCount);
	DDX_Control(pDX, IDC_VERTEXCOUNT, vertexCount);
	DDX_Control(pDX, IDC_SURFACESIZE, surfaceSize);
	DDX_Control(pDX, IDC_ACTIVECHUNKS, activeChunks);
	DDX_Control(pDX, IDC_INDICESCOUNT, indicesCount);
	DDX_Control(pDX, IDC_COORDX, coordX);
	DDX_Control(pDX, IDC_COORDY, coordY);
	DDX_Control(pDX, IDC_COORDZ, coordZ);
}

BEGIN_MESSAGE_MAP(CVoxelTerrainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BROWSE, &CVoxelTerrainDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_LOADMAP, &CVoxelTerrainDlg::OnBnClickedLoadmap)
	ON_BN_CLICKED(IDC_LOADGEOMETRY, &CVoxelTerrainDlg::OnBnClickedLoadgeometry)
END_MESSAGE_MAP()


// CVoxelTerrainDlg message handlers

BOOL CVoxelTerrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);
	ScreenToClient(rect);

	oglWindow.oglCreate(rect,this);
	oglWindow.timer = oglWindow.SetTimer(1,1,0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVoxelTerrainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVoxelTerrainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		UpdateData();
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVoxelTerrainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVoxelTerrainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
    
	switch (nType)
	{
		case SIZE_RESTORED:
		{
			if (oglWindow.isMaximized)
			{
				oglWindow.OnSize(nType, cx, cy);
				oglWindow.isMaximized = false;
			}

			break;
		}

		case SIZE_MAXIMIZED:
		{
			oglWindow.OnSize(nType, cx, cy);
			oglWindow.isMaximized = true;

			break;
		}
	}
}
BOOL CVoxelTerrainDlg::PreTranslateMessage(MSG* message)
{
	if(message->message == WM_KEYDOWN)
	{
		if(message->wParam == 'W')
		{
			oglWindow.MovePlayer(FORWARD);
			UpdateData();
			return true;
		}
		else if(message->wParam == 'S')
		{
			oglWindow.MovePlayer(BACKWARDS);
			UpdateData();
			return true;
		}
		else if(message->wParam == 'A')
		{
			oglWindow.MovePlayer(LEFT);
			UpdateData();
			return true;
		}
		else if(message->wParam == 'D')
		{
			oglWindow.MovePlayer(RIGHT);
			UpdateData();
			return true;
		}
		else if(message->wParam == VK_SPACE)
		{
			oglWindow.MovePlayer(UP);
			UpdateData();
			return true;
		}
		else if(message->wParam == VK_CONTROL)
		{
			oglWindow.MovePlayer(DOWN);
			UpdateData();
			return true;
		}
	}
	return CDialogEx::PreTranslateMessage(message);
}
CString CVoxelTerrainDlg::doubleToLPCTSTR(double value)
{
	CString string;
	string.Format(_T("%.3f"),value);
	return string;
}
CString CVoxelTerrainDlg::intToLPCTSTR(double value)
{
	CString string;
	string.Format(_T("%.1f"),value);
	return string;
}

void CVoxelTerrainDlg::OnBnClickedBrowse()
{
	CFileDialog browseFile(true,NULL,NULL,OFN_OVERWRITEPROMPT,_T("Bmp Files (*.bmp)|*.bmp|"));
	int err = browseFile.DoModal();
	filePath = browseFile.GetPathName();
	if(err == IDOK)
		filePathControl.SetWindowTextW(filePath);
}


void CVoxelTerrainDlg::OnBnClickedLoadmap()
{
	int selectedAlgorithm = loadingAlgorithm.GetCurSel();
	int size = filePath.GetLength();
	if(size == 0)
	{
		MessageBox(L"Choose map to load first!",L"Warning!",0);
	}
	else
	{
		double executionTime;
		clock_t startTime = clock();

		oglWindow.LoadHeightmap((LPCTSTR)filePath);
		if(selectedAlgorithm <= 0)
			oglWindow.LoadAllMap(true);
		else
			oglWindow.LoadAllMap(false);

		executionTime = double(clock()-startTime)/(double)CLOCKS_PER_SEC;
		surfaceLoadTime.SetWindowTextW(doubleToLPCTSTR(executionTime));

		startTime = clock();
		oglWindow.findSurface();
		executionTime = double(clock()-startTime)/(double)CLOCKS_PER_SEC;
		scanTime.SetWindowTextW(doubleToLPCTSTR(executionTime));
		oglWindow.StartupCamera();
		oglWindow.mapLoaded = true;
		UpdateData();
	}
}


void CVoxelTerrainDlg::OnBnClickedLoadgeometry()
{
	double executionTime;
	clock_t startTime = clock();
	int selectedPrimitive = renderingPrimitive.GetCurSel();
	int cullingType = culling.GetCurSel();
	if(selectedPrimitive == -1)
		selectedPrimitive = 0;
	if(oglWindow.mapLoaded)
	{
		if(cullingType == 1 || cullingType == -1)
			oglWindow.renderingFlags = selectedPrimitive;
		else if(cullingType == 0)
			oglWindow.renderingFlags = selectedPrimitive | R_CULLING;
		else if(cullingType == 2)
			oglWindow.renderingFlags = selectedPrimitive | R_CULLING | R_PROXIMITY;

		if(indexedCheck.GetCheck() == BST_CHECKED)
			oglWindow.renderingFlags = oglWindow.renderingFlags | R_INDEXED;
		if(optimizedCheck.GetCheck() == BST_CHECKED)
			oglWindow.renderingFlags = oglWindow.renderingFlags | R_OPTIMIZED;

		startTime = clock();
		oglWindow.LoadGeometry();
		executionTime = double(clock()-startTime)/(double)CLOCKS_PER_SEC;
		geometryTime.SetWindowTextW(doubleToLPCTSTR(executionTime));

		startTime = clock();
		oglWindow.LoadBuffers();
		executionTime = double(clock()-startTime)/(double)CLOCKS_PER_SEC;
		vboTime.SetWindowTextW(doubleToLPCTSTR(executionTime));

		oglWindow.geometryLoaded = true;
		glm::vec4 geometryStats = oglWindow.GetStatistics();
		glm::vec2 sizes = oglWindow.GetSizes();
		voxelCount.SetWindowTextW(intToLPCTSTR(geometryStats.x));
		primitiveCount.SetWindowTextW(intToLPCTSTR(geometryStats.y));
		vertexCount.SetWindowTextW(intToLPCTSTR(geometryStats.z));
		indicesCount.SetWindowTextW(intToLPCTSTR(geometryStats.w));
		surfaceSize.SetWindowTextW(intToLPCTSTR(sizes.x));
		if(oglWindow.renderingFlags & R_CULLING)
			activeChunks.SetWindowTextW(intToLPCTSTR(sizes.y));
	}
	else
	{
		MessageBox(L"There is no heightmap loaded! Please load any map first!",L"Map not loaded!",0);
	}
}
void CVoxelTerrainDlg::UpdateData()
{
	glm::vec3 position = oglWindow.GetCameraPosition();
	glm::vec2 sizes = oglWindow.GetSizes();

	coordX.SetWindowTextW(doubleToLPCTSTR(position.x));
	coordY.SetWindowTextW(doubleToLPCTSTR(position.y));
	coordZ.SetWindowTextW(doubleToLPCTSTR(position.z));

	if(oglWindow.renderingFlags & R_CULLING)
			activeChunks.SetWindowTextW(intToLPCTSTR(sizes.y));
}
