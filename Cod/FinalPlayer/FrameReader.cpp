#include "stdafx.h"
#include "FrameReader.h"


FrameReader::FrameReader(char* vedioPath)
{
	//ifs.open(vedioPath,std::ios::in|std::ios::binary);
	IN_FILE = fopen(vedioPath, "rb");

}

FrameReader::~FrameReader() 
{
	if(IN_FILE)
		//ifs.close();
		abort();
	if(Data) 
		delete Data; 
};


//*************************************************************
// Name:
// Des: Read one frame from rgb file
//*************************************************************
bool FrameReader::ReadOneFrame()
{

	if(feof(IN_FILE))
		return false;
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
		Rbuf[i] = fgetc(IN_FILE);
		//Rbuf[i] = ifs.get();
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
		//Gbuf[i] = ifs.get();
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
		//Bbuf[i] = ifs.get();
	}


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
	
	return true;

}



//*************************************************************
// Set the posistion where to start
// For the purpose to jump the video
//*************************************************************
void FrameReader::setPos(int iPos)
{
	double offset=0;
	double bits=0;
	//double a = 304128;
	bits=(Width*Height*3);
	offset = iPos * bits; //R,G,B per frame
	//ifs.seekg(offset, ios::beg);
	fseek(IN_FILE,offset,0);
}

//*************************************************************
// Name:
// Des: Reset the video stream
//*************************************************************
void FrameReader::reset()
{
	//ifs.seekg(0, ios::beg);
	fseek(IN_FILE,0,0);
}

//*************************************************************
// Name:
// Des: Load a rgb file
// ONLY for test!!!
//*************************************************************
void FrameReader::load(){
	//ifs.open(ImagePath,std::ios::in|std::ios::binary);
	IN_FILE = fopen(ImagePath, "rb");

}

//*************************************************************
// Name:
// Des: Close the file stream
//*************************************************************
void FrameReader::abort()
{
	//ifs.close();
	fclose(IN_FILE);
}