#include "KeyFrame.h"

int main(int argc, char *argv[])
{

	//Check for proper syntax
	if(argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input1_image.txt input2_image.rgb" << endl;
		return 0;
	}

	KeyFrame K;
	K.setSummaryPath(argv[1]);
	K.setVideoPath(argv[2]);
	K.setWidth(352);
	K.setHeight(288);
	//K.ReadSummary();
	//K.setSummaryPath(argv[3]);
	K.setNumofFrame(20);
	K.Summary();
    K.WriteSummary();

	return 0;
}

