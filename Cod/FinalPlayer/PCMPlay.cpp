//----------------------------------------------------------------------------
// File: PCMPlay.cpp
//
// Author- Parag Havaldar
// Desc: The PCMPlay sample shows how to load and play a PCM file using
//       a DirectSound buffer.
//
//-----------------------------------------------------------------------------
#include "stdafx.h"

#define STRICT

//#include <windows.h>
//#include "basetsd.h"
//#include <commdlg.h>
//#include <mmreg.h>
//#include <dxerr8.h>
//#include <dsound.h>
#include "resource.h"
#include "CS576SoundUtil.h"
//#include <afxcmn.h>
//#include "DXUtil.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Image.h"
//#include <afxcmn.h>  

#define MAX_LOADSTRING 100

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
VOID    OnInitDialog( HWND hDlg );
VOID    OnOpenSoundFile( HWND hDlg );
HRESULT OnPlaySound( HWND hDlg );
HRESULT PlayBuffer( BOOL bLooped );
VOID    OnTimer( HWND hDlg );
VOID    EnablePlayUI( HWND hDlg, BOOL bEnable );
VOID    showCurrentPos(HWND hDlg);
VOID    OnHScroll( HWND hDlg ); 

TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
CSoundManager* g_pSoundManager = NULL;
CSound*        g_pSound = NULL;
BOOL           g_bBufferPaused;
CSliderCtrl*   g_pSlider;

MyImage myImage;


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application.  Since we use a simple dialog for 
//       user interaction we don't need to pump messages.
//-----------------------------------------------------------------------------
INT APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, 
                      INT nCmdShow )
{
 	int w=352, h=288;
//	char ImagePath[_MAX_PATH]="image1.rgb";
//	sscanf(pCmdLine, "%s %d %d", &ImagePath, &w, &h);
	myImage.setWidth(w);
	myImage.setHeight(h);
//	myImage.setImagePath(ImagePath);
//	myImage.ReadImage();

	// Display the main dialog box.
    DialogBox( hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc );
    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: MainDlgProc()
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    HRESULT hr;
	PAINTSTRUCT ps;
	HDC hdc;

    switch( msg ) 
    {
        case WM_INITDIALOG:
            OnInitDialog( hDlg );
            break;

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
				case IDC_SOUNDFILE:					
                    OnOpenSoundFile( hDlg );
                    break;

                case IDCANCEL:
                    EndDialog( hDlg, IDCANCEL );
                    break;

                case IDC_PLAY:
                    // The 'play'/'pause' button was pressed
					
					RedrawWindow(hDlg, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
                    if( FAILED( hr = OnPlaySound( hDlg ) ) )
                    {
                        DXTRACE_ERR( TEXT("OnPlaySound"), hr );
                        MessageBox( hDlg, "Error playing DirectSound buffer. "
                                    "Sample will now exit.", "DirectSound Sample", 
                                    MB_OK | MB_ICONERROR );
                        EndDialog( hDlg, IDABORT );
                    }
                    break;

                case IDC_STOP:
                    if( g_pSound )
                    {
                        g_pSound->Stop();
						g_pSound->SetPosition(1500000);
                    }

                    EnablePlayUI( hDlg, TRUE );
                    break;

                default:
                    return FALSE; // Didn't handle message            
			}
			break;
		case WM_HSCROLL:
			{
					OnHScroll(hDlg);
			}
			break;
		
		case WM_PAINT:
			{
	
				hdc = BeginPaint(hDlg, &ps);
	/*			// TODO: Add any drawing code here...
				RECT rt;
				GetClientRect(hDlg, &rt);

			BITMAPINFO bmi;
				memset(&bmi,0,sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biWidth = myImage.getWidth();
				bmi.bmiHeader.biHeight = -myImage.getHeight();  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biSizeImage = myImage.getWidth()*myImage.getHeight();

				// Display original image
				SetDIBitsToDevice(hdc,
								 70,70,myImage.getWidth(),myImage.getHeight(),
								  0,0,0,myImage.getHeight(),
								  myImage.getImageData(),&bmi,DIB_RGB_COLORS);
				*/EndPaint(hDlg, &ps);

			}				
			break;

        case WM_TIMER:
            OnTimer( hDlg );
            break;

        case WM_DESTROY:
            // Cleanup everything
            KillTimer( hDlg, 1 );    
            SAFE_DELETE( g_pSound );
            SAFE_DELETE( g_pSoundManager );
            break; 

        default:
            return FALSE; // Didn't handle message
    }

    return TRUE; // Handled message
}




