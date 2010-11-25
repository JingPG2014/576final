#include "stdafx.h"
#include "FrameManager.h"

FrameManager::FrameManager(FrameReader* reader)
{
	this->reader = reader;
	this->iHeight = reader->getHeight();
	this->iWidth = reader->getWidth();
	iFrameCount = 0;
	ready = false;
	bIsEnd = false;
}

FrameManager::~FrameManager()
{
}



//*************************************************************
// Name:
// Des:  Set the Height of the frame
//*************************************************************
bool FrameManager::setHeight(int iHeight)
{
	this->iHeight = iHeight;
	reader->setHeight(iHeight);
	return true;
}


//*************************************************************
// Name:
// Des:  Set the Width of the frame
//*************************************************************
bool FrameManager::setWidth(int iWidth)
{
	this->iWidth = iWidth;
	reader->setWidth(iWidth);
	return true;
}


//*************************************************************
// Name:
// Des:  Set the refill position
//*************************************************************
bool FrameManager::setLoadingPos(int iPos)
{
	iLoadingPos = iPos;
	return true;
}


//*************************************************************
// Name:
// Des: Set the size of buffer by seconds
//*************************************************************
bool FrameManager::setBufferSize(int iSize)
{
	this->iBufferSize = iSize;//this->iRate * iSec;
	return true;
}

//*************************************************************
// Name:
// Des:  Set the frame rate, 30 for this
//*************************************************************
bool FrameManager::setFrameRate(int iRate)
{
	this->iRate = iRate;
	return true;
}


//*************************************************************
// Name:
// Des:   Fill the buffer for the first time
//*************************************************************
bool FrameManager::fillBuffer()
{

	chBuffer = new char*[iWidth*iHeight];
	_loadBuffer(0,iBufferSize);
	iCurrentPos = 0;
	iStart = 0;
    iRangA = iFrameCount; 
	iRangB = iRangA + iBufferSize;
	ready= true;
	return true;
}

//*************************************************************
// Name:
// Des:  Refill the buffer from 0 to iCurrentPos
//*************************************************************
bool FrameManager::reFillBuffer()
{
	if(iStart == 0)
	{
		loadBuffer(0, iLoadingPos);
		iStart = iLoadingPos;
		iRangA += iLoadingPos;
		iRangB = iRangA + iBufferSize;
	}
	else if(iStart == iLoadingPos)
	{
		loadBuffer(iLoadingPos, iBufferSize);
		iStart = 0;
		iRangA += iBufferSize - iLoadingPos;
		iRangB = iRangA + iBufferSize;
	}
	
	
	return true;
}

//*************************************************************
// Name:
// Des:  Call the Thread function _loadBuffer
//*************************************************************
bool FrameManager::loadBuffer(int start, int end)
{
	loadParam* param;
	param = (loadParam*)malloc(sizeof(loadParam));
	    param->reader = reader;
	    param->chBuffer = chBuffer;
	    param->start = start;
	    param->end = end;
		param->pbIsEnd = &bIsEnd;
	 AfxBeginThread(_loadBufferThread,(LPVOID)param);
		//_loadBuffer(start,end);

	return true;
}
//*************************************************************
// Name:
// Des:  Load buffer
// Normal version!
//*************************************************************
bool FrameManager::_loadBuffer(int start, int end)
{
	for(int i =start; i<end; i++)
	{
		if(reader->ReadOneFrame())
			chBuffer[i] = reader->getFrameData();
		else
		{
			bIsEnd = true;
			break;
		}
	}
	return true;
}
//*************************************************************
// Name:
// Des:  Load buffer
// Thread version!
//*************************************************************
UINT FrameManager::_loadBufferThread(LPVOID lpParam)
{
	loadParam* param = (loadParam*)lpParam;
	FrameReader* reader = (FrameReader*)param->reader;
	char** chBuffer= (char**)param->chBuffer;
	int start = (int)param->start;
	int end = (int)param->end;
	bool* pbIsEnd = (bool*)param->pbIsEnd;

	for(int i =start; i<end; i++)
	{
		if(reader->ReadOneFrame())
			chBuffer[i] = reader->getFrameData();
		else
		{
			*pbIsEnd = true;
			break;
		}
	}

	return(0);
}

//*************************************************************
// Name:
// Des:  Jump the video
//*************************************************************
bool  FrameManager::jump(int iPos)
{
	iFrameCount = iPos;

	if(iPos>=iRangA && iPos<iRangB)
	{
		iCurrentPos = (iStart + (iPos - iRangA))%iBufferSize;
	}
	else 
	{
		reader->setPos(iPos);
		fillBuffer();
	}
	
	return true;
}

//*************************************************************
// Name:
// Des:  Render one frame
// Update the current position of buffer
//*************************************************************
char* FrameManager::renderOneFrame()
{
	
	char* frame = chBuffer[iCurrentPos];
	if(!bIsEnd){
	iCurrentPos++;
	iFrameCount++;
	if(iCurrentPos == iLoadingPos||iCurrentPos == iBufferSize)
		reFillBuffer();
	iCurrentPos %= iBufferSize;
	}
	return frame;
}

//*************************************************************
// Name:
// Des:  Stop the video
// Reset the video stream and refresh the buffer
//*************************************************************
bool FrameManager::stop()
{
	iFrameCount = 0;
	reader->reset();
	fillBuffer();
	return true;
}

//*************************************************************
// Name:
// Des:  Draw a frame on the dlg
//*************************************************************
bool FrameManager::play(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd,&ps);
	
	// TODO: Add any drawing code here...
	RECT rt;
	GetClientRect(hWnd,&rt);
				
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

				
	SetDIBitsToDevice(hdc,
					  50,80,iWidth,iHeight,
					  0,0,0,iHeight,
					  renderOneFrame(),&bmi,DIB_RGB_COLORS);
	EndPaint(hWnd,&ps);
	//RedrawWindow(hWnd,NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
	
	return true;
}