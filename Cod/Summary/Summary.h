#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

//Class of Summary 
class Frame;

class Summary 
{
	friend class Frame;

private:
	//summary data
	int		        NumofFrame;
	char	        SummaryPath[_MAX_PATH];
	Frame**			SummaryData;
	//frame data
	int				Width;
	int				Height;
	int				NewWidth;
	int				NewHeight;
	char	        FramePath[_MAX_PATH];

public:
	Summary() {};
	~Summary() { if(SummaryData) delete SummaryData; };

	void	        setNumofFrame(int n)  { NumofFrame = n; }; 
	void	        setSummaryPath(char *path) { strcpy(SummaryPath, path); }
	void	        setSummary(Frame** sum) { SummaryData = sum; };
	void	        setWidth(int w)  { Width = w; }; 
	void	        setHeight(int h)  { Height = h; }; 
	void	        setNewWidth(int nw)  { NewWidth = nw; }; 
	void	        setNewHeight(int nh)  { NewHeight = nh; }; 
	void	        setFramePath(char *frmpath) { strcpy(FramePath, frmpath); }

	int		        getNumofFrame() { return NumofFrame; };
	char*	        getSummaryPath() { return SummaryPath; }
	Frame**			getSummary() { return SummaryData; };
	int		        getWidth()  { return Width; }; 
	int		        getHeight()  { return Height; }; 
	int		        getNewWidth()  { return NewWidth; }; 
	int		        getNewHeight()  { return NewHeight; }; 
	char*	        getFramePath() { return FramePath; }

	void	        ReadSummary();

};


//Class of Frame
class Frame
{

private:
	int		        FrameNum;
	unsigned char*	FrameData;

public:
	Frame()  {};
	~Frame() {if(FrameData) delete FrameData;};
	void	        setFrameNum(int n)  { FrameNum = n; };
	int		        getFrameNum() { return FrameNum; };
	void	        setFrameData(unsigned char*	frm)  { FrameData = frm; };
	unsigned char*  getFrameData() { return FrameData; };
	void	        ReadFrame(Summary* MySum);

	friend class Summary;

};