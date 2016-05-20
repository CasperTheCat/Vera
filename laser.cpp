//
// Created by canderson on 5/19/16.
//

#define RwGB_LUMA float3{float(0.2126), float(0.7152), float(0.0722)} //RGB
#define RGB_LUMA float3{float(0.0722), float(0.7152), float(0.2126)} //BGR
#include "laser.h"
#include <iostream>
#include <opencv2/highgui.hpp>

#ifdef __ICC
#include <cilk/cilk.h>
#endif

std::string getFilterName(uint8_t t)
{
	switch (t)
	{
	case VeraFilterLaplace:
		return "Laplace";
	case VeraFilterWideLaplace:
		return "WideLaplace";
	case VeraFilterSobel:
		return "Sobel";
	default:
		return "VeraUFilter";
	}

}

bool laserBeam(boost::filesystem::path _path, eFilterTypes fType, bool bColor)
{
	// Make a video capture
	//std::cout << "chk" << _path.string() << std::endl;
	cv::VideoCapture videoInput(_path.string());
	cv::Mat frameBuffer;
	// Check if that worked
	if (!videoInput.isOpened())
	{
		frameBuffer = cv::imread(_path.string()); // Read Image
		if (frameBuffer.empty()) return false;
	}
	else
	{
		videoInput >> frameBuffer;
		if (frameBuffer.empty()) return false;
	}


	std::string frameName = _path.string().substr(0, _path.string().length() - 4);

	std::cout << frameName << std::endl;



	if (fType == VeraFilterAll)
	{
		cv::Mat writeBuffer;
		writeBuffer.create(frameBuffer.size(), frameBuffer.type());
		for (auto i = 0; i < VeraTotalFilters - 1; i++)
		{
			memcpy(writeBuffer.data, frameBuffer.data, frameBuffer.rows * frameBuffer.cols * 3);

			laserMelt(writeBuffer.data, i, frameBuffer.cols, frameBuffer.rows, bColor);

			cv::imwrite(frameName + getFilterName(i) + (bColor ? "" : "Mask") + ".png", writeBuffer);
		}
	}
	else
	{
		laserMelt(frameBuffer.data, fType, frameBuffer.cols, frameBuffer.rows, bColor);

		cv::imwrite(frameName + getFilterName(fType) + (bColor ? "" : "Mask") + ".png", frameBuffer);
	}
	return true;

}

float sobelHorz(unsigned char* in, uint64_t idx, uint32_t pX, uint32_t pY)
{
	float n1 = -1 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) - 3 - pX * 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) - 3 - pX * 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) - 3 - pX * 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float n2 = -2 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) - pX * 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) - pX * 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) - pX * 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float n3 = -1 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) + 3 - (pX * 3), pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) + 3 - (pX * 3), pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) + 3 - (pX * 3), pX * pY * 3)])  // B
	}, RGB_LUMA);

	float p1 = fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) - 3 + pX * 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) - 3 + pX * 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) - 3 + pX * 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float p2 = 2 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) + pX * 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) + pX * 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) + pX * 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float p3 = fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) + 3 + (pX * 3), pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) + 3 + (pX * 3), pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) + 3 + (pX * 3), pX * pY * 3)])  // B
	}, RGB_LUMA);

	return n1 + n2 + n3 + p1 + p2 + p3;
}


float sobelVert(unsigned char* in, uint64_t idx, uint32_t pX, uint32_t pY)
{
	float n1 = -1 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) - 3 - pX * 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) - 3 - pX * 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) - 3 - pX * 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float n2 = -2 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) - 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) - 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) - 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float n3 = -1 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) - 3 + (pX * 3), pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) - 3 + (pX * 3), pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) - 3 + (pX * 3), pX * pY * 3)])  // B
	}, RGB_LUMA);

	float p1 = fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) + 3 - pX * 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) + 3 - pX * 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) + 3 - pX * 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float p2 = 2 * fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) + 3, pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) + 3, pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) + 3, pX * pY * 3)])  // B
	}, RGB_LUMA);

	float p3 = fdot(
		float3{
		PTF(in[Modulus(((idx * 3) + 0) + 3 + (pX * 3), pX * pY * 3)]), // R
		PTF(in[Modulus(((idx * 3) + 1) + 3 + (pX * 3), pX * pY * 3)]), // G
		PTF(in[Modulus(((idx * 3) + 2) + 3 + (pX * 3), pX * pY * 3)])  // B
	}, RGB_LUMA);

	return n1 + n2 + n3 + p1 + p2 + p3;


}