//-----------------------------------------------------------------------------
// Name: OnInitDialog()
// Desc: Initializes the dialogs (sets up UI controls, etc.)
//-----------------------------------------------------------------------------
VOID OnInitDialog( HWND hDlg )
{
    HRESULT hr;

    // Load the icon
#ifdef _WIN64
    HINSTANCE hInst = (HINSTANCE) GetWindowLongPtr( hDlg, GWLP_HINSTANCE );
#else
    HINSTANCE hInst = (HINSTANCE) GetWindowLong( hDlg, GWL_HINSTANCE );
#endif
    HICON hIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDR_MAINFRAME ) );

    // Set the icon for this dialog.
    SendMessage( hDlg, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
    SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon

    // Create a static IDirectSound in the CSound class.  
    // Set coop level to DSSCL_PRIORITY, and set primary buffer 
    // format to stereo, 22kHz and 16-bit output.
    g_pSoundManager = new CSoundManager();

    if( FAILED( hr = g_pSoundManager->Initialize( hDlg, DSSCL_PRIORITY, 2, 22050, 16 ) ) )
    {
        DXTRACE_ERR( TEXT("Initialize"), hr );
        MessageBox( hDlg, "Error initializing DirectSound.  Sample will now exit.", 
                            "DirectSound Sample", MB_OK | MB_ICONERROR );
        EndDialog( hDlg, IDABORT );
        return;
    }

    g_bBufferPaused = FALSE;

    // Create a timer, so we can check for when the soundbuffer is stopped
    SetTimer( hDlg, 0, 250, NULL );

    // Set the UI controls
    SetDlgItemText( hDlg, IDC_FILENAME, TEXT("No file loaded.") );

	// Set up the Slider control 
/*	g_pSlider=(CSliderCtrl*)GetDlgItem(hDlg,IDC_TIMESLIDER);
	g_pSlider->SetRange(0,100);
	int pos = g_pSlider->GetPos();
	g_pSlider->SetPos(50);*/
}




//-----------------------------------------------------------------------------
// Name: OnOpenSoundFile()
// Desc: Called when the user requests to open a sound file
//-----------------------------------------------------------------------------
VOID OnOpenSoundFile( HWND hDlg ) 
{
    HRESULT hr;

    static TCHAR strFileName[MAX_PATH] = TEXT("");
    static TCHAR strPath[MAX_PATH] = TEXT("");
	static TCHAR VideoPath[MAX_PATH] = TEXT("");

    // Setup the OPENFILENAME structure
    OPENFILENAME ofn = { sizeof(OPENFILENAME), hDlg, NULL,
                         TEXT("PCM Files\0*.pcm\0All Files\0*.*\0\0"), NULL,
                         0, 1, strFileName, MAX_PATH, NULL, 0, strPath,
                         TEXT("Open Sound File"),
                         OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, 0, 0,
                         TEXT(".wav"), 0, NULL, NULL };

    // Get the default media path (something like C:\WINDOWS\MEDIA)
    if( '\0' == strPath[0] )
    {
        GetWindowsDirectory( strPath, MAX_PATH );
        if( strcmp( &strPath[strlen(strPath)], TEXT("\\") ) )
            strcat( strPath, TEXT("\\") );
        strcat( strPath, TEXT("MEDIA") );
    }

    if( g_pSound )
    {
        g_pSound->Stop();
        g_pSound->Reset();
    }

    // Update the UI controls to show the sound as loading a file
    EnableWindow(  GetDlgItem( hDlg, IDC_PLAY ), FALSE);
    EnableWindow(  GetDlgItem( hDlg, IDC_STOP ), FALSE);
    SetDlgItemText( hDlg, IDC_FILENAME, TEXT("Loading file...") );

    // Display the OpenFileName dialog. Then, try to load the specified file
    if( TRUE != GetOpenFileName( &ofn ) )
    {
        SetDlgItemText( hDlg, IDC_FILENAME, TEXT("Load aborted.") );
        return;
    }

    SetDlgItemText( hDlg, IDC_FILENAME, TEXT("") );

    // Free any previous sound, and make a new one
    SAFE_DELETE( g_pSound );

    // Load the wave file into a DirectSound buffer
    if( FAILED( hr = g_pSoundManager->Create( &g_pSound, strFileName, 0, GUID_NULL ) ) )
    {
        // Not a critical failure, so just update the status
        DXTRACE_ERR_NOMSGBOX( TEXT("Create"), hr );
        SetDlgItemText( hDlg, IDC_FILENAME, TEXT("Could not create sound buffer.") );
        return; 
    }

    // Update the UI controls to show the sound as the file is loaded
    SetDlgItemText( hDlg, IDC_FILENAME,strFileName );
    EnablePlayUI( hDlg, TRUE );


    // Remember the path for next time
    strcpy( strPath, strFileName );
	strcpy( VideoPath, strFileName );
    char* strLastSlash = strrchr( strPath, '\\' );
    strLastSlash[0] = '\0';

	// Load video file that has same name with audio file
    char* VideoLastSlash = strrchr( VideoPath, '.' );
    VideoLastSlash[0] = '\0';
	strcat(VideoLastSlash,".rgb");
	myImage.setImagePath(VideoPath);
	//myImage.ReadImage();

}




