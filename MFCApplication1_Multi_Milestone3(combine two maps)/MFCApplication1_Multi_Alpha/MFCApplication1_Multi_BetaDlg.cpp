
// MFCApplication1_Multi_AlphaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCApplication1_Multi_Beta.h"
#include "MFCApplication1_Multi_BetaDlg.h"
#include "afxdialogex.h"
#include "windows.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CWinThread* pThread[THREADNUM];
LPTHREADDATA pThreadData = new THREADDATA[THREADNUM];
Rect *Roi_thread = new Rect[THREADNUM];

//LPTHREADDATA pThreadData[THREADNUM];

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


// CMFCApplication1_Multi_AlphaDlg dialog



CMFCApplication1_Multi_AlphaDlg::CMFCApplication1_Multi_AlphaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1_Multi_AlphaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1_Multi_AlphaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1_Multi_AlphaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_MESSAGE(WM_PULSE, &CMFCApplication1_Multi_AlphaDlg::OnPulse)
	ON_MESSAGE(WM_SHOW_UPDATA, &CMFCApplication1_Multi_AlphaDlg::OnShowUpdata)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton7)
	ON_MESSAGE(WM_STOPREC, &CMFCApplication1_Multi_AlphaDlg::OnStoprec)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCApplication1_Multi_AlphaDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CMFCApplication1_Multi_AlphaDlg message handlers

BOOL CMFCApplication1_Multi_AlphaDlg::OnInitDialog()
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
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(false);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1_Multi_AlphaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1_Multi_AlphaDlg::OnPaint()
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
HCURSOR CMFCApplication1_Multi_AlphaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1_Multi_AlphaDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//MessageBox(_T("Timber"));
	if (signal == (THREADNUM))
	{
		signal = 0;
		//MessageBox(_T("timber enter"));
		SendMessage(WM_SHOW_UPDATA, NULL, NULL);

	}
	//
	CDialogEx::OnTimer(nIDEvent);
}


afx_msg LRESULT CMFCApplication1_Multi_AlphaDlg::OnPulse(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(_T("loop"));
	signal++;
	return 0;
}


