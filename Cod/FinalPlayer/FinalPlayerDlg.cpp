// FinalPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FinalPlayer.h"
#include "FinalPlayerDlg.h"
#include <sstream>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//void CALLBACK RenderOneFrame(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime);


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFinalPlayerDlg dialog




CFinalPlayerDlg::CFinalPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinalPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFinalPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETXT, m_FilePath);
	DDX_Control(pDX, IDC_PLAYBTN, m_PlayBtn);
	DDX_Control(pDX, IDC_STOPBTN, m_StopBtn);
	DDX_Control(pDX, IDC_TIMESLD, m_TimeSlider);
	DDX_Control(pDX, IDC_TIMETXT, m_TimeTxt);
	DDX_Control(pDX, IDC_VLOUMETXT, m_VolumeTxt);
	DDX_Control(pDX, IDC_PIC1, m_PicCtr1);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_HScroll);
	DDX_Control(pDX, IDC_MOVPIC, m_MovCtr);
	DDX_Control(pDX, IDC_EDITPIC, m_EditPic);
}

BEGIN_MESSAGE_MAP(CFinalPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOADBTN, &CFinalPlayerDlg::OnBnClickedLoadbtn)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TIMESLD, &CFinalPlayerDlg::OnNMReleasedcaptureTimesld)
//	ON_BN_CLICKED(IDCANCEL, &CFinalPlayerDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDCANCEL, &CFinalPlayerDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_PLAYBTN, &CFinalPlayerDlg::OnBnClickedPlaybtn)
ON_BN_CLICKED(IDC_STOPBTN, &CFinalPlayerDlg::OnBnClickedStopbtn)
ON_BN_CLICKED(IDC_VEDIOPLAYBTN, &CFinalPlayerDlg::OnBnClickedVedioplaybtn)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_TIMESLD, &CFinalPlayerDlg::OnNMCustomdrawTimesld)
ON_BN_CLICKED(IDC_STOPVEDIOBTN, &CFinalPlayerDlg::OnBnClickedStopvediobtn)
ON_WM_VSCROLL()
//ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CFinalPlayerDlg::OnNMThemeChangedScrollbar1)
ON_WM_HSCROLL()
ON_STN_CLICKED(IDC_PIC1, &CFinalPlayerDlg::OnStnClickedPic1)
ON_STN_DBLCLK(IDC_PIC1, &CFinalPlayerDlg::OnStnDblclickPic1)
ON_BN_CLICKED(IDC_BUTTON1, &CFinalPlayerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CFinalPlayerDlg message handlers

BOOL CFinalPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetWindowRect(m_rect);
    m_nScrollPos = 0;
	CFinalPlayerDlg::m_HScroll.SetScrollRange(0,100);
	//CFinalPlayerDlg::m_PicCtr1.SetWindowPos(0,6,0,880,72,SWP_NOMOVE);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	/*int w=352, h=288;
	myImage.setWidth(w);
	myImage.setHeight(h);
*/
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
/**/	
	HRESULT hr;
	g_pSound = NULL;
	g_pSoundManager = new CSoundManager();
	g_pFrameReader = NULL;
	g_pFrameManager = NULL;

	if( FAILED( hr = g_pSoundManager->Initialize( this->m_hWnd, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        MessageBox(TEXT( "Error initializing DirectSound.  Sample will now exit."));
//        EndDialog( this->m_hWnd, IDABORT );
        return FALSE;
    }

    g_bBufferPaused = FALSE;
	g_bDraw = false;
	g_isPlay = false;
	g_isSum = false;

	// set slider
	CFinalPlayerDlg::m_TimeSlider.SetRange(0,1000);
	CFinalPlayerDlg::m_TimeSlider.SetPos(0);

	ShowCurrentTime(0);
	g_iTimer = 0;
	g_iTime = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFinalPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFinalPlayerDlg::OnPaint()
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
		if(g_isPlay)
			PlayVedio();

		if(g_isSum)
		    DrawSummary();
		//UpdateSlider();
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFinalPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFinalPlayerDlg::OnNMCustomdrawTimesld(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

//*************************************************************
// Name:
// Des:  On load button click
//*************************************************************
void CFinalPlayerDlg::OnBnClickedLoadbtn()
{
	// TODO: Add your control notification handler code here

	// set slider
	CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT("Loading file..."));
	HRESULT hr;

    static TCHAR strFileName[MAX_PATH] = TEXT("");
    static TCHAR strPath[MAX_PATH] = TEXT("");
	static CHAR VideoPath[MAX_PATH] = "";//TEXT("");

    // Setup the OPENFILENAME structure
	OPENFILENAME ofn = { sizeof(OPENFILENAME), this->m_hWnd, NULL,
                         TEXT("PCM Files\0*.pcm\0All Files\0*.*\0\0"), NULL,
                         0, 1, strFileName, MAX_PATH, NULL, 0, strPath,
                         TEXT("Open Sound File"),
                         OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 0, 0,
                         TEXT(".wav"), 0, NULL, NULL };
	

    // Get the default media path (something like C:\WINDOWS\MEDIA)
    if( '\0' == strPath[0] )
    {
        GetWindowsDirectory( strPath, MAX_PATH );
        if( wcscmp( &strPath[wcslen(strPath)], TEXT("\\") ) )
            wcscat( strPath, TEXT("\\") );
        wcscat( strPath, TEXT("MEDIA") );
    }

  /**/  if( g_pSound )
    {
        g_pSound->Stop();
        g_pSound->Reset();
    }

    // Update the UI controls to show the sound as loading a file
	//CFinalPlayerDlg::m_PlayBtn.EnableWindow(0);
	//CFinalPlayerDlg::m_StopBtn.EnableWindow(0);
  
    // Display the OpenFileName dialog. Then, try to load the specified file
   if( TRUE != GetOpenFileName( &ofn ) )
    {
        CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT("Load aborted.") );
        return;
    }

    CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT(""));

    // Free any previous sound, and make a new one
    SAFE_DELETE( g_pSound );

    // Load the wave file into a DirectSound buffer
    if( FAILED( hr = g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL ) ) )
    {
        // Not a critical failure, so just update the status
        DXTRACE_ERR_NOMSGBOX( TEXT("Create"), hr );
       CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT("Could not create sound buffer.") );
        return; 
    }

    // Update the UI controls to show the sound as the file is loaded
    CFinalPlayerDlg::m_FilePath.SetWindowTextW(strFileName );

	g_dwAudioLen = g_pSoundManager->GetLen();
	g_dwAudioSize = g_pSoundManager->GetSize();

    // Load video file that has same name with audio file
	USES_CONVERSION;
	char * _strFileName= T2A(strFileName);
	strcpy( VideoPath, _strFileName );
    char* VideoLastSlash = strrchr( VideoPath, '.' );
    VideoLastSlash[0] = '\0';
	strcat(VideoLastSlash,".rgb");	
	
	// Set up frameReader
	int w=352, h=288;
    g_pFrameReader = new FrameReader(VideoPath);
	g_pFrameReader->setHeight(h);
	g_pFrameReader->setWidth(w);

	// Set up frameReader
	g_pFrameManager = new FrameManager(g_pFrameReader);
	g_pFrameManager->setFrameRate(30);
	g_pFrameManager->setBufferSize(30*10);
	g_pFrameManager->setSumRatio(4);
	//g_pFrameManager->setLoadingPos(15);

	wstringstream ss;	
	
	CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT("Loading..."));

	// loading summary
	g_isSum = true;
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );

	startT = GetTickCount();
	//g_pFrameManager->jump(9000);
	//g_pFrameManager->fillBuffer();
	endT = GetTickCount();
	endT -= startT;
	ss<<endT;
	wstring strPos = ss.str();
	LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
	CFinalPlayerDlg::m_FilePath.SetWindowTextW(lpPos);
	
	
	


}