//-----------------------------------------------------------------------------
// Name: OnPlaySound()
// Desc: User hit the "Play" button
//-----------------------------------------------------------------------------
HRESULT OnPlaySound( HWND hDlg ) 
{
    HRESULT hr;

    HWND hLoopButton = GetDlgItem( hDlg, IDC_LOOP_CHECK );
    BOOL bLooped = ( SendMessage( hLoopButton, BM_GETSTATE, 0, 0 ) == BST_CHECKED );	

	if( g_bBufferPaused )
    {
        // Play the buffer since it is currently paused
        DWORD dwFlags = bLooped ? DSBPLAY_LOOPING : 0L;
        if( FAILED( hr = g_pSound->Play( 0, dwFlags ) ) )
            return DXTRACE_ERR( TEXT("Play"), hr );

        // Update the UI controls to show the sound as playing
        g_bBufferPaused = FALSE;
        EnablePlayUI( hDlg, FALSE );
    }
    else
    {
        if( g_pSound->IsSoundPlaying() )
        {
            // To pause, just stop the buffer, but don't reset the position
            if( g_pSound )
                g_pSound->Stop();

            g_bBufferPaused = TRUE;
            SetDlgItemText( hDlg, IDC_PLAY, "Play" );
        }
        else
        {
            // The buffer is not playing, so play it again
            DWORD dwFlags = bLooped ? DSBPLAY_LOOPING : 0L;
            if( FAILED( hr = g_pSound->Play( 0, dwFlags ) ) )
                return DXTRACE_ERR( TEXT("Play"), hr );

            // Update the UI controls to show the sound as playing
            g_bBufferPaused = FALSE;
            EnablePlayUI( hDlg, FALSE );
        }
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: OnHScroll()
// Desc: When scrolling the HSlider 
//-----------------------------------------------------------------------------
VOID OnHScroll( HWND hDlg ) 
{
	int pos;
	stringstream ss;
	string strPos;
	LPCSTR lpPos;
	g_pSlider = (CSliderCtrl*)GetDlgItem(hDlg,IDC_TIMESLIDER);
	pos = g_pSlider->GetPos();
	ss<<pos;
	strPos = ss.str();
    lpPos = strPos.c_str();
   SetDlgItemText( hDlg, IDC_TIMESTATIC, TEXT("123213123") );
}


//-----------------------------------------------------------------------------
// Name: OnTimer()
// Desc: When we think the sound is playing this periodically checks to see if 
//       the sound has stopped.  If it has then updates the dialog.
//-----------------------------------------------------------------------------
VOID OnTimer( HWND hDlg ) 
{
    if( IsWindowEnabled( GetDlgItem( hDlg, IDC_STOP ) ) )
    {
        // We think the sound is playing, so see if it has stopped yet.
        if( !g_pSound->IsSoundPlaying() ) 
        {
            // Update the UI controls to show the sound as stopped
            EnablePlayUI( hDlg, TRUE );
        }
    }
}




//-----------------------------------------------------------------------------
// Name: EnablePlayUI( hDlg,)
// Desc: Enables or disables the Play UI controls 
//-----------------------------------------------------------------------------
VOID EnablePlayUI( HWND hDlg, BOOL bEnable )
{
    if( bEnable )
    {
        EnableWindow(   GetDlgItem( hDlg, IDC_LOOP_CHECK ), TRUE );
        EnableWindow(   GetDlgItem( hDlg, IDC_STOP ),       FALSE );

        EnableWindow(   GetDlgItem( hDlg, IDC_PLAY ),       TRUE );
        SetFocus(       GetDlgItem( hDlg, IDC_PLAY ) );
        SetDlgItemText( hDlg, IDC_PLAY, "&Play" );
    }
    else
    {
        EnableWindow(  GetDlgItem( hDlg, IDC_LOOP_CHECK ), FALSE );
        EnableWindow(  GetDlgItem( hDlg, IDC_STOP ),       TRUE );
        SetFocus(      GetDlgItem( hDlg, IDC_STOP ) );

        EnableWindow(  GetDlgItem( hDlg, IDC_PLAY ),       TRUE );
        SetDlgItemText( hDlg, IDC_PLAY, "&Pause" );
    }
}



// MyImage functions defined here

void MyImage::ReadImage()
{

	FILE *IN_FILE;
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	IN_FILE = fopen(ImagePath, "rb");

	if (IN_FILE == NULL) 
	{
		fprintf(stderr, "Error");
		exit(0);
	}
	
	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}

	Data = new char[Width*Height*3];

	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	
	fclose(IN_FILE);

}

VOID showCurrentPos(HWND hDlg){
	int *intPos;
	stringstream ss;
	string strPos;
	LPCSTR lpPos;
	LPDWORD currentPos;
	if(!FAILED(g_pSound->GetCurrentPosition())){
		currentPos=g_pSound->CurrentPosition();
	}
	//currentPos = g_pSound->GetCurrentPosition();
	intPos = (int *)currentPos;
	//intPos = 1;
	ss<<intPos;
	strPos = ss.str();
	lpPos = strPos.c_str();
	SetDlgItemText( hDlg, IDC_FILENAME, TEXT("123456") );
	SetDlgItemText( hDlg, IDC_FILENAME, lpPos );
}


