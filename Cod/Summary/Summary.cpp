#include "Summary.h"

void Summary::ReadSummaryData()
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

void Summary::SaveSummaryImage()
{
	FILE* IN_FILE;
	string path = "E:\\SummaryImageGollum1.rgb";
	const char *filename;

	filename = path.c_str();
	IN_FILE = fopen(filename, "wb");

	fwrite(SummaryImage, sizeof(unsigned char), 3*SIWidth*SIHeight, IN_FILE);

	/*for(int i = 0; i < NumofFrame; i++){
		fwrite(SummaryData[i]->FrameData, sizeof(unsigned char), 3*NewWidth*NewHeight, IN_FILE);
	}*/

	fclose(IN_FILE);

}

void Summary::SaveSummaryIndex()
{
	FILE* IN_FILE;
	string path = "E:\\SummaryIndexGollum1.txt";
	const char *filename;

	filename = path.c_str();
	IN_FILE = fopen(filename, "wb");

	fwrite(SummaryIndex, sizeof(int), SIWidth*SIHeight, IN_FILE);

	/*for(int i = 0; i < NumofFrame; i++){
		fwrite(SummaryData[i]->FrameData, sizeof(unsigned char), 3*NewWidth*NewHeight, IN_FILE);
	}*/

	fclose(IN_FILE);

}

void Summary::CreateSummaryImage()
{
	void SeamCarving(unsigned char* p_Imagedata, unsigned char* p_TempImagedata, int* tm, int* t, int p_WidthSize, int p_HeightSize, int p_Width, int p_Height);

	unsigned char* tmpmem;
	unsigned char* tmp;
	int* tm;
	int* t;
	int Row = 2;
	int NumEachRow;

	if(NumofFrame % Row == 0) NumEachRow = NumofFrame / Row;
	else NumEachRow = NumofFrame / Row + 1;

	int tWidth = NewWidth*NumEachRow+NewWidth/2;
	int tHeight = Row*NewHeight;

	tmpmem = new unsigned char[3*tWidth*tHeight];
	tm = new int[tWidth*tHeight];

	for(int k = 0; k < 3*tWidth*tHeight; k++){
		tmpmem[k] = 0;
	}
	for(int k = 0; k < tWidth*tHeight; k++){
		tm[k] = 0;
	}

	for(int n = 0; n < Row; n++){
		if(n % 2 == 0)
		{
			for(int m = 0; m < NumEachRow; m++){
				for(int j = 0; j < NewHeight; j++){
					for(int i = 0; i < NewWidth; i++){
						tmpmem[3*((m*NewWidth+i)+tWidth*(n*NewHeight+j))] = SummaryData[m+n*NumEachRow]->FrameData[3*(i+NewWidth*j)];
						tmpmem[3*((m*NewWidth+i)+tWidth*(n*NewHeight+j))+1] = SummaryData[m+n*NumEachRow]->FrameData[3*(i+NewWidth*j)+1];
						tmpmem[3*((m*NewWidth+i)+tWidth*(n*NewHeight+j))+2] = SummaryData[m+n*NumEachRow]->FrameData[3*(i+NewWidth*j)+2];
						tm[(m*NewWidth+i)+tWidth*(n*NewHeight+j)] = SummaryData[m+n*NumEachRow]->FrameNum;
					}
				}
			}
		}
		else
		{
			for(int m = 0; m < NumEachRow; m++){
				for(int j = 0; j < NewHeight; j++){
					for(int i = 0; i < NewWidth; i++){
						tmpmem[3*((NewWidth/2+m*NewWidth+i)+tWidth*(n*NewHeight+j))] = SummaryData[m+n*NumEachRow]->FrameData[3*(i+NewWidth*j)];
						tmpmem[3*((NewWidth/2+m*NewWidth+i)+tWidth*(n*NewHeight+j))+1] = SummaryData[m+n*NumEachRow]->FrameData[3*(i+NewWidth*j)+1];
						tmpmem[3*((NewWidth/2+m*NewWidth+i)+tWidth*(n*NewHeight+j))+2] = SummaryData[m+n*NumEachRow]->FrameData[3*(i+NewWidth*j)+2];
						tm[(NewWidth/2+m*NewWidth+i)+tWidth*(n*NewHeight+j)] = SummaryData[m+n*NumEachRow]->FrameNum;
					}
				}
			}
		}
	}

	tmp = new unsigned char[3*SIWidth*SIHeight];
	t = new int[SIWidth*SIHeight];

	SeamCarving(tmpmem, tmp, tm, t, tWidth, tHeight, SIWidth, SIHeight);

	SummaryImage = new unsigned char[3*SIWidth*SIHeight];
	SummaryIndex = new int[SIWidth*SIHeight];

	for(int k = 0; k < SIWidth*SIHeight; k++){
		SummaryImage[3*k] = tmp[3*k]; 
		SummaryImage[3*k+1] = tmp[3*k+1]; 
		SummaryImage[3*k+2] = tmp[3*k+2]; 
		SummaryIndex[k] = t[k]; 
	}

	delete[]tmpmem;
	delete[]tmp;
	delete[]tm;
	delete[]t;

}

