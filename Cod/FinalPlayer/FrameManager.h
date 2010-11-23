#include "stdafx.h"
#include "FrameReader.h"

typedef
struct LoadParam
{
	FrameReader* reader; 
	char** chBuffer;
	int start;
	int end;
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
	int iBufferSize;
	int iRate;// 30 fra/sec
	int iCurrentPos;
	int iLoadingPos;// when iCurrentPos come to this position, begin laoding
	int iFrameCount;// current playing frame count
	int iRangA;// start form 0, included
	int iRangB;// Not included
	int iStart;
	FrameReader* reader;
	bool ready;;
    

	// Public Methods
public:
	FrameManager();
	FrameManager(FrameReader* reader);
	~FrameManager();

	bool setWidth(int iWidth);
	bool setHeight(int iheight);
	bool setLoadingPos(int iPos);
	bool setBufferSize(int iSec);
	bool setFrameRate(int iRate);
	int  getWidth(){ return iWidth;}
	int  getHeight(){ return iHeight;}
	bool fillBuffer();
	bool reFillBuffer();
	char* renderOneFrame();
	bool pause();
	bool stop();
	bool jump(int iPos);
	bool play(HWND hWnd);
	bool isReady(){ return ready;}
	static UINT _loadBuffer(LPVOID lpParam);
	bool loadBuffer(int start, int end);


};