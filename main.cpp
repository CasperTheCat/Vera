#include <iostream>
#include <string.h>
//#include <opencv2/video.hpp>
//#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/timer.hpp>
#include "laser.h"

using namespace std;

/*
 * Print Help
 */
void printHelp(const char *name)
{
    cout << "Help!" << endl;
    cout << name << " inputVideo" << endl;
    cout << "Args:" << endl;
    cout << "\t-s\tStitch" << endl;
    cout << "\t-f [int]\tFrame Stride" << endl;
}

int main(int argc, char **argv)
{
    boost::timer tTimer = boost::timer();
    // Parse arguments
    if (argc < 2)
    {
        printHelp(argv[0]);
        return 2;
    }

    // Declare variables out of scope
    eFilterTypes filterType = VeraFilterLaplace;
    boost::filesystem::path pFile(argv[argc - 1]); // Last file!
    if (!exists(pFile)) return 130;
	bool bColourise = false;


    // Parse any args
    for (int i = 1; i < argc; i++)
    {
		if (!strcmp(argv[i], "-c"))
		{
			bColourise = true;
		}
        if (!strcmp(argv[i], "-s"))
        {
            filterType = VeraFilterSobel;
        }
		if (!strcmp(argv[i], "-wl"))
		{
			filterType = VeraFilterWideLaplace;
		}
		if (!strcmp(argv[i], "-l"))
		{
			filterType = VeraFilterLaplace;
		}
		if (!strcmp(argv[i], "-all"))
		{
			filterType = VeraFilterAll;
		}
	}

    // Dump Data
	cout << (laserBeam(pFile, filterType,bColourise) ? "High" : "Low") << endl;


    return 0;
}