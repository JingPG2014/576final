#include "stdafx.h"
#include "FrameReader.h"


FrameReader::FrameReader(char* vedioPath)
{
	ifs.open(vedioPath,std::ios::in|std::ios::binary);

}

FrameReader::~FrameReader() 
{
	if(ifs)
		ifs.close();
	if(Data) 
		delete Data; 
};


void FrameReader::ReadOneFrame()
{

	//FILE *IN_FILE;
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	//IN_FILE = fopen(ImagePath, "rb");
	
	if(ifs ==NULL)
	{
		fprintf(stderr, "Error");
		exit(0);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		//Rbuf[i] = fgetc(IN_FILE);
		Rbuf[i] = ifs.get();
	}
	for (i = 0; i < Width*Height; i ++)
	{
		//Gbuf[i] = fgetc(IN_FILE);
		Gbuf[i] = ifs.get();
	}
	for (i = 0; i < Width*Height; i ++)
	{
		//Bbuf[i] = fgetc(IN_FILE);
		Bbuf[i] = ifs.get();
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
	

}


void FrameReader::setPos(int iPos)
{
	double offset=0;
	double bits=0;
	//double a = 304128;
	bits=(Width*Height*3);
	offset = iPos * bits; //R,G,B per frame
	ifs.seekg(offset, ios::beg);
}

void FrameReader::reset()
{
	ifs.seekg(0, ios::beg);
}

void FrameReader::load(){
	ifs.open(ImagePath,std::ios::in|std::ios::binary);

}

void FrameReader::abort()
{
	ifs.close();
}