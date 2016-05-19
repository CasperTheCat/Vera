//
// Created by zsmyn on 19-May-16.
//

#ifndef VERA_TYPES_H
#define VERA_TYPES_H

struct float3
{
	float x, y, z;
	float3() :x(0), y(0), z(0) {}
	float3(float x, float y, float z) : x(x), y(y), z(z) {}

	friend float3 operator+ (const float3 &f1, const float &f2)
	{
		return float3(f1.x + f2, f1.y + f2, f1.z + f2);
	}

	friend float3 operator+ (const float3 &f1, const float3 &f2)
	{
		return float3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z);
	}

	friend float3 operator* (const float3 &f1, const float &f2)
	{
		return float3(f1.x * f2, f1.y * f2, f1.z * f2);
	}

	friend float3 operator* (const float3 &f1, const float3 &f2)
	{
		return float3(f1.x * f2.x, f1.y * f2.y, f1.z * f2.z);
	}
};

inline float3 make_float3(float x, float y, float z)
{
	float3 out;
	out.x = x;
	out.y = y;
	out.z = z;
	return out;
}


inline float fdot(float3 fx, float3 fy)
{
	return fmaf(fx.x, fy.x, fmaf(fx.y, fy.y, fx.z * fy.z));
}

inline float ffMax(float x)
{
	return (x - 1) * (x < 1.f) + 1;
	//return std::min(x, 1.f);
}

inline float ffMin(float in)
{
	return in * (in > 0.f);
	//return std::max(in, 0.f);
}

inline float ffClamp(float x)
{
	return ffMin(ffMax(x));
}

inline float PTF(unsigned char in)
{
	return in / 255.f;
}

inline unsigned char FTP(float x)
{
	return ffClamp(x) * 255;
}

inline float dot(float3 iX, float3 iY)
{
	return (iX.x * iY.x) + (iX.y * iY.y) + (iX.z * iY.z);
}

inline float3 lerp(float3 fX, float3 fY, float fZ)
{
	return fX*(1 - fZ) + fY * fZ;
}

inline int Modulus(int a, int b)
{
	//int ret = a % b;
	//if (ret < 0)
	//	ret += b;
	//return ret;
	return (a % b + b) % b;
}

#endif //VERA_TYPES_H
