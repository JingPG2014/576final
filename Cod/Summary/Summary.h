#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#define ROW 1
#define COLUMN 2

using namespace std;

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
	unsigned char*	SummaryImage;
	int*			SummaryIndex;
	int				SIWidth;
	int				SIHeight;
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
	void	        setSummaryData(Frame** sum) { SummaryData = sum; };
	void	        setSummaryImage(unsigned char* img) { SummaryImage = img; };
	void	        setSummaryIndex(int* idx) { SummaryIndex = idx; };
	void	        setWidth(int w)  { Width = w; }; 
	void	        setHeight(int h)  { Height = h; }; 
	void	        setSIWidth(int siw)  { SIWidth = siw; }; 
	void	        setSIHeight(int sih)  { SIHeight = sih; }; 
	void	        setNewWidth(int nw)  { NewWidth = nw; }; 
	void	        setNewHeight(int nh)  { NewHeight = nh; }; 
	void	        setFramePath(char *frmpath) { strcpy(FramePath, frmpath); }

	int		        getNumofFrame() { return NumofFrame; };
	char*	        getSummaryPath() { return SummaryPath; }
	Frame**			getSummaryData() { return SummaryData; };
	unsigned char*	getSummaryImage() { return SummaryImage; };
	int*			getSummaryIndex() { return SummaryIndex; };
	int		        getWidth()  { return Width; }; 
	int		        getHeight()  { return Height; }; 
	int		        getSIWidth()  { return SIWidth; }; 
	int		        getSIHeight()  { return SIHeight; }; 
	int		        getNewWidth()  { return NewWidth; }; 
	int		        getNewHeight()  { return NewHeight; }; 
	char*	        getFramePath() { return FramePath; }

	void	        ReadSummaryData();
	void	        SaveSummaryImage();
	void	        SaveSummaryIndex();
	void	        CreateSummaryImage();

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