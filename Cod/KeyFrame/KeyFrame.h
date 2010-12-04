#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#define BLOCK_SIZE 16
#define SEARCH_WINDOW 9

//Class of KeyFrame 
class KeyFrame
{

private:
	int		        NumofFrame;
	char	        SummaryPath[_MAX_PATH];
	char	        VideoPath[_MAX_PATH];
	int*			SummaryData;
	int				Width;
	int				Height;

public:
	KeyFrame() {};
	~KeyFrame() { if(SummaryData) delete SummaryData; };

	void	        setNumofFrame(int n)  { NumofFrame = n; }; 
	void	        setSummaryPath(char *path) { strcpy(SummaryPath, path); }
	void	        setVideoPath(char *vpath) { strcpy(VideoPath, vpath); }
	void	        setSummary(int* sum) { SummaryData = sum; };
	void	        setWidth(int w)  { Width = w; }; 
	void	        setHeight(int h)  { Height = h; }; 

	int		        getNumofFrame() { return NumofFrame; };
	char*	        getSummaryPath() { return SummaryPath; }
	char*	        getVideoPath() { return VideoPath; }
	int*			getSummary() { return SummaryData; };
	int		        getWidth()  { return Width; }; 
	int		        getHeight()  { return Height; }; 

	void	        Summary();
	void			ReadSummary();
	void			WriteSummary();
	void	        EnergyComputation(unsigned char* t1, unsigned char* t2, unsigned char* t3, int frmnum, int* sd, double* energy, int numfrm);
	void			Sorting(int* sd, int numfrm);
};