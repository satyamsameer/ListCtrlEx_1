
// AppListCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AppListCtrlDemo.h"
#include "AppListCtrlDemoDlg.h"
#include "afxdialogex.h"
#include <vector>

#include "MemExtDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Can be any valid control ID value
#define LIST_CONTROL_CUSTOM_ID 34354

//Number of columns in Custom List Control
#define LIST_CONTROL_COLUMN_COUNT 5

//Number of Rows in Custom List Control
#define LIST_CONTROL_ROWS_COUNT 15

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAppListCtrlDemoDlg dialog



CAppListCtrlDemoDlg::CAppListCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_APPLISTCTRLDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pCtrlCustomList = NULL;
	m_iDataCount = 0;

}

CAppListCtrlDemoDlg::~CAppListCtrlDemoDlg() {
	if (m_pCtrlCustomList != NULL) {
		delete m_pCtrlCustomList;
		m_pCtrlCustomList = NULL;
	}
}

void CAppListCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAppListCtrlDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAppListCtrlDemoDlg message handlers

BOOL CAppListCtrlDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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


	//Calculate the dimensions of control at runtime
	CRect rLocal;
	GetWindowRect(&rLocal);
	CRect rContrlTop;
	GetDlgItem(IDC_STATIC)->GetWindowRect(&rContrlTop);
	ScreenToClient(&rContrlTop);
	int offset = (rLocal.Height() - rContrlTop.top);
	offset += 10;

	CRect rCustomList;
	rCustomList.left = 1;
	rCustomList.top = 1;
	rCustomList.right = rCustomList.left + (rLocal.Width() - 20);
	rCustomList.bottom = (rContrlTop.top - 20);


	//Create control when dialog is fully made
	if (m_pCtrlCustomList == NULL) {
		m_pCtrlCustomList = new CExtListCtrl();
		m_pCtrlCustomList->Create(LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_ALIGNLEFT | WS_TABSTOP, rCustomList, this, LIST_CONTROL_CUSTOM_ID);
		m_pCtrlCustomList->ShowWindow(FALSE);

		//Set Custom List Control Properties

		//Basic UI Properties
		m_pCtrlCustomList->setControlBgColor(RGB(153, 217, 234));
		m_pCtrlCustomList->setAdjustColumns(TRUE);
		m_pCtrlCustomList->setRowAlternateColor(TRUE, RGB(153, 217, 234), RGB(95, 197, 222));
		m_pCtrlCustomList->setRowHightlight(TRUE, RGB(255, 127,39));
		m_pCtrlCustomList->setHeaderAttribute(RGB(38, 154, 183), RGB(255, 255, 255), TRUE, RGB(45, 45, 45));
		m_pCtrlCustomList->setItemsBorder(TRUE, RGB(45, 45, 45));
		m_pCtrlCustomList->allowScrollBars(FALSE);

		//Add Columns To Custom List Control
		CExtListCtrl::ColumnAttribute objLocal;
		CString sLocalHead = L"";
		for (int i = 0;i < LIST_CONTROL_COLUMN_COUNT;i++) {
			m_pCtrlCustomList->customAddColumn(i);
			m_pCtrlCustomList->getColumnAttribute(0, objLocal);
			sLocalHead.Format(L"Column %d",(i+1));
			objLocal.m_sColumnHeading = sLocalHead;
			objLocal.m_uiItemFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
			objLocal.m_iOriginalWidth = 130 ;
			objLocal.m_cBkColor = RGB(153, 217, 234);
			objLocal.m_cTextColor = RGB(0, 0, 0);
			objLocal.m_bAutoFitText = TRUE;
			m_pCtrlCustomList->setColumnAttribute(FALSE, i, objLocal);
		}

		m_pCtrlCustomList->bulkAddItems(LIST_CONTROL_ROWS_COUNT, TRUE);

		m_pCtrlCustomList->setItemsHeight(20, TRUE);

	}

	m_pCtrlCustomList->ShowWindow(TRUE);


	//Timer of 1000 milliseconds to append data in List
	SetTimer(1, 1000,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAppListCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAppListCtrlDemoDlg::OnPaint()
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
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAppListCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAppListCtrlDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent)
	{

	case 1: {
		
		m_iDataCount++;
		std::vector <CString> v_sData;
		CString sTemp;
		for (int iColumn = 0;iColumn<LIST_CONTROL_COLUMN_COUNT;iColumn++) {

			sTemp.Format(L"Data (%d,%d)", m_iDataCount, iColumn);
			v_sData.emplace_back(sTemp);

		}

		if (m_pCtrlCustomList !=NULL) {
			m_pCtrlCustomList->pushDataToBottom(v_sData);
		}

		break;
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
