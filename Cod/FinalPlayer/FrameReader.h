/* This is a class that handle the I/O processing from the rgb file
*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "resource.h"
#include <stdio.h>
#include <fstream>
using namespace std;

class FrameReader 
{

private:
	int		Width;
	int		Height;
	char	ImagePath[_MAX_PATH];
	char*	Data;// One frame data
	char*   SumData;
	ifstream ifs;
	FILE *IN_FILE;

public:
	FrameReader(){};
	FrameReader(char * vedioPath);
	~FrameReader();
	void	setWidth(int w)  { Width = w; }; 
	void	setHeight(int h) { Height = h; }; 
	void	setImageData(char *img ) { Data = img; };
	void	setImagePath(char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getFrameData() { return Data; };
	char*   getSummaryData(){ return SumData;}
	char*	getImagePath() { return ImagePath; }
	bool	ReadOneFrame();
	bool    ReadSummary(double index, int ratio);
	void    setPos(int iPos);
	void    reset();
	void    load();
	void    abort();

};