float fWideLaplacian(unsigned char* cIn, float3 cLocal, uint64_t gIdx, uint32_t pX, uint32_t pY)
{
	float3 tempLap;
	// Lets do a laplace filter over the image!
	tempLap.x
		= (2 * (PTF(cIn[Modulus(((gIdx * 3) + 0) - pX * 3, pX * pY * 3)])) // UP
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 0) + pX * 3, pX * pY * 3)])) // Down
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 0) - 3, pX * pY * 3)])) // Left
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 0) + 3, pX * pY * 3)]))) // Right
		// All 2 block done

		+ PTF(cIn[Modulus(((gIdx * 3) + 0) + (2 * 3), pX * pY * 3)]) // Far Right
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) - (2 * 3), pX * pY * 3)]) // Far Left
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) - (2 * pX * 3), pX * pY * 3)]) // Far Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) + (2 * pX * 3), pX * pY * 3)]) // Far Down
		// Extremes done

		+ PTF(cIn[Modulus(((gIdx * 3) + 0) + 3 - (pX * 3), pX * pY * 3)]) // Right Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) + 3 + (pX * 3), pX * pY * 3)]) // Right Down
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) - 3 - (pX * 3), pX * pY * 3)]) // Left Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) - 3 + (pX * 3), pX * pY * 3)]) // Left Down

		- 16 * cLocal.x;

	tempLap.y
		= (2 * (PTF(cIn[Modulus(((gIdx * 3) + 1) - pX * 3, pX * pY * 3)])) // UP
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 1) + pX * 3, pX * pY * 3)])) // Down
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 1) - 3, pX * pY * 3)])) // Left
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 1) + 3, pX * pY * 3)]))) // Right
		// All 2 block done

		+ PTF(cIn[Modulus(((gIdx * 3) + 1) + (2 * 3), pX * pY * 3)]) // Far Right
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) - (2 * 3), pX * pY * 3)]) // Far Left
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) - (2 * pX * 3), pX * pY * 3)]) // Far Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) + (2 * pX * 3), pX * pY * 3)]) // Far Down
		// Extremes done

		+ PTF(cIn[Modulus(((gIdx * 3) + 1) + 3 - (pX * 3), pX * pY * 3)]) // Right Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) + 3 + (pX * 3), pX * pY * 3)]) // Right Down
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) - 3 - (pX * 3), pX * pY * 3)]) // Left Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) - 3 + (pX * 3), pX * pY * 3)]) // Left Down

		- 16 * cLocal.y;

	tempLap.z
		= (2 * (PTF(cIn[Modulus(((gIdx * 3) + 2) - pX * 3, pX * pY * 3)])) // UP
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 2) + pX * 3, pX * pY * 3)])) // Down
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 2) - 3, pX * pY * 3)])) // Left
		+ (2 * PTF(cIn[Modulus(((gIdx * 3) + 2) + 3, pX * pY * 3)]))) // Right
		// All 2 block done

		+ PTF(cIn[Modulus(((gIdx * 3) + 2) + (2 * 3), pX * pY * 3)]) // Far Right
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) - (2 * 3), pX * pY * 3)]) // Far Left
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) - (2 * pX * 3), pX * pY * 3)]) // Far Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) + (2 * pX * 3), pX * pY * 3)]) // Far Down
		// Extremes done

		+ PTF(cIn[Modulus(((gIdx * 3) + 2) + 3 - (pX * 3), pX * pY * 3)]) // Right Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) + 3 + (pX * 3), pX * pY * 3)]) // Right Down
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) - 3 - (pX * 3), pX * pY * 3)]) // Left Up
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) - 3 + (pX * 3), pX * pY * 3)]) // Left Down

		- 16 * cLocal.z;

	return fdot(tempLap, RGB_LUMA);
}