afx_msg LRESULT CMFCApplication1_Multi_AlphaDlg::OnShowUpdata(WPARAM wParam, LPARAM lParam)
{
	Mat result_CT,result_temp;
	globalmap.m_dst.convertTo(result_CT, CV_8U);
	globalmap.m_dst = 0;
	//MessageBox(_T("display"));
	if (rotation == BST_CHECKED)
	{
		flip(result_CT, result_temp,-1);
		result_CT = result_temp;
	}

	imshow("result", result_CT);
////////
	//IplImage frame = result_CT;
	//cvWriteFrame(video, &frame);
	//FrameCounter++;
	/////////
	waitKey(1);
	
	for (int i = 0; i < Num_Cams; i++)
	{
		IplImage *temp = cvQueryFrame(Capture[i]);
		//IplImage *t = cvCloneImage(temp);
		//cvRemap(t, temp, mapx, mapy);
		//cvReleaseImage(&t);
		Images[i] = temp;
	}


	for (int i = 0; i < THREADNUM; i++)
	{
		g_event[i].SetEvent();
	}
#ifdef RECORD
	if (FrameCounter==3000)
		SendMessage(WM_STOPREC, NULL, NULL);
#endif
	return 0;
}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//move up
	disp_origin.y = disp_origin.y - pow(-1, rotation) * 50;
	if (disp_origin.y>globalmap.m_mask.size().height - disp_height - 1)
		disp_origin.y = globalmap.m_mask.size().height - disp_height - 1;
	if (disp_origin.y < 0)
		disp_origin.y = 0;
	for (int i = 0; i < THREADNUM; i++)
	{
		*(Roi_thread + i) = Rect(disp_origin.x, disp_origin.y + i*((int)ceil(disp_height / THREADNUM)), disp_width, (int)ceil(disp_height / THREADNUM));
	}
}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	//move down
	disp_origin.y = disp_origin.y + pow(-1, rotation) * 50;
	if (disp_origin.y>globalmap.m_mask.size().height - disp_height - 1)
		disp_origin.y = globalmap.m_mask.size().height - disp_height - 1;
	if (disp_origin.y < 0)
		disp_origin.y = 0;
	for (int i = 0; i < THREADNUM; i++)
	{
		*(Roi_thread + i) = Rect(disp_origin.x, disp_origin.y + i*((int)ceil(disp_height / THREADNUM)), disp_width, (int)ceil(disp_height / THREADNUM));
	}
}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//move left
	disp_origin.x = disp_origin.x - pow(-1, rotation) * 50;
	if (disp_origin.x < 0)
		disp_origin.x = 0;
	if (disp_origin.x>globalmap.m_mask.size().width - disp_width - 1)
		disp_origin.x = globalmap.m_mask.size().width - disp_width - 1;
	for (int i = 0; i < THREADNUM; i++)
	{
		*(Roi_thread + i) = Rect(disp_origin.x, disp_origin.y + i*((int)ceil(disp_height / THREADNUM)), disp_width, (int)ceil(disp_height / THREADNUM));
	}
}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	//move right
	disp_origin.x = disp_origin.x + pow(-1,rotation)*50;
	if (disp_origin.x < 0)
		disp_origin.x = 0;
	if (disp_origin.x>globalmap.m_mask.size().width - disp_width - 1)
		disp_origin.x = globalmap.m_mask.size().width - disp_width - 1;
	for (int i = 0; i < THREADNUM; i++)
	{
		*(Roi_thread + i) = Rect(disp_origin.x, disp_origin.y + i*((int)ceil(disp_height / THREADNUM)), disp_width, (int)ceil(disp_height / THREADNUM));
	}

}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	cvReleaseVideoWriter(&video);

}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnCancel();
}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	//open a directory

	CString m_FileDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	BOOL bRet = FALSE;
	TCHAR szFolder[MAX_PATH * 2];
	szFolder[0] = _T('/0');
	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
			bRet = TRUE;
		IMalloc *pMalloc = NULL;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
			{
				pMalloc->Free(pidl);
				pMalloc->Release();
			}
	}
	m_FileDir = szFolder;//选择的文件夹路径  
	TRACE("/n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/n");
	TRACE(m_FileDir);
	TRACE("/n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/n");
	FolderPathName = m_FileDir;

	//FolderPathName.Format(_T("\\BG.xml"));

}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	//start the application
	char buffer[256];
	sprintf(buffer,"%S\\BG_Map.xml",FolderPathName);
	FileStorage fs(buffer, FileStorage::READ);

	fs["xmap"] >> globalmap.m_xmap;
	fs["ymap"] >> globalmap.m_ymap;
	fs["mask"] >> globalmap.m_mask;




	Mat ImageTemp;
	CvCapture* CaptureTemp;
	IplImage* frame;
	for (int i = 0; i < Num_Cams; i++)
	{
		sprintf(buffer, "%S\\Camera#%d.avi", FolderPathName,i);
		CaptureTemp = cvCreateFileCapture(buffer);
		Capture.push_back(CaptureTemp);
		frame = cvQueryFrame(CaptureTemp);

		//IplImage *t = cvCloneImage(frame);
		//cvRemap(t, frame, mapx, mapy);

		//cvReleaseImage(&t);
		ImageTemp = frame;
		//cvSaveImage("D:\\outputs\\test.jpg", frame);
		Images.push_back(ImageTemp);
	}
	globalmap.initialdst(Roi_disp.size());
	namedWindow("result", WINDOW_AUTOSIZE);

	for (int i = 0; i < THREADNUM; i++)
	{
		(pThreadData + i)->Images = &Images;
		(pThreadData + i)->Obj = &globalmap;
		(pThreadData + i)->Roi_dst = &Roi_disp;
		*(Roi_thread + i) = Rect(disp_origin.x, disp_origin.y + i*((int)ceil(disp_height / THREADNUM)), disp_width, (int)ceil(disp_height / THREADNUM));
		(pThreadData + i)->Roi = Roi_thread + i;
		(pThreadData + i)->ID = i;
		pThread[i] = AfxBeginThread(ThreadProc, (pThreadData + i), THREAD_PRIORITY_NORMAL);
		g_event[i].SetEvent();
	}
	SetTimer(1, 10, NULL);

}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here

	//globalmap.Calibrate(FolderPathName, Num_Cams);
	globalmap.MapGenerate(FolderPathName, Num_Cams);

}


afx_msg LRESULT CMFCApplication1_Multi_AlphaDlg::OnStoprec(WPARAM wParam, LPARAM lParam)
{
	return 0;
	cvReleaseVideoWriter(&video);
	MessageBox(_T("Done"));
}


void CMFCApplication1_Multi_AlphaDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	//bool rotationcheck;
	//if (rotationcheck == ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
	//{
	//}
	//MessageBox(_T("rotate"));
	rotation = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
}