//************************
// On slider button released
//************************
void CFinalPlayerDlg::OnNMReleasedcaptureTimesld(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
/**/	
	
	int iPos = CFinalPlayerDlg::m_TimeSlider.GetPos();
	
	int iMax = CFinalPlayerDlg::m_TimeSlider.GetRangeMax();
	double dPercent = (double)iPos/iMax;
	

	// Video jump
	double dCurrentTime = g_dwAudioLen * dPercent;
	double dVideoJumpPos = dCurrentTime * g_pFrameManager->getFrameRate();
	VideoJumpping(dVideoJumpPos);
	
	// Audio jump
	AudioJumpping(dPercent);
	ShowCurrentTime((int)dCurrentTime);
	*pResult = 0;
}

//*************************************************************
// Name:
// Des: Update the position of the Sliderbar
// ONLY for test!
//**************************************************************
bool CFinalPlayerDlg::UpdateSlider()
{
	double dPercent;
	dPercent = (double)g_iTimer/(30*g_dwAudioLen);
	int iPos = CFinalPlayerDlg::m_TimeSlider.GetRangeMax() * dPercent;
	CFinalPlayerDlg::m_TimeSlider.SetPos(iPos);
	return true;
}

//*************************************************************
// Name:
// Des:  Audio jumpping
//*************************************************************
HRESULT CFinalPlayerDlg::AudioJumpping(double dPercent)
{
	HRESULT hr = 0;
	
	DWORD pos_byte = (DWORD)( g_pSoundManager->GetSize() *dPercent);

	g_pSound->SetPosition(pos_byte);

	return hr;
}