void Frame::ReadFrame(Summary* MySum)
{
	void cropping(unsigned char* p_Imagedata, unsigned char* p_Temp4PAR, int p_BytesPerPixel, int p_WidthSize, int p_HeightSize, int p_WidthPAR, int p_HeightPAR);

	FILE *IN_FILE;

	int WidthSize = MySum->getWidth();
	int HeightSize = MySum->getHeight();
	int NewWidthSize = MySum->getNewWidth();
	int NewHeightSize = MySum->getNewHeight();

	unsigned char *Rbuf = new unsigned char[HeightSize*WidthSize]; 
	unsigned char *Gbuf = new unsigned char[HeightSize*WidthSize]; 
	unsigned char *Bbuf = new unsigned char[HeightSize*WidthSize];

	unsigned char *Buff = new unsigned char[3*HeightSize*WidthSize];
	FrameData = new unsigned char[NewHeightSize*NewWidthSize*3];

	IN_FILE = fopen(MySum->FramePath, "rb");

	if (IN_FILE == NULL) 
	{
		fprintf(stderr, "Error: can not open file");
		exit(0);
	}

	//fseek(IN_FILE, FrameNum*HeightSize*WidthSize*sizeof(unsigned char), SEEK_SET);
	_fseeki64(IN_FILE, 3*FrameNum*HeightSize*WidthSize*sizeof(unsigned char), SEEK_SET);
		
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
	
	for (int j = 0; j < HeightSize; j++){
		for (int i = 0; i < WidthSize; i++){
			Buff[3*(i+WidthSize*j)]		= Bbuf[i+WidthSize*j];
			Buff[3*(i+WidthSize*j)+1]	= Gbuf[i+WidthSize*j];
			Buff[3*(i+WidthSize*j)+2]	= Rbuf[i+WidthSize*j];
		}
	}

	//PAR changing
	int WidthPAR, HeightPAR;
	
	WidthPAR = WidthSize;
	HeightPAR = 148;//198;
	
	unsigned char* Temp4PAR = new unsigned char[WidthPAR*HeightPAR*3];

	cropping(Buff, Temp4PAR, 3, WidthSize, HeightSize, WidthPAR, HeightPAR);

	for (int j = 0; j < NewHeightSize; j++){
		for (int i = 0; i < NewWidthSize; i++){
			FrameData[3*(i+NewWidthSize*j)]		= Temp4PAR[3*((i*(WidthPAR-1)/(NewWidthSize-1))+WidthPAR*(j*(HeightPAR-1)/(NewHeightSize-1)))];
			FrameData[3*(i+NewWidthSize*j)+1]	= Temp4PAR[3*((i*(WidthPAR-1)/(NewWidthSize-1))+WidthPAR*(j*(HeightPAR-1)/(NewHeightSize-1)))+1];
			FrameData[3*(i+NewWidthSize*j)+2]	= Temp4PAR[3*((i*(WidthPAR-1)/(NewWidthSize-1))+WidthPAR*(j*(HeightPAR-1)/(NewHeightSize-1)))+2];
		}
	}

	delete [] Rbuf;
	delete [] Gbuf;
	delete [] Bbuf;
	delete [] Buff;
	delete [] Temp4PAR;

	fclose(IN_FILE);
	
}

void
cropping(unsigned char* p_Imagedata, 
		 unsigned char* p_Temp4PAR, 
		 int p_BytesPerPixel,
		 int p_WidthSize, 
		 int p_HeightSize, 
		 int p_WidthPAR, 
		 int p_HeightPAR)
{
	int k,widthdiff,heightdiff;

	widthdiff = (p_WidthSize - p_WidthPAR) / 2;
	heightdiff = (p_HeightSize - p_HeightPAR) / 2;
	
	if(p_WidthSize == p_WidthPAR && p_HeightSize == p_HeightPAR)
	{
		for (int j = 0; j < p_HeightPAR; j++){
			for(int i = 0; i < p_WidthPAR; i++){
				k = 0;
				while(k < p_BytesPerPixel)
				{
					p_Temp4PAR[(i + p_WidthPAR * j) * p_BytesPerPixel + k] = p_Imagedata[(i + p_WidthSize * j) * p_BytesPerPixel + k];
					k++;
				}
			}
		}
	}
	else if(p_WidthSize == p_WidthPAR)
	{
		for (int j = 0; j < p_HeightPAR; j++){
			for(int i = 0; i < p_WidthPAR; i++){
				k = 0;
				while(k < p_BytesPerPixel)
				{
					p_Temp4PAR[(i + p_WidthPAR * j) * p_BytesPerPixel + k] = p_Imagedata[(i + p_WidthSize * (j + heightdiff)) * p_BytesPerPixel + k];
					k++;
				}
			}
		}
	}
	else if(p_HeightSize == p_HeightPAR)
	{
		for (int j = 0; j < p_HeightPAR; j++){
			for(int i = 0; i < p_WidthPAR; i++){
				k = 0;
				while(k < p_BytesPerPixel)
				{
					p_Temp4PAR[(i + p_WidthPAR * j) * p_BytesPerPixel + k] = p_Imagedata[((i + widthdiff) + p_WidthSize * j) * p_BytesPerPixel + k];
					k++;
				}
			}
		}
	}

}

void 
SeamCarving(unsigned char* p_Imagedata, 
			unsigned char* p_TempImagedata,
			int* tm,
			int* t,
			int p_WidthSize, 
			int p_HeightSize, 
			int p_Width, 
			int p_Height)
{
	int NearEnergyMin(int* p_temp, double* p_energy, int indx1, int indx2, int p_WidthSize, int p_HeightSize, int p_flag);
	void EnergyComputation(unsigned char* p_Imagedata, double* p_energy, int p_WidthSize, int p_HeightSize);

	int hd,wd,c1,c2,flag,WW,HH;

	hd = p_HeightSize - p_Height;
	wd = p_WidthSize - p_Width;
	
	int* temp;
	double* energy;

	WW = p_WidthSize;
	HH = p_HeightSize;
		
	temp = new int[p_WidthSize*p_HeightSize*5];
	energy = new double[p_WidthSize*p_HeightSize];

	for(int j = 0; j < /*10*/p_HeightSize; j++){
		for(int i = 0; i < /*10*/p_WidthSize; i++){
			temp[4 * (i + p_WidthSize * j)] = i;
			temp[4 * (i + p_WidthSize * j) + 1] = j;
			temp[4 * (i + p_WidthSize * j) + 2] = -1;
			temp[4 * (i + p_WidthSize * j) + 3] = -1;
		}
	}
	
	//energy computation
	EnergyComputation(p_Imagedata, energy, p_WidthSize, p_HeightSize);
	
	c1 = 1;
	c2 = 1;

	double tmp;
	int ii,jj;

	while(hd > 0 || wd > 0)
	{
		if(wd > 0)
		{

		//horizontal
		//mark removal pixel
		ii = 0;
		for(int j = 0; j < HH; j++){
			if(j == 0)
			{
				tmp = 100000000;

				for(int i = 0; i < WW; i++){
					if(/*temp[4 * (i + WW * j) + 2] == -1 && energy[(temp[4 * (i + WW * j)]) + WW * (temp[4 * (i + WW * j) + 1])] != -1 && */energy[(temp[4 * (i + WW * j)]) + WW * (temp[4 * (i + WW * j) + 1])] < tmp)
					{
						tmp = energy[(temp[4 * (i + WW * j)]) + WW * (temp[4 * (i + WW * j) + 1])];
						ii = i;
					}
				}
			}
			else
			{
				if(j < HH - 1) ii = NearEnergyMin(temp, energy, ii, j, WW, HH, ROW);
			}
			temp[4 * (ii + WW * j) + 2] = c1;
		}

		//change coordinates
		for(int j = 0; j < HH; j++){
			flag = -1;
			for(int i = 0; i < WW; i++){
				if(flag == -1 && temp[4 * (i + WW * j) + 2] == c1)
				{
					if(i + 1 < WW)
					{
						temp[4 * (i + WW * j)]		= temp[4 * (i + 1 + WW * j)];
					    temp[4 * (i + WW * j) + 1]	= temp[4 * (i + 1 + WW * j) + 1];
					    temp[4 * (i + WW * j) + 2]	= temp[4 * (i + 1 + WW * j) + 2];
					    temp[4 * (i + WW * j) + 3]	= temp[4 * (i + 1 + WW * j) + 3];
					}
					else temp[4 * (i + WW * j) + 2] = -1;
					flag = 1;
				}
				else if(flag == 1)
				{
					if(i + 1 < WW)
					{
						temp[4 * (i + WW * j)] = temp[4 * (i + 1 + WW * j)];
					    temp[4 * (i + WW * j) + 1] = temp[4 * (i + 1 + WW * j) + 1];
					    temp[4 * (i + WW * j) + 2] = temp[4 * (i + 1 + WW * j) + 2];
					    temp[4 * (i + WW * j) + 3] = temp[4 * (i + 1 + WW * j) + 3];
					}
					else temp[4 * (i + WW * j) + 2] = -1;
				}
			}
		}	

		wd--;
		c1++;

		}

		if(hd > 0)
		{

		//vertical
		//mark removal pixel
		jj = 0;
		for(int i = 0; i < WW; i++){
			if(i == 0)
			{
				tmp = 100000000;

				for(int j = 0; j < HH; j++){
					if(/*temp[4 * (i + WW * j) + 3] == -1 && energy[(temp[4 * (i + WW * j)]) + WW * (temp[4 * (i + WW * j) + 1])] != -1 && */energy[(temp[4 * (i + WW * j)]) + WW * (temp[4 * (i + WW * j) + 1])] < tmp)
					{
						tmp = energy[(temp[4 * (i + WW * j)]) + WW * (temp[4 * (i + WW * j) + 1])];
						jj = j;
					}
				}
			}
			else
			{
				if(i < WW - 1) jj = NearEnergyMin(temp, energy, jj, i, WW, HH, COLUMN);
			}
			temp[4 * (i + WW * jj) + 3] = c2;
		}
		
		//change coordinates	
		for(int i = 0; i < WW; i++){
			flag = -1;
			for(int j = 0; j < HH; j++){
				if(flag == -1 && temp[4 * (i + WW * j) + 3] == c2)
				{
					if(j + 1 < HH)
					{
						temp[4 * (i + WW * j)] = temp[4 * (i + WW * (j + 1))];
					    temp[4 * (i + WW * j) + 1] = temp[4 * (i + WW * (j + 1)) + 1];
					    temp[4 * (i + WW * j) + 2] = temp[4 * (i + WW * (j + 1)) + 2];
					    temp[4 * (i + WW * j) + 3] = temp[4 * (i + WW * (j + 1)) + 3];
					}
					else temp[4 * (i + WW * j) + 3] = -1;
					flag = 1;
				}
				else if(flag == 1)
				{
					if(j + 1 < HH)
					{
						temp[4 * (i + WW * j)] = temp[4 * (i + WW * (j + 1))];
					    temp[4 * (i + WW * j) + 1] = temp[4 * (i + WW * (j + 1)) + 1];
					    temp[4 * (i + WW * j) + 2] = temp[4 * (i + WW * (j + 1)) + 2];
					    temp[4 * (i + WW * j) + 3] = temp[4 * (i + WW * (j + 1)) + 3];
					}
					else  temp[4 * (i + WW * j) + 3] = -1;
				}
			}
		}

		hd--;
		c2++;

		}
	}

	//do real retargeting based on the seam carving matrix
	for(int j = 0; j < p_Height; j++){
		for(int i = 0; i < p_Width; i++){
			for(int k = 0; k < 3; k++){
				p_TempImagedata[3 * (i + p_Width * j) + k] =  /*energy[(i + p_Width * j)];*/p_Imagedata[3 * ((temp[4 * (i + WW * j)]) + p_WidthSize * (temp[4 * (i + WW * j) + 1])) + k];
			}
			t[i + p_Width * j] = tm[(temp[4 * (i + WW * j)]) + p_WidthSize * (temp[4 * (i + WW * j) + 1])];
		}
	}

	if(temp == NULL) delete[]temp;
	if(energy == NULL) delete[]energy;
}

void 
EnergyComputation(unsigned char* p_Imagedata, 
				  double* p_energy,
				  int p_WidthSize, 
				  int p_HeightSize)
{
	double* temp1 = new double[p_WidthSize*p_HeightSize];
	double* energy1 = new double[p_WidthSize*p_HeightSize];

	double dr,dc;

	//covert 24 bits color into gray-level image for discontinuity analysis
	for(int j = 0; j < p_HeightSize; j++){
		for(int i = 0; i < p_WidthSize; i++){
			temp1[i + p_WidthSize * j] = 0.299 * (double)p_Imagedata[(i + p_WidthSize * j) * 3] 
			                           + 0.587 * (double)p_Imagedata[(i + p_WidthSize * j) * 3 + 1]
			                           + 0.114 * (double)p_Imagedata[(i + p_WidthSize * j) * 3 + 2];

		}
	}

	//e1 = |dI/dx|+|dI/dY|
	for(int j = 0; j < p_HeightSize; j++){
		for(int i = 0; i < p_WidthSize; i++){
			if(i - 1 >= 0) dr = temp1[i + p_WidthSize * j] - temp1[i - 1 + p_WidthSize * j];
			else if(i - 1 < 0) dr = temp1[i + p_WidthSize * j] - temp1[i + 1 + p_WidthSize * j];
			if(j + 1 < p_HeightSize) dc = temp1[i + p_WidthSize * j] - temp1[i + p_WidthSize * (j + 1)];
			else if(j + 1 >= p_HeightSize) dc = temp1[i + p_WidthSize * j] - temp1[i + p_WidthSize * (j - 1)];
			energy1[i + p_WidthSize * j] = abs(dr) + abs(dc);
		}
	}

	//total energy computation
	for(int j = 0; j < p_HeightSize; j++){
		for(int i = 0; i < p_WidthSize; i++){
			p_energy[i + p_WidthSize * j] = energy1[i + p_WidthSize * j]; 
		}
	}
}

int 
NearEnergyMin(int* p_temp,
			  double* p_energy,
			  int indx1, 
			  int indx2, 
			  int p_WidthSize,
			  int p_HeightSize,
			  int p_flag)
{
	int tempindx1,tempindx2,energyindx1,energyindx2,energyindx3,energyindx4;
	int min = indx1;
	
	if(p_flag == ROW)
	{	
		if(indx1 - 1 >= 0) tempindx1 = indx1 - 1;
		else tempindx1 = indx1;
		if(indx1 + 1 < p_WidthSize) tempindx2 = indx1 + 1;
		else tempindx2 = indx1;
		
		energyindx1 = p_temp[4 * (tempindx1 + p_WidthSize * indx2)];
		energyindx2 = p_temp[4 * (tempindx1 + p_WidthSize * indx2) + 1];
		energyindx3 = p_temp[4 * (tempindx2 + p_WidthSize * indx2)];
		energyindx4 = p_temp[4 * (tempindx2 + p_WidthSize * indx2) + 1];

		if(p_energy[energyindx1 + p_WidthSize * energyindx2] < p_energy[energyindx3 + p_WidthSize * energyindx4]) min = tempindx1;
		else if(p_energy[energyindx1 + p_WidthSize * energyindx2] >= p_energy[energyindx3 + p_WidthSize * energyindx4]) min = tempindx2;

		energyindx1 = p_temp[4 * (min + p_WidthSize * indx2)];
		energyindx2 = p_temp[4 * (min + p_WidthSize * indx2) + 1];
		energyindx3 = p_temp[4 * (indx1 + p_WidthSize * indx2)];
		energyindx4 = p_temp[4 * (indx1 + p_WidthSize * indx2) + 1];

		if(p_energy[energyindx1 + p_WidthSize * energyindx2] >= p_energy[energyindx3 + p_WidthSize * energyindx4]) min = indx1;
	}
	else if(p_flag == COLUMN)
	{	
		if(indx1 - 1 >= 0) tempindx1 = indx1 - 1;
		else tempindx1 = indx1;
		if(indx1 + 1 < p_HeightSize) tempindx2 = indx1 + 1;
		else tempindx2 = indx1;
		
		energyindx1 = p_temp[4 * (indx2 + p_WidthSize * tempindx1)];
		energyindx2 = p_temp[4 * (indx2 + p_WidthSize * tempindx1) + 1];
		energyindx3 = p_temp[4 * (indx2 + p_WidthSize * tempindx2)];
		energyindx4 = p_temp[4 * (indx2 + p_WidthSize * tempindx2) + 1];

		if(p_energy[energyindx1 + p_WidthSize * energyindx2] < p_energy[energyindx3 + p_WidthSize * energyindx4]) min = tempindx1;
		else if(p_energy[energyindx1 + p_WidthSize * energyindx2] >= p_energy[energyindx3 + p_WidthSize * energyindx4]) min = tempindx2;
		
		energyindx1 = p_temp[4 * (indx2 + p_WidthSize * min)];
		energyindx2 = p_temp[4 * (indx2 + p_WidthSize * min) + 1];
		energyindx3 = p_temp[4 * (indx2 + p_WidthSize * indx1)];
		energyindx4 = p_temp[4 * (indx2 + p_WidthSize * indx1) + 1];

		if(p_energy[energyindx1 + p_WidthSize * energyindx2] >= p_energy[energyindx3 + p_WidthSize * energyindx4]) min = indx1;
	}

	return min;
}