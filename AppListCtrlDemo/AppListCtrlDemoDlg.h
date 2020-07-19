
// AppListCtrlDemoDlg.h : header file
//

#pragma once
#include "ExtListCtrl.h"

// CAppListCtrlDemoDlg dialog
class CAppListCtrlDemoDlg : public CDialogEx
{
// Construction
public:
	CAppListCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CAppListCtrlDemoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLISTCTRLDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//Dynamically create list control pointer
	CExtListCtrl *m_pCtrlCustomList;

	//Data Row Counter
	int m_iDataCount;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