//*************************************************************
// Name:
// Des:  Video jumpping
//*************************************************************
bool CFinalPlayerDlg::VideoJumpping(double dPos)
{
	StopVedio();
	Sleep(2000);
	g_pFrameManager->jump(dPos);
	OnBnClickedVedioplaybtn();
	return true;
}
//*************************************************************
// Name:
// Des:  On exit button click
//*************************************************************
void CFinalPlayerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	 //KillTimer(1);
	 SAFE_DELETE( g_pSound );
     SAFE_DELETE( g_pSoundManager );
SAFE_DELETE(g_pFrameManager);
SAFE_DELETE(g_pFrameReader);


	OnCancel();
}

//*************************************************************
// Name:
// Des:  On play button click
//*************************************************************
void CFinalPlayerDlg::OnBnClickedPlaybtn()
{
	// TODO: Add your control notification handler code here
	HRESULT hr;

    //HWND hLoopButton = GetDlgItem(this->m_hWnd, IDC_LOOP_CHECK );
    BOOL bLooped = false;//( SendMessage( hLoopButton, BM_GETSTATE, 0, 0 ) == BST_CHECKED );	

	if( g_bBufferPaused )
    {
		OnBnClickedVedioplaybtn();
        // Play the buffer since it is currently paused
        DWORD dwFlags = bLooped ? DSBPLAY_LOOPING : 0L;
		if( FAILED( hr = g_pSound->Play( 0, dwFlags ) ) )
		{
			CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT("Play Failed!"));		
			return;
		
		}
		
           
		// return DXTRACE_ERR( TEXT("Play"), hr );

        // Update the UI controls to show the sound as playing
        g_bBufferPaused = FALSE;
		CFinalPlayerDlg::m_PlayBtn.SetWindowTextW(TEXT("Pause"));
//        EnablePlayUI(this->m_hWnd, FALSE );
    }
    else
    {
        if( g_pSound->IsSoundPlaying() )
        {
            // To pause, just stop the buffer, but don't reset the position
            if( g_pSound )
			{
                g_pSound->Stop();
				OnBnClickedStopvediobtn();
			}
            g_bBufferPaused = TRUE;

			CFinalPlayerDlg::m_PlayBtn.SetWindowTextW(TEXT("Play"));
        }
        else
        {
			OnBnClickedVedioplaybtn();
            // The buffer is not playing, so play it again
            DWORD dwFlags = bLooped ? DSBPLAY_LOOPING : 0L;
			if( FAILED( hr = g_pSound->Play( 0, dwFlags ) ) )
			{
				CFinalPlayerDlg::m_FilePath.SetWindowTextW(TEXT("Play Failed!"));
			    return;
			}

            // Update the UI controls to show the sound as playing
            g_bBufferPaused = FALSE;
            //EnablePlayUI(this->m_hWnd, FALSE );
			CFinalPlayerDlg::m_PlayBtn.SetWindowTextW(TEXT("Pause"));
        }
    }

}

//*************************************************************
//  Name:
//  Des: On stop button click
//*************************************************************
void CFinalPlayerDlg::OnBnClickedStopbtn()
{
	// TODO: Add your control notification handler code here
	if( g_pSound )
    {    
		CFinalPlayerDlg::m_PlayBtn.SetWindowTextW(TEXT("Play"));
		g_pSound->Stop();	
		g_pSound->Reset();
		g_pFrameManager->stop();
		StopVedio();
		CFinalPlayerDlg::m_TimeSlider.SetPos(0);
    }
}


