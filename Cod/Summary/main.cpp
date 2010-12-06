//#include <iostream>
#include "Summary.h"

//using namespace std;

int main(int argc, char *argv[])
{

	//Check for proper syntax
	if(argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input1_image.txt input2_image.rgb" << endl;
		return 0;
	}

	Summary S;
	S.setSummaryPath(argv[1]);
	S.setFramePath(argv[2]);
	S.setWidth(352);
	S.setHeight(288);
	S.setNewWidth(150);
	S.setNewHeight(75);
	S.setSIWidth(880);
	S.setSIHeight(72);
	S.ReadSummaryData();
	S.CreateSummaryImage();
	S.SaveSummaryImage();
	S.SaveSummaryIndex();

	return 0;
}

