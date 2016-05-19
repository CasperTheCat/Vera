#include <iostream>
#include <string.h>
//#include <opencv2/video.hpp>
//#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer.hpp>
#include "rocket.h"

using namespace std;

/*
 * Print Help
 */
void printHelp(const char* name)
{
	cout << "Help!" << endl;
	cout << name << " inputVideo" << endl;
	cout << "Args:" << endl;
	cout << "\t-s\tStitch" << endl;
	cout << "\t-f [int]\tFrame Stride" << endl;
}

int main(int argc, char** argv)
{
	boost::timer tTimer = boost::timer();
	// Parse arguments
	if (argc < 2)
	{
		printHelp(argv[0]);
		return 2;
	}

	// Declare variables out of scope
	int fStride = 2;
	bool bStitch = false;
	boost::filesystem::path pFile(argv[argc - 1]); // Last file!
	if (!exists(pFile)) return 130;

	// Parse any args
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-f") && (i + 1) < argc)
		{
			fStride = atoi(argv[++i]);
		}
		else if (!strcmp(argv[i], "-s"))
		{
			bStitch = true;
		}
	}

	// Dump Data
	cout << pFile.string() << "\t" << argc << "\t" << fStride << endl;

	if (!bStitch)
	{
		// Blast
		cout << (rocketBlast(pFile, fStride) ? "High" : "Low") << endl;
	}
	else
	{
		cout << "Something bad happened!" << endl;
	}



	return 0;
}