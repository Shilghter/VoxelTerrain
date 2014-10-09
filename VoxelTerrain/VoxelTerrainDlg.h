
// VoxelTerrainDlg.h : header file
//

#pragma once
#include "OpenGLControl.h"
#include "afxwin.h"
#include <sstream>


// CVoxelTerrainDlg dialog
class CVoxelTerrainDlg : public CDialogEx
{
private:
	COpenGLControl oglWindow;
// Construction
public:
	CVoxelTerrainDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VOXELTERRAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* message);
	CString doubleToLPCTSTR(double value);
	CString intToLPCTSTR(double value);
	void UpdateData();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedLoadmap();
	afx_msg void OnBnClickedLoadgeometry();

	CEdit filePathControl;
	CString filePath;
	CEdit surfaceLoadTime;
	CComboBox loadingAlgorithm;
	CEdit scanTime;
	CButton indexedCheck;
	CComboBox renderingPrimitive;
	CComboBox culling;
	CButton optimizedCheck;
	CEdit geometryTime;
	CEdit vboTime;
	CEdit voxelCount;
	CEdit primitiveCount;
	CEdit vertexCount;
	CEdit surfaceSize;
	CEdit activeChunks;
	CEdit indicesCount;
	CEdit coordX;
	CEdit coordY;
	CEdit coordZ;
};
