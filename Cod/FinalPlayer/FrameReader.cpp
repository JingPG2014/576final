#include "stdafx.h"
#include "FrameReader.h"
#include <sstream>
#include <string>
using namespace std;


FrameReader::FrameReader(char* videoPath)
{
	//ifs.open(vedioPath,std::ios::in|std::ios::binary);
	//IN_FILE = fopen(vedioPath, "rb");
	beginFileMapping(videoPath);
	iFrameCount = 0;
	Data = NULL;
	SumData = NULL;
}

FrameReader::~FrameReader() 
{
	//if(IN_FILE)
		//ifs.close();
		//abort();
	endFileMapping();
	if(Data) 
		delete Data; 
};
//*************************************************************
// Name:
// Des: begin file mapping
//*************************************************************
bool FrameReader::beginFileMapping(char* videoPath)
{
	/*wstringstream ss;
	ss<<g_dwCounter;
	wstring strPos = ss.str();
	LPCTSTR lpPos = (LPCTSTR)strPos.c_str();*/
	USES_CONVERSION;
	WCHAR * _videoPath= A2T(videoPath);

	// Create file object
    HANDLE hFile = CreateFile(_videoPath, 
		GENERIC_READ | GENERIC_WRITE,
        0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
       AfxMessageBox(TEXT("create file object error "));
        return false;
    }
    
	// create file map
    hFileMap = CreateFileMapping(hFile, 
		NULL, 
		PAGE_READWRITE, 
		0, 
		0, 
		NULL);
    if (hFileMap == NULL)
    {
       AfxMessageBox(TEXT("create file map error "));
        return false;
    }
    
	// get system granularity
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    DWORD dwGran = SysInfo.dwAllocationGranularity;
    
	// get file size
    DWORD dwFileSizeHigh;
    qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
    qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
    
	// close file object
    CloseHandle(hFile);
    
	// offset 
    qwFileOffset = 0;
    
	// block size
    dwBlockBytes = 5940 * dwGran;

	iFileCount = qwFileSize/dwBlockBytes==0?qwFileSize/dwBlockBytes:qwFileSize/dwBlockBytes+1;

	//pFileBeginA = new char*[iFileCount];
 //   if (qwFileSize < dwBlockBytes)
 //       dwBlockBytes = (DWORD)qwFileSize;
	//for(int i =0;i<iFileCount;i++)
	//{
	//	dwBlockBytes = qwFileSize-qwFileOffset>dwBlockBytes?dwBlockBytes: qwFileSize-qwFileOffset;
	//		
	//	pFileBegin[i] = (char *)MapViewOfFile(hFileMap,
	//		FILE_MAP_ALL_ACCESS, 
 //           (DWORD)(qwFileOffset >> 32), 
	//		(DWORD)(qwFileOffset & 0xFFFFFFFF),//dwFileSizeHigh, 
 //           dwBlockBytes);
 //       
	//		if (pFileBegin[i] == NULL)
 //           {
	//			wstringstream ss;
	//			DWORD error = GetLastError();
	//			string s = "view map error:";
	//            ss<<s<<error;
	//            wstring strPos = ss.str();
	//            LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
	//			AfxMessageBox(lpPos);
 //               return false;
 //           }

	//		qwFileOffset += dwBlockBytes;
	//		

	//}

	//pFileCurrent = pFileBegin[0];
	iFileCurrentIndex = 0;
        
	AfxMessageBox(TEXT("Load ok!"));
	return true;
}

//*************************************************************
// Name:
// Des: end file mapping
//*************************************************************
bool FrameReader::endFileMapping()
{
	UnmapViewOfFile(pFileBegin);
    CloseHandle(hFileMap);    
	return true;
}
//*************************************************************
// Name:
// Des: get a file map view by index
//*************************************************************
bool FrameReader::getFileView()
{
	if(pFileBegin!= NULL)
		UnmapViewOfFile(pFileBegin);

	qwFileOffset = iFileCurrentIndex * dwBlockBytes;
	dwBlockBytes = qwFileSize-qwFileOffset>dwBlockBytes?dwBlockBytes: qwFileSize-qwFileOffset;
			
		pFileBegin = (char *)MapViewOfFile(hFileMap,
			FILE_MAP_ALL_ACCESS, 
            (DWORD)(qwFileOffset >> 32), 
			(DWORD)(qwFileOffset & 0xFFFFFFFF),//dwFileSizeHigh, 
            dwBlockBytes);
        
			if (pFileBegin == NULL)
            {
				wstringstream ss;
				DWORD error = GetLastError();
	            ss<<error;
	            wstring strPos = ss.str();
	            LPCTSTR lpPos = (LPCTSTR)strPos.c_str();
				AfxMessageBox(lpPos);
                return false;
            }

			qwFileOffset += dwBlockBytes;
			pFileCurrent = pFileBegin;
	return true;
}
//*************************************************************
// Name: ReadOneFrame
// Des: Read one frame from rgb file
//*************************************************************
bool FrameReader::ReadOneFrame()
{

	/*if(feof(IN_FILE))
		return false;*/
	if(iFrameCount%1280 == 0)
	{
		if(iFileCurrentIndex == iFileCount)
			return false;
	
		/*if( g_pSound->IsSoundPlaying() )
	    {
			g_pSound->Stop();
	    }*/
		getFileView();
		iFileCurrentIndex++;
		/*DWORD dwFlags = 0L;
		if(g_pSound!=NULL)
			g_pSound->Play( 0, dwFlags );*/
		
	}
		

	if (pFileBegin == NULL)
        {
			AfxMessageBox(TEXT("bad pointer or the end!"));
            return false;
        }
	//FILE *IN_FILE;
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	//IN_FILE = fopen(ImagePath, "rb");
	
/*	if(ifs ==NULL)
	{
		fprintf(stderr, "Error");
		exit(0);
	}*/
	for (i = 0; i < Width*Height; i ++)
	{
		//Rbuf[i] = fgetc(IN_FILE);
		Rbuf[i] = *pFileCurrent++;
	}
	for (i = 0; i < Width*Height; i ++)
	{
		//Gbuf[i] = fgetc(IN_FILE);
		Gbuf[i] = *pFileCurrent++;
	}
	for (i = 0; i < Width*Height; i ++)
	{
		//Bbuf[i] = fgetc(IN_FILE);
		Bbuf[i] = *pFileCurrent++;
	}
 
    if(Data!= NULL)
		delete Data;
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
	
	iFrameCount++;
	return true;

}