//*************************************************************
// Name:
// Des: Show current time aside the slider
//*************************************************************
void CFinalPlayerDlg::ShowCurrentTime(int pos)
{
	int seconds;
	int mins;
	int hours;
	wstringstream ss;
	

	hours = pos/3600;
	pos %= 3600;
	
	mins = pos/60;
	
	seconds = pos%60;
	if(hours<10)
		ss<<"0"<<hours;
	else
		ss<<hours;
	ss<<":";
	if(mins<10)
		ss<<"0"<<mins;
	else
		ss<<mins;
	ss<<":";
	if(seconds<10)
		ss<<"0"<<seconds;
	else
		ss<<seconds;
	wstring strPos = ss.str();
	LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
	CFinalPlayerDlg::m_TimeTxt.SetWindowTextW(lpPos);

}
//*************************************************************
// Name:
// Des:Play video
//*************************************************************
bool CFinalPlayerDlg::PlayVedio()
{
	//this->SetTimer(1,1000,NULL);
	//g_pFrameManager->jump(1600);
	if(g_pFrameManager->isEnd())
	{
		StopVedio();
		return false;
	}
	if(g_pFrameManager->isReady()){
		//this->Invalidate();
        g_pFrameManager->play(CFinalPlayerDlg::m_MovCtr.m_hWnd);
	    g_isPlay = false;
	}
	//RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
	
	
/*PAINTSTRUCT ps;
	HDC hdc;

	//hdc= this->GetDC();
	hdc = BeginPaint(&ps)->m_hDC;
	
	
	// TODO: Add any drawing code here...
	RECT rt;
	GetClientRect(&rt);
	int iWidth = g_pFrameManager->getWidth();
	int iHeight = g_pFrameManager->getHeight();
		
	//WCHAR text[71];		
	//wstrcpy(text, "Display original image and Image after Compression/Decompression here\n");
	//text = TEXT("Display original image and Image after Compression/Decompression here\n");
	DrawTextW(hdc, TEXT("Display original image and Image after Compression/Decompression here\n"), 71, &rt, DT_LEFT);
	
	BITMAPINFO bmi;
	CBitmap bitmap;
	memset(&bmi,0,sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = iWidth;
	bmi.bmiHeader.biHeight = -iHeight;  // Use negative height.  DIB is top-down.
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = iWidth * iHeight;

	char* color = g_pFrameReader->getFrameData();
	SetDIBitsToDevice(hdc,
					  0,100,iWidth,iHeight,
					  0,0,0,iHeight,
					  color,&bmi,DIB_RGB_COLORS);
					  
	EndPaint(&ps);
	*/
	return true;
}

//*************************************************************
// Name: OnTimer
// Des:  Draw one frame
//*************************************************************
void CFinalPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	g_dwCounter = g_pFrameManager->getFrameCount();
	if(g_dwCounter==0)
	{
		startT = GetTickCount();
	}
	if(g_dwCounter%15==0)
	{

	passT = GetTickCount();
		wstringstream ss;
		
		passT -= startT;
		if(passT<500)
			Sleep(500-passT);
		endT = GetTickCount();
		endT -= startT;
		ss<<endT;
		wstring strPos = ss.str();
	LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
	CFinalPlayerDlg::m_FilePath.SetWindowTextW(lpPos);
		startT = GetTickCount();
	}/**/
//	g_iTime++;
    wstringstream ss;
	ss<<g_dwCounter;
	wstring strPos = ss.str();
	LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
	CFinalPlayerDlg::m_VolumeTxt.SetWindowTextW(lpPos);
	g_isPlay = true;
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
	
}

//*************************************************************
// Name:
// Des:  Pause the video
//*************************************************************
bool CFinalPlayerDlg::StopVedio()
{
	KillTimer(1);
	return true;
}
//*************************************************************
// Name:
// Des:  On video play button click
// ONLY for test!
//*************************************************************
void CFinalPlayerDlg::OnBnClickedVedioplaybtn()
{
	// TODO: Add your control notification handler code here
	//PlayVedio();
	//g_bDraw =!g_bDraw;
	//RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
	//forTest();
	if(g_pFrameManager== NULL)
		return;

	DWORD startT,passT,endT,sleepT;
	 wstringstream ss;
	//g_pFrameManager->jump(1600);
	this->SetTimer(1,31,NULL);
	
/*int run =30;
    startT = GetTickCount();
	while(run)
	{
		g_isPlay = true;	
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
		//passT = GetTickCount();
		//passT -= startT;
		//sleepT = 33-passT;
		//Sleep(sleepT);
		
		g_iTime++;
        wstringstream ss1;
	    ss1<<g_iTime;
	    wstring strPtime = ss1.str();
	    LPCTSTR lpPtime = (LPCTSTR)strPtime.c_str();
	    CFinalPlayerDlg::m_VolumeTxt.SetWindowTextW(lpPtime);
		run--;
	};
	endT = GetTickCount();
		endT -= startT;
		ss.clear();
		ss<<endT;
		wstring strPos = ss.str();
	    LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
	    CFinalPlayerDlg::m_FilePath.SetWindowTextW(lpPos);*/
}