float fLaplacian(unsigned char* cIn, float3 cLocal, uint64_t gIdx, uint32_t pX, uint32_t pY)
{
	float3 tempLap;
	// Lets do a laplace filter over the image!
	tempLap.x = (PTF(cIn[Modulus(((gIdx * 3) + 0) - pX * 3, pX * pY * 3)]) // UP
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) + pX * 3, pX * pY * 3)]) // Down
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) - 3, pX * pY * 3)]) // Left
		+ PTF(cIn[Modulus(((gIdx * 3) + 0) + 3, pX * pY * 3)])) // Right
		- 4 * cLocal.x;

	tempLap.y = (PTF(cIn[Modulus(((gIdx * 3) + 1) - pX * 3, pX * pY * 3)]) // UP
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) + pX * 3, pX * pY * 3)]) // Down
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) - 3, pX * pY * 3)]) // Left
		+ PTF(cIn[Modulus(((gIdx * 3) + 1) + 3, pX * pY * 3)])) // Right
		- 4 * cLocal.y;

	tempLap.z = (PTF(cIn[Modulus(((gIdx * 3) + 2) - pX * 3, pX * pY * 3)]) // UP
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) + pX * 3, pX * pY * 3)]) // Down
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) - 3, pX * pY * 3)]) // Left
		+ PTF(cIn[Modulus(((gIdx * 3) + 2) + 3, pX * pY * 3)])) // Right
		- 4 * cLocal.z;

	return fdot(tempLap, RGB_LUMA);
}









void laserMelt(unsigned char*data, uint8_t fType, uint32_t sX, uint32_t sY, bool bColor)
{
	// Allocate second buffer for image!
	auto result = new unsigned char[sX * sY * 3];

#ifdef __ICC
	cilk_for(uint64_t i = 0; i < sX * sY; i++)
#else
#ifdef __GNUC__
#pragma omp parallel for
	for (uint64_t i = 0; i < sX * sY; i++)
#else
#pragma omp parallel for
	for (signed long long i = 0; i < sX * sY; i++)
#endif
#endif
	{
		// So we need to filter and greyscaleb
		// temporary invert!
		float3 cLocal;
		float filter = 1.f;
		cLocal.x = PTF(data[3 * i]);
		cLocal.y = PTF(data[3 * i + 1]);
		cLocal.z = PTF(data[3 * i + 2]);



		if (fType == VeraFilterSobel)
		{
			float sobelV = sobelVert(data, i, sX, sY);
			float sobelH = sobelHorz(data, i, sX, sY);
			filter = sqrt((sobelV * sobelV) + (sobelH * sobelH));
			filter *= 0.2;
		}
		else if (fType == VeraFilterWideLaplace)
		{
			filter = fWideLaplacian(data, cLocal, i, sX, sY);
			filter *= (filter >= 0.f);
			filter *= 0.1; // or 0.0625 for better accuracy
		}
		else if (fType == VeraFilterLaplace)
		{
			filter = fLaplacian(data, cLocal, i, sX, sY);
			filter *= (filter >= 0.f);
			filter *= 0.25; // or 0.0625 for better accuracy
		}


		if (!bColor)
		{
			result[i * 3 + 0] = FTP(filter);
			result[i * 3 + 1] = FTP(filter);
			result[i * 3 + 2] = FTP(filter);
		}
		else
		{
			cLocal = lerp(cLocal, make_float3(0, 0, 0), filter);

			result[i * 3 + 0] = FTP(cLocal.x);
			result[i * 3 + 1] = FTP(cLocal.y);
			result[i * 3 + 2] = FTP(cLocal.z);
		}
	}
	memcpy(data, result, sX * sY * 3);
	delete result;
}
