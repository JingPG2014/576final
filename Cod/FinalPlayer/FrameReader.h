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
	double		Width;
	double		Height;
	char	ImagePath[_MAX_PATH];
	char*	Data;
	ifstream ifs;

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
	char*	getImagePath() { return ImagePath; }
	void	ReadOneFrame();
	void    setPos(int iPos);
	void    reset();
	void    load();
	void    abort();

};
