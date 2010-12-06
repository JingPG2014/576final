#include "stdafx.h"
#include "FrameReader.h"

typedef
struct LoadParam
{
	FrameReader* reader; 
	char** chBuffer;
	int start;
	int end;
	bool* pbIsEnd;
}
loadParam;

class FrameManager
{

	// Private Members
private:
	int	iWidth;
	int	iHeight;
	char* chFrame;
	char** chBuffer;
	char** chBuffer2;
	int iBufferNum;
	int iBufferSize;
	int iRate;// 30 fra/sec
	int iCurrentPos;
	int iFirstLoadingPos;// when iCurrentPos come to this position, begin laoding
	int iSecLoadingPos;
	int iThirdLoadingPos;
	double iFrameCount;// current playing frame count
	int iRangA;// start form 0, included
	int iRangB;// Not included
	int iStart;
	FrameReader* reader;
	bool ready;;
	bool bIsEnd;
	int iSumRatio;//summary image down-sampling ratio
    

	// Public Methods
public:
	FrameManager();
	FrameManager(FrameReader* reader);
	~FrameManager();

	bool setWidth(int iWidth);
	bool setHeight(int iheight);
	bool setLoadingPos(int iPos);
	bool setBufferSize(int iSize);
	bool setBufferNum(int iBufferNum){ this->iBufferNum = iBufferNum;}
	bool setFrameRate(int iRate);
	void setSumRatio(int iRatio){ iSumRatio = iRatio;}
	bool  isEnd(){ return bIsEnd;}
	int  getWidth(){ return iWidth;}
	int  getHeight(){ return iHeight;}
	int  getFrameRate(){ return iRate;}
	int  getBufferNum(){ return iBufferNum;}
	double  getFrameCount(){ return iFrameCount;}
	//bool fillBuffer();
	bool fillBufferEx();
	bool reFillBuffer();
	char* renderOneFrame();
	bool pause();
	bool stop();
	bool jump(int iPos);
	bool play(HWND hWnd);
	bool drawSummay(HWND hWnd, int* index);
	bool isReady(){ return ready;}
	static UINT _loadBufferThread(LPVOID lpParam);
	bool _loadBuffer(int start, int end, char** chBuffer);
	bool loadBuffer(int start, int end);

};