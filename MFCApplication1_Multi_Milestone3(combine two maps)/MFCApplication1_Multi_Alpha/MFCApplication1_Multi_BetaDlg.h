
// MFCApplication1_Multi_AlphaDlg.h : header file
//

#pragma once
#include"Resource.h"


#define WM_PULSE WM_APP+100
#define WM_SHOW_UPDATA WM_USER+101
#define WM_STOPREC WM_USER+102

// CMFCApplication1_Multi_AlphaDlg dialog
class CMFCApplication1_Multi_AlphaDlg : public CDialogEx
{
// Construction
public:
	CMFCApplication1_Multi_AlphaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCAPPLICATION1_MULTI_ALPHA_DIALOG };

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
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	afx_msg LRESULT OnPulse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowUpdata(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
protected:
	afx_msg LRESULT OnStoprec(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedCheck1();
};
