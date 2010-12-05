// FinalPlayerDlg.h : header file
//

#pragma once
#include "CS576SoundUtil.h"
#include "FrameReader.h"
#include "FrameManager.h"
#include "afxwin.h"
#include "afxcmn.h"

// CFinalPlayerDlg dialog
class CFinalPlayerDlg : public CDialog
{
// Construction
public:
	CFinalPlayerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FINALPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
			//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
     CSoundManager* g_pSoundManager;
     CSound*        g_pSound; 
     BOOL           g_bBufferPaused;
	 DWORD          g_dwAudioLen;
	 DWORD          g_dwAudioSize;
	 int            g_iTimer;
	 int            g_iTime;
	 FrameManager*  g_pFrameManager;
	 FrameReader*   g_pFrameReader;
	 bool           g_bDraw;
	 bool           g_isPlay;
	 bool           g_isSum;
	 DWORD          startT;
	 DWORD          endT;
	 DWORD          passT;
	 DWORD          g_dwCounter;

	 int            m_nCurHeight;
     int            m_nScrollPos;
     CRect          m_rect;
	 int            m_iSumIndex[10];



	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadbtn();
	CStatic m_FilePath;
	CButton m_PlayBtn;
	CButton m_StopBtn;
	afx_msg void OnNMReleasedcaptureTimesld(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_TimeSlider;
	CStatic m_TimeTxt;
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPlaybtn();
	afx_msg void OnBnClickedStopbtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedVedioplaybtn();
	afx_msg void OnNMCustomdrawTimesld(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedStopvediobtn();

	HRESULT AudioJumpping(double dPercent);
	bool VideoJumpping(double dPos);
	void ShowCurrentTime(int pos);
	bool PlayVedio();
	bool StopVedio();
	bool UpdateSlider();
	bool DrawSummary();
	
	CStatic m_VolumeTxt;
	
	void forTest();
	static UINT LoadThreadFun(LPVOID lpParam);
	CStatic m_PicCtr1;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_HScroll;
//	afx_msg void OnNMThemeChangedScrollbar1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnStnClickedPic1();
	afx_msg void OnStnDblclickPic1();
	afx_msg void OnBnClickedButton1();
	CStatic m_MovCtr;
	CEdit m_EditPic;
};
