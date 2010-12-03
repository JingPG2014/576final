#include "Summary.h"

void Summary::ReadSummary()
{
	FILE *IN_FILE;
	char Num[10];
	char c;
	int n;
	long Number;

	IN_FILE = fopen(SummaryPath, "rb");

	//read NumofFrame
	//read first char
	n = 0;
	c = fgetc(IN_FILE);
	Num[n] = c;
	n++;
	//read chars until meeting SPACE
	while(c != ' ')
	{
		c = fgetc(IN_FILE);
		Num[n] = c;
		n++;
	}
	//get frame number
	Number = atoi(Num);
	//set NumofFrame
	NumofFrame = Number;
	//allocate
	SummaryData = new Frame* [NumofFrame];

	for(int i = 0; i < NumofFrame; i++){
		SummaryData[i] = new Frame[1];
	}
	//read frame number
	for(int i = 0; i < NumofFrame; i++){
		//read first char
		n = 0;
		c = fgetc(IN_FILE);
		Num[0] = c;
		n++;
		//read chars until meeting SPACE
		while(c != ' ')
		{
			c = fgetc(IN_FILE);
			Num[n] = c;
			n++;
		}
		//get frame number
		Number = atoi(Num);
		//set frame number
		SummaryData[i]->FrameNum = Number;
		//if reach the end of file, break out
		//if(feof(IN_FILE)) break;
		printf("%d ", Number);
	}


	//read frame data
	for(int i = 0; i < NumofFrame; i++){
		SummaryData[i]->ReadFrame(this);
	}

	//set frame path
	//strcpy(this->FramePath, this->SummaryPath);

	fclose(IN_FILE);
}

void Frame::ReadFrame(Summary* MySum)
{

	FILE *IN_FILE;

	int WidthSize = MySum->getWidth();
	int HeightSize = MySum->getHeight();
	int NewWidthSize = MySum->getNewWidth();
	int NewHeightSize = MySum->getNewHeight();

	unsigned char *Rbuf = new unsigned char[HeightSize*WidthSize]; 
	unsigned char *Gbuf = new unsigned char[HeightSize*WidthSize]; 
	unsigned char *Bbuf = new unsigned char[HeightSize*WidthSize];

	FrameData = new unsigned char[NewHeightSize*NewWidthSize*3];

	IN_FILE = fopen(MySum->FramePath, "rb");

	if (IN_FILE == NULL) 
	{
		fprintf(stderr, "Error: can not open file");
		exit(0);
	}

	fseek(IN_FILE, FrameNum*HeightSize*WidthSize*sizeof(unsigned char), 0);
		
	for (int i = 0; i < WidthSize*HeightSize; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (int i = 0; i < WidthSize*HeightSize; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (int i = 0; i < WidthSize*HeightSize; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	for (int j = 0; j < NewHeightSize; j++){
		for (int i = 0; i < NewWidthSize; i++){
			FrameData[3*(i+NewWidthSize*j)]		= Bbuf[(i*(WidthSize-1)/(NewWidthSize-1))+WidthSize*(j*(HeightSize-1)/(NewHeightSize-1))];
			FrameData[3*(i+NewWidthSize*j)+1]	= Gbuf[(i*(WidthSize-1)/(NewWidthSize-1))+WidthSize*(j*(HeightSize-1)/(NewHeightSize-1))];
			FrameData[3*(i+NewWidthSize*j)+2]	= Rbuf[(i*(WidthSize-1)/(NewWidthSize-1))+WidthSize*(j*(HeightSize-1)/(NewHeightSize-1))];
		}
	}

	delete [] Rbuf;
	delete [] Gbuf;
	delete [] Bbuf;

	fclose(IN_FILE);
	
}