//*************************************************************
// Name:
// Des: Read one frame from rgb file
//*************************************************************
bool FrameReader::ReadSummary()
{

	int Width = SumWidth;
	int Height = SumHeight;
	FILE *IN_SUM = fopen(SummaryPath, "rb");
	FILE *IN_IDX = fopen(IndexPath, "rb");
	if(IN_SUM == NULL)
		AfxMessageBox(TEXT("Sum file cant open!"));
	if(IN_IDX ==NULL )
		AfxMessageBox(TEXT("Indxe file cant open!"));
	/*if(feof(IN_FILE))
		return false;*/
	if (pFileBegin == NULL)
        {
			AfxMessageBox(TEXT("bad pointer or the end!"));
            return false;
        }
	//FILE *IN_FILE;
	int i;
	/*char *Rbuf = new char[Width*Height]; 
	char *Gbuf = new char[Width*Height]; 
	char *Bbuf = new char[Width*Height]; */

	/*int dWidth = Width/ratio;
	int dHeight = Height/ratio;
	char *dRbuf = new char[dWidth*dHeight]; 
	char *dGbuf = new char[dWidth*dHeight]; 
	char *dBbuf = new char[dWidth*dHeight];*/ 

	// find the frame by index
	//setPos(index);

	//for (i = 0; i < Width*Height; i ++)
	//{
	//	Rbuf[i] = fgetc(IN_SUM);
	//	//Rbuf[i] =  *pFileCurrent++;
	//}
	//
	//for (i = 0; i < Width*Height; i ++)
	//{
	//	Gbuf[i] = fgetc(IN_SUM);
	//	//Gbuf[i] =  *pFileCurrent++;
	//}
	//for (i = 0; i < Width*Height; i ++)
	//{
	//	Bbuf[i] = fgetc(IN_SUM);
	//	//Bbuf[i] =  *pFileCurrent++;
	//}

	// down sampling
	/*for(int h=0;h<dHeight;h++)
		for(int w=0;w<dWidth;w++)
	{
		dRbuf[w+h*dWidth] = Rbuf[w*ratio+(h*ratio)*Width];
		dGbuf[w+h*dWidth] = Gbuf[w*ratio+(h*ratio)*Width];
		dBbuf[w+h*dWidth] = Bbuf[w*ratio+(h*ratio)*Width];

	}*/

	//delete SumData;
	SumData = new char[Height*Width*3];

	for (i = 0; i < Height*Width; i++)
	{
		SumData[3*i]	= fgetc(IN_SUM);//Rbuf[i];
		SumData[3*i+1]	= fgetc(IN_SUM);//Gbuf[i];
		SumData[3*i+2]	= fgetc(IN_SUM);//Bbuf[i];
	}

	IdxData = new int[Height*Width];
	int idx=0;
	/*for(int h=0;h<Height;h++)
		for(int w=0;w<Width;w++)
		{
		   fscanf(IN_IDX,"%d",&idx );
		   IdxData[w+h*Width] = idx;
		}*/
	fread(IdxData, sizeof(int), Width*Height, IN_IDX);

	/*delete Rbuf;
	delete Gbuf;
	delete Bbuf;*/

	/*delete dRbuf;
	delete dGbuf;
	delete dBbuf;*/

//	reset();
	fclose(IN_SUM);
	fclose(IN_IDX);
	
	return true;

}



//*************************************************************
// Set the posistion where to start
// For the purpose to jump the video
//*************************************************************
void FrameReader::setPos(int iPos)
{

	int fileIndex, fileOffset;
	iFrameCount = iPos;
	fileIndex = iPos/1280;
	iFileCurrentIndex = fileIndex;
	getFileView();
	fileOffset = iPos%1280;
	int bytes=(Width*Height*3);
	pFileCurrent +=fileOffset* bytes;
	//double offset=0;
	//double bytes=0;
	//
	//offset = iPos * bytes; //R,G,B per frame
	//ifs.seekg(offset, ios::beg);
	//_fseeki64(IN_FILE,offset,0);
}

//*************************************************************
// Name:
// Des: Reset the video stream
//*************************************************************
void FrameReader::reset()
{
	//ifs.seekg(0, ios::beg);
	//fseek(IN_FILE,0,0);

	iFileCurrentIndex = 0;
	 getFileView();
}


//*************************************************************
// Name:
// Des: Read get current play percent
//*************************************************************
double FrameReader::getPercent()
{
	double percent;
	percent = (double)Width*Height*3*iFrameCount/qwFileSize;
	
	return percent;
}
//*************************************************************
// Name:
// Des: Load a rgb file
// ONLY for test!!!
//*************************************************************
void FrameReader::load(){
	//ifs.open(ImagePath,std::ios::in|std::ios::binary);
	//IN_FILE = fopen(ImagePath, "rb");

}

//*************************************************************
// Name:
// Des: Close the file stream
//*************************************************************
void FrameReader::abort()
{
	//ifs.close();
	//fclose(IN_FILE);
}