//
// Created by canderson on 5/19/16.
//

#ifndef VERA_LASER_H
#define VERA_LASER_H

//
enum eFilterTypes
{
    VeraFilterLaplace,
    VeraFilterSobel,


    VeraTotalFilters
};

// Laser

bool laserBeam(boost::filesystem::path _path, eFilterTypes fType);
void laserMelt(char* data, eFilterTypes fType);

#endif //VERA_LASER_H