//*************************************************************
// Name:
// Des: On video pasuse button click
// ONLY for test!!!!!!!!!
//*************************************************************
void CFinalPlayerDlg::OnBnClickedStopvediobtn()
{
	// TODO: Add your control notification handler code here
	StopVedio();

}

//*************************************************************
// Name:
// Des:
// ONLY for test!
//**************************************************************
void CFinalPlayerDlg::forTest()
{
	if(!g_bDraw){
		this->Invalidate();
		return;
	}
	PAINTSTRUCT ps;
	HDC hdc;

	//hdc= this->GetDC();
	hdc = BeginPaint(&ps)->m_hDC;
	
	
	// TODO: Add any drawing code here...
	RECT rt;
	GetClientRect(&rt);
	DrawText(hdc, TEXT("12345678"), 8, &rt, DT_LEFT);

	EndPaint(&ps);
}

//*************************************************************
// Name:
// Des:
// ONLY for test!
//**************************************************************
UINT CFinalPlayerDlg::LoadThreadFun(LPVOID lpParam)
{
/*	FrameManager* g_pFrameManager;
	g_pFrameManager = (FrameManager*)lpParam;
	g_pFrameManager->fillBuffer();
*/
	return(0);
}
//*************************************************************
// Name:
// Des: Draw summary
//**************************************************************
bool  CFinalPlayerDlg::DrawSummary()
{
	//g_pFrameManager->play(CFinalPlayerDlg::m_PicCtr1.m_hWnd);//this->m_hWnd);
	int index[10]={1261,10666,11521,13366,13411,13501,14401,144446,17101,17056};
	int init = 0;
/**/	for(int i=0;i<10;i++)
	{
		init += 1500;
		index[i] = init;
	}
	//m_iSumIndex = index;
	//g_pFrameManager->drawSummay(CFinalPlayerDlg::m_PicCtr1.m_hWnd, index);
	g_pFrameManager->drawSummay(CFinalPlayerDlg::m_EditPic.m_hWnd, index);
	//g_isSum = false;
	return true;
}
void CFinalPlayerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int nDelta;
	int nMaxPos = m_rect.Height() - m_nCurHeight;


	switch (nSBCode)
	{
/*	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/100,nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/100,m_nScrollPos);
		break;

         case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/100,nMaxPos-m_nScrollPos);
		break;
*/
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

/*	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/10,m_nScrollPos);
		break;
*/	
         default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	ScrollWindow(0,-nDelta);


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CFinalPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int nDelta;
	switch (nSBCode)
	{
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		m_nScrollPos += nDelta;
		CFinalPlayerDlg::m_HScroll.SetScrollPos(m_nScrollPos);
		CFinalPlayerDlg::m_PicCtr1.ScrollWindow(-nDelta,0);
		break;

    default:
		return;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFinalPlayerDlg::OnStnClickedPic1()
{
	// TODO: Add your control notification handler code here
	CPoint point; 
    GetCursorPos( &point ); 

	CRect rect; 
	CFinalPlayerDlg::m_PicCtr1.GetClientRect(&rect);
	CFinalPlayerDlg::m_PicCtr1.ScreenToClient(&point);

	if(rect.PtInRect(point)) 
    { 
		 wstringstream ss;
		 ss<<point.x<<" , "<<point.y;
         wstring strPos = ss.str();
	     LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
         AfxMessageBox(lpPos); 
     } 

}

void CFinalPlayerDlg::OnStnDblclickPic1()
{
	// TODO: Add your control notification handler code here
	AfxMessageBox(TEXT("123123123")); 
}

void CFinalPlayerDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	AfxMessageBox(TEXT("123123123"));
}
