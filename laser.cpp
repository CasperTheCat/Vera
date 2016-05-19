//
// Created by canderson on 5/19/16.
//



#include "laser.h"
#include <iostream>

bool laserBeam(boost::filesystem::path _path, eFilterTypes fType)
{
    // Make a video capture
    std::cout << "chk" << _path.string() << std::endl;
    cv::VideoCapture videoInput(_path.string());

    // Check if that worked
    if (!videoInput.isOpened()) return false;

    cv::Mat frameBuffer;
    uint64_t frameCount = 0;
    std::string frameName = _path.string().substr(0, _path.string().length() - 4);

    std::cout << frameName << std::endl;

    while (true)
    {
        videoInput >> frameBuffer;
        if (frameBuffer.empty()) break;

        std::cout << "  |  FrameCount : " << frameCount << std::endl;

        laserMelt()

        cv::imwrite(frameName + std::to_string(frameCount++) + ".png", frameBuffer);
    }
    return true;

}