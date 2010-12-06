/* This is a class that handle the I/O processing from the rgb file
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"
#include "CS576SoundUtil.h"
#include <stdio.h>
#include <fstream>
using namespace std;

class FrameReader 
{

private:
	int		Width;
	int		Height;
	int     SumWidth;
	int     SumHeight;
	char	ImagePath[_MAX_PATH];
	char	SummaryPath[_MAX_PATH];
	char	IndexPath[_MAX_PATH];
	char*	Data;// One frame data
	char*   SumData;
	int*    IdxData;
	char**  pFileBeginA;
	char*   pFileBegin;
	char*   pFileCurrent;
	int     iFileCurrentIndex;// index of file view
	int     iFileCount;
	int     iFrameCount;
	ifstream ifs;
	//FILE *IN_FILE;
	HANDLE hFileMap;
	DWORD dwBlockBytes;
	 __int64 qwFileSize;
	 __int64 qwFileOffset;
	 CSound*  g_pSound;

public:
	FrameReader(){};
	FrameReader(char * vedioPath);
	~FrameReader();
	void	setWidth(int w)  { Width = w; }; 
	void	setHeight(int h) { Height = h; }; 
	void	setSumWidth(int w)  { SumWidth = w; }; 
	void	setSumHeight(int h) { SumHeight = h; }; 
	void	setImageData(char *img ) { Data = img; };
	void	setImagePath(char *path) { strcpy(ImagePath, path); }
	void    setSound(CSound* g_pSound){this->g_pSound = g_pSound;}
	void    setSummaryPath(char* sumPath){ strcpy(SummaryPath, sumPath);}
	void    setIndexPath(char* idxPath){strcpy(IndexPath, idxPath);}
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	int		getSumWidth() { return SumWidth; };
	int		getSumHeight() { return SumHeight; };
	char*	getFrameData() { return Data; };
	char*   getSummaryData(){ return SumData;}
	char*	getImagePath() { return ImagePath; }
	int     getIdxData(int x, int y){return IdxData[x+y*SumWidth];}
	bool	ReadOneFrame();
	bool    ReadSummary();
	void    setPos(int iPos);
	void    reset();
	void    load();
	void    abort();
	bool    beginFileMapping(char* videoPath);
	bool    endFileMapping();
	bool    getFileView();
	__int64 getFileSize(){ return qwFileSize;}

	double  getPercent();
};
