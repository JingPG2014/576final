#include "stdafx.h"
#include "FrameManager.h"

FrameManager::FrameManager(FrameReader* reader)
{
	this->reader = reader;
	this->iHeight = reader->getHeight();
	this->iWidth = reader->getWidth();
	iFrameCount = 0;
	ready = true;//false;
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
	iFirstLoadingPos = iPos;
	return true;
}


//*************************************************************
// Name:
// Des: Set the size of buffer by seconds
//*************************************************************
bool FrameManager::setBufferSize(int iSize)
{
	this->iBufferSize = iSize;//this->iRate * iSec;
	iFirstLoadingPos = iSize/3;
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
//bool FrameManager::fillBuffer()
//{
//
//	chBuffer = new char*[iWidth*iHeight];
//	_loadBuffer(0,iBufferSize);
//	iCurrentPos = 0;
//	iStart = 0;
//	ready= true;
//	return true;
//}
//*************************************************************
// Name:
// Des:   Fill multiple buffers at different threads
//*************************************************************
bool FrameManager::fillBufferEx()
{

	chBuffer = 	new char*[iBufferSize];
	chBuffer2 = new char*[iBufferSize];
	//for(int i =0; i<5;i++){
	_loadBuffer(0,iBufferSize,chBuffer);
	AfxMessageBox(TEXT("i feel good"));
	/*_loadBuffer(0,iBufferSize,chBuffer2);
	AfxMessageBox(TEXT("i feel so good!"));*/
	//_loadBuffer(0,iBufferSize,chBuffer2);
	//AfxMessageBox(TEXT("i fell very good!"));
	//}
	iCurrentPos = 0;
	iStart = 0;
	ready= true;
	return true;
}

//*************************************************************
// Name:
// Des:  Refill the buffer from 0 to iCurrentPos
//*************************************************************
bool FrameManager::reFillBuffer()
{	
	loadBuffer(iStart, iStart+iFirstLoadingPos);
	iStart += iFirstLoadingPos;
	iStart %= iBufferSize;
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
	 
	/* param->chBuffer = chBuffer2;
	 AfxBeginThread(_loadBufferThread,(LPVOID)param);*/
		//_loadBuffer(start,end);

	return true;
}
//*************************************************************
// Name:
// Des:  Load buffer
// Normal version!
//*************************************************************
bool FrameManager::_loadBuffer(int start, int end, char ** chBuffer)
{
	for(int i =start; i<end; i++)
	{
		if(reader->ReadOneFrame())
		{
			chBuffer[i] = reader->getFrameData();
		}
			
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

	delete lpParam;

	for(int i =start; i<end; i++)
	{
		if(reader->ReadOneFrame())
		{
			delete chBuffer[i];//char* oldFrame = chBuffer[i];
			chBuffer[i] = reader->getFrameData();
			//delete oldFrame;
		}
			
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

/*	if(iPos>=iRangA && iPos<iRangB)
	{
		iCurrentPos = (iStart + (iPos - iRangA))%iBufferSize;
	}
	else 
	{
	*/
	   // delete chBuffer;
	/*for(int i = 0;i<iBufferSize;i++)
		delete chBuffer[i];*/
	reader->setPos(iPos);
	//fillBufferEx();
//	}
	
	return true;
}

//*************************************************************
// Name:
// Des:  Render one frame
// Update the current position of buffer
//*************************************************************
char* FrameManager::renderOneFrame()
{
	
	int currentPosition= iCurrentPos;
	if(!bIsEnd){
	iCurrentPos++;
	iFrameCount++;
	if(iCurrentPos % iFirstLoadingPos == 0)//== iFirstLoadingPos|| iCurrentPos == iThirdLoadingPos||iCurrentPos == iBufferSize)
		reFillBuffer();
	iCurrentPos %= iBufferSize;
	}
	return chBuffer[currentPosition];
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
	//fillBufferEx();
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

	if(reader->ReadOneFrame())
	{
		SetDIBitsToDevice(hdc,
					  0,0,iWidth,iHeight,
					  0,0,0,iHeight,
					  reader->getFrameData(),&bmi,DIB_RGB_COLORS);
	}
	else
		bIsEnd = true;
	EndPaint(hWnd,&ps);
	iFrameCount++;
	//RedrawWindow(hWnd,NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
	
	return true;
}
//*************************************************************
// Name:
// Des:  Draw a summary on the dlg
//*************************************************************
bool FrameManager::drawSummay(HWND hWnd,int* index)
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
	bmi.bmiHeader.biWidth = iWidth/iSumRatio;
	bmi.bmiHeader.biHeight = -iHeight/iSumRatio;  // Use negative height.  DIB is top-down.
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = (iWidth/iSumRatio) * (iHeight/iSumRatio);


	int x,y;

    for(int i =0;i<10;i++)
    {
		x = (i)*(iWidth/iSumRatio+0);
		y = 0;// i<5?0:iHeight/iSumRatio;
		reader->ReadSummary(index[i], iSumRatio);
		SetDIBitsToDevice(hdc,
					  x,y,iWidth/iSumRatio,iHeight/iSumRatio,
					  0,0,0,iHeight/iSumRatio,
					  reader->getSummaryData(),&bmi,DIB_RGB_COLORS);
    }
	
	EndPaint(hWnd,&ps);
    //RedrawWindow(hWnd,NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW  );
	
	return true;
}