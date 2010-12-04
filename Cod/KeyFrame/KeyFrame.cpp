#include "KeyFrame.h"

void KeyFrame::ReadSummary()
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
	SummaryData = new int[NumofFrame];
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
		SummaryData[i] = Number;
		//if reach the end of file, break out
		if(feof(IN_FILE)) break;
		//printf("%d ", Number);
	}

	fclose(IN_FILE);
}

void KeyFrame::WriteSummary()
{
	FILE *IN_FILE;
	char Num[10];
	int n;
	long Number;

	IN_FILE = fopen(SummaryPath, "wb");

	//write NumofFrame
	Number = NumofFrame;
	itoa(Number, Num, 10);
	//write chars until meeting SPACE
	n = 0;
	while(Num[n] != '\0')
	{
		fputc(Num[n], IN_FILE);
		n++;
	}
	fputc(' ', IN_FILE);
	//read frame number
	for(int i = 0; i < NumofFrame; i++){
		//write NumofFrame
		Number = SummaryData[i];
		//printf("%d ", Number);
		itoa(Number, Num, 10);
		//write chars until meeting SPACE
		n = 0;
		while(Num[n] != '\0')
		{
			fputc(Num[n], IN_FILE);
			n++;
		}
		fputc(' ', IN_FILE);
		//printf("%d ", Number);
	}

	fclose(IN_FILE);
}

void KeyFrame::Summary()
{
	FILE *IN_FILE;
	long tmp1,tmp2;
	long frmnum = 0;
	long numfrm;
	unsigned char* temp1 = new unsigned char[Width*Height*3];
	unsigned char* temp2 = new unsigned char[Width*Height*3];
	unsigned char* temp3 = new unsigned char[Width*Height*3];
	double* energy = new double[NumofFrame];

	IN_FILE = fopen(VideoPath, "rb");

	tmp1 = ftell(IN_FILE);
	fseek(IN_FILE, 0, SEEK_END);
	tmp2 = ftell(IN_FILE);
	numfrm = (tmp2 - tmp1) / (Width*Height*3*sizeof(unsigned char));
	fseek(IN_FILE, 0, SEEK_SET);

	SummaryData = new int[NumofFrame];

	//summary
	//copy data for 3 frame
	while(ftell(IN_FILE) != tmp2)
	{
		if(frmnum == 0)
		{
			fread(temp1, sizeof(unsigned char), Width*Height*3, IN_FILE);
			memcpy(temp2, temp1, sizeof(unsigned char)*Width*Height*3);
			fread(temp3, sizeof(unsigned char), Width*Height*3, IN_FILE);
		}
		else if(frmnum == numfrm - 1)
		{
			memcpy(temp1, temp2, sizeof(unsigned char)*Width*Height*3);
			memcpy(temp2, temp3, sizeof(unsigned char)*Width*Height*3);
		}
		else
		{
			memcpy(temp1, temp2, sizeof(unsigned char)*Width*Height*3);
			memcpy(temp2, temp3, sizeof(unsigned char)*Width*Height*3);
			fread(temp3, sizeof(unsigned char), Width*Height*3, IN_FILE);
		}
		//compute energy for frame
		EnergyComputation(temp1, temp2, temp3, frmnum, SummaryData, energy, NumofFrame);
		//go to next frame
		frmnum++;
	}

	Sorting(SummaryData, NumofFrame);

	delete[]temp1;
	delete[]temp2;
	delete[]temp3;

}

void 
KeyFrame::EnergyComputation(unsigned char* t1, 
							unsigned char* t2, 
							unsigned char* t3,
							int frmnum,
							int* sd, 
							double* energy,
							int numfrm)
{
	double tmp1,tmp2,err,sum;
	int idx1,idx2,flg,sw;
	int indx1[9];
	int indx2[9];

	sum = 0;
	for(int jj = 0; jj < Height; jj += BLOCK_SIZE){
		for(int ii = 0; ii < Width; ii += BLOCK_SIZE){
			//=========================================
			//search match block in backward frame
			//=========================================
			//intial search window size and search vector
			sw = SEARCH_WINDOW;
			indx1[0] = 0;			indx2[0] = 0;
			indx1[1] = - sw / 2;	indx2[1] = 0;
			indx1[2] = - sw / 2;	indx2[2] = - sw / 2;
			indx1[3] = 0;			indx2[3] = - sw / 2;
			indx1[4] = sw / 2;		indx2[4] = - sw / 2;
			indx1[5] = sw / 2;		indx2[5] = 0;
			indx1[6] = sw / 2;		indx2[6] = sw / 2;
			indx1[7] = - sw / 2;	indx2[7] = sw / 2;
			indx1[8] = - sw / 2;	indx2[8] = sw / 2;
			//fast search
			tmp1 = 1000000;
			idx1 = ii;
			idx2 = jj;
			while(sw > 1)
			{
				for(int k = 0; k < 9; k++){
					err = 0;
					for(int j = 0; j < BLOCK_SIZE; j++){
						for(int i = 0; i < BLOCK_SIZE; i++){
							//only consider green component
							if((idx1 + i >= 0 && idx1 + i < Width && idx2 + j >= 0 && idx2 + j < Height) &&
							   (idx1 + indx1[k] + i >= 0 && idx1 + indx1[k] + i < Width && idx2 + indx2[k] + j >= 0 && idx2 + indx2[k] + j < Height))
							{
								err += abs(t2[3*((idx1+i)+Width*(idx2+j))+1] - t1[3*((idx1+indx1[k]+i)+Width*(idx2+indx2[k]+j))+1]);
							}
							else if((idx1 + i < 0 || idx1 + i >= Width || idx2 + j < 0 || idx2 + j >= Height) &&
							   (idx1 + indx1[k] + i >= 0 && idx1 + indx1[k] + i < Width && idx2 + indx2[k] + j >= 0 && idx2 + indx2[k] + j < Height))
							{
								err += t1[3*((idx1+indx1[k]+i)+Width*(idx2+indx2[k]+j))+1];
							}
							else if((idx1 + i < 0 || idx1 + i >= Width || idx2 + j < 0 || idx2 + j >= Height) &&
							        (idx1 + indx1[k] + i >= 0 && idx1 + indx1[k] + i < Width && idx2 + indx2[k] + j >= 0 && idx2 + indx2[k] + j < Height))
							{
								err += t2[3*((idx1+i)+Width*(idx2+j))+1];
							}
							else err += 0;
						}
					}
					err = err / (BLOCK_SIZE * BLOCK_SIZE);
					if(err < tmp1)
					{
						tmp1 = err;
						flg = k;
					}
				}
				//change new center
				idx1 = idx1 + indx1[flg];
				idx2 = idx2 + indx2[flg];
				//reduce search window size by 2
				sw /= 2;
				//compute new search vector
				indx1[0] = 0;			indx2[0] = 0;
				indx1[1] = - sw / 2;	indx2[1] = 0;
				indx1[2] = - sw / 2;	indx2[2] = - sw / 2;
				indx1[3] = 0;			indx2[3] = - sw / 2;
				indx1[4] = sw / 2;		indx2[4] = - sw / 2;
				indx1[5] = sw / 2;		indx2[5] = 0;
				indx1[6] = sw / 2;		indx2[6] = sw / 2;
				indx1[7] = - sw / 2;	indx2[7] = sw / 2;
				indx1[8] = - sw / 2;	indx2[8] = sw / 2;
			}
			//=========================================
			//search match block in forward frame
			//=========================================
			//intial search window size and search vector
			sw = SEARCH_WINDOW;
			indx1[0] = 0;			indx2[0] = 0;
			indx1[1] = - sw / 2;	indx2[1] = 0;
			indx1[2] = - sw / 2;	indx2[2] = - sw / 2;
			indx1[3] = 0;			indx2[3] = - sw / 2;
			indx1[4] = sw / 2;		indx2[4] = - sw / 2;
			indx1[5] = sw / 2;		indx2[5] = 0;
			indx1[6] = sw / 2;		indx2[6] = sw / 2;
			indx1[7] = - sw / 2;	indx2[7] = sw / 2;
			indx1[8] = - sw / 2;	indx2[8] = sw / 2;
			//fast search
			tmp2 = 1000000;
			idx1 = ii;
			idx2 = jj;
			while(sw >= 1)
			{
				for(int k = 0; k < 9; k++){
					err = 0;
					for(int j = 0; j < BLOCK_SIZE; j++){
						for(int i = 0; i < BLOCK_SIZE; i++){
							//only consider green component
							if((idx1 + i >= 0 && idx1 + i < Width && idx2 + j >= 0 && idx2 + j < Height) &&
							   (idx1 + indx1[k] + i >= 0 && idx1 + indx1[k] + i < Width && idx2 + indx2[k] + j >= 0 && idx2 + indx2[k] + j < Height))
							{
								err += abs(t2[3*((idx1+i)+Width*(idx2+j))+1] - t3[3*((idx1+indx1[k]+i)+Width*(idx2+indx2[k]+j))+1]);
							}
							else if((idx1 + i < 0 || idx1 + i >= Width || idx2 + j < 0 || idx2 + j >= Height) &&
							   (idx1 + indx1[k] + i >= 0 && idx1 + indx1[k] + i < Width && idx2 + indx2[k] + j >= 0 && idx2 + indx2[k] + j < Height))
							{
								err += t3[3*((idx1+indx1[k]+i)+Width*(idx2+indx2[k]+j))+1];
							}
							else if((idx1 + i < 0 || idx1 + i >= Width || idx2 + j < 0 || idx2 + j >= Height) &&
							        (idx1 + indx1[k] + i >= 0 && idx1 + indx1[k] + i < Width && idx2 + indx2[k] + j >= 0 && idx2 + indx2[k] + j < Height))
							{
								err += t2[3*((idx1+i)+Width*(idx2+j))+1];
							}
							else err += 0;
						}
					}
					err = err / (BLOCK_SIZE * BLOCK_SIZE);
					if(err < tmp2)
					{
						tmp2 = err;
						flg = k;
					}
				}
				//change new center
				idx1 = idx1 + indx1[flg];
				idx2 = idx2 + indx2[flg];
				//reduce search window size by 2
				sw /= 2;
				//compute new search vector
				indx1[0] = 0;			indx2[0] = 0;
				indx1[1] = - sw / 2;	indx2[1] = 0;
				indx1[2] = - sw / 2;	indx2[2] = - sw / 2;
				indx1[3] = 0;			indx2[3] = - sw / 2;
				indx1[4] = sw / 2;		indx2[4] = - sw / 2;
				indx1[5] = sw / 2;		indx2[5] = 0;
				indx1[6] = sw / 2;		indx2[6] = sw / 2;
				indx1[7] = - sw / 2;	indx2[7] = sw / 2;
				indx1[8] = - sw / 2;	indx2[8] = sw / 2;
			}
			//find the smallest value and add it to frame energy
			if(tmp1 < tmp2) sum += tmp1;
			else sum += tmp2;
		}
	}

	//save as key frame if this frame has smaller energy than those in summary
	for(int k = 0; k < numfrm; k++){
		if(energy[k] < sum)
		{
			energy[k] = sum;
			sd[k] = frmnum;
			break;
		}
	}

}


void 
KeyFrame::Sorting(int* sd, 
				  int numfrm)
{
	int temp = 0;
	
	for(int i = 0; i < numfrm - 1; i++){
		for(int j = i + 1; j < numfrm; j++){
			if(sd[j] < sd[i])
			{ 
				temp = sd[i]; 
				sd[i] = sd[j]; 
				sd[j] = temp;
			}
		}
	}
}
