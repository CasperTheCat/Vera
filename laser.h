//
// Created by canderson on 5/19/16.
//

#ifndef VERA_LASER_H
#define VERA_LASER_H

#include <boost/filesystem/path.hpp>
// Get compiler
#ifdef _WIN32
#ifdef __ICL
#ifndef __ICC
#define __ICC // Already defined in linux and max
#endif
#endif
#endif

#include "types.h"

//
enum eFilterTypes
{
	VeraFilterLaplace,
	VeraFilterWideLaplace,
	VeraFilterSobel,

	VeraFilterAll,

	VeraTotalFilters
};

// Laser

bool laserBeam(boost::filesystem::path _path, eFilterTypes fType, bool bColor);
void laserMelt(unsigned char* data, uint8_t fType, uint32_t sX, uint32_t sY, bool bColor);
bool laserSew(boost::filesystem::path _path, boost::filesystem::path _mask);

#endif //VERA_LASER_H
