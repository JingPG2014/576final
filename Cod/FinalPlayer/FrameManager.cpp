#include "stdafx.h"
#include "FrameManager.h"

FrameManager::FrameManager(FrameReader* reader)
{
	this->reader = reader;
	this->iHeight = reader->getHeight();
	this->iWidth = reader->getWidth();
	iFrameCount = 0;
	ready = false;
}

FrameManager::~FrameManager()
{
}


/// Set the Height of the frame
bool FrameManager::setHeight(int iHeight)
{
	this->iHeight = iHeight;
	reader->setHeight(iHeight);
	return true;
}

/// Set the Width of the frame
bool FrameManager::setWidth(int iWidth)
{
	this->iWidth = iWidth;
	reader->setWidth(iWidth);
	return true;
}

/// Set the refill position
bool FrameManager::setLoadingPos(int iPos)
{
	iLoadingPos = iPos;
	return true;
}

/// Set the size of buffer by seconds
bool FrameManager::setBufferSize(int iSec)
{
	this->iBufferSize = this->iRate * iSec;
	return true;
}

/// Set the frame rate, 30 for this
bool FrameManager::setFrameRate(int iRate)
{
	this->iRate = iRate;
	return true;
}

//// Fill the buffer for the first time
bool FrameManager::fillBuffer()
{

	chBuffer = new char*[iWidth*iHeight];
	loadBuffer(0,iBufferSize);
	iCurrentPos = 0;
	iStart = 0;
    iRangA = iFrameCount; 
	iRangB = iRangA + iBufferSize;
	ready= true;
	return true;
}

/// Refill the buffer from 0 to iCurrentPos
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

/// Call the Thread function _loadBuffer
bool FrameManager::loadBuffer(int start, int end)
{
	loadParam* param;
	    param->reader = reader;
	    param->chBuffer = chBuffer;
	    param->start = start;
	    param->end = end;
	 AfxBeginThread(_loadBuffer,(LPVOID)param);

	return true;
}

/// Load the buffer in Thread
UINT FrameManager::_loadBuffer(LPVOID lpParam)
{
	loadParam* param = (loadParam*)lpParam;
	FrameReader* reader = (FrameReader*)param->reader;
	char** chBuffer= (char**)param->chBuffer;
	int start = (int)param->start;
	int end = (int)param->end;

	for(int i =start; i<end; i++)
	{
		reader->ReadOneFrame();
		chBuffer[i] = reader->getFrameData();
	}

	return(0);
}

/// Jump to the position
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

/// Reander a frame
char* FrameManager::renderOneFrame()
{
	iCurrentPos++;
	iFrameCount++;
	if(iCurrentPos == iLoadingPos)
		reFillBuffer();
	return chBuffer[iCurrentPos-1];
}

/// Stop
bool FrameManager::stop()
{
	iFrameCount = 0;
	reader->reset();
	fillBuffer();
	return true;
}

/// Play
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