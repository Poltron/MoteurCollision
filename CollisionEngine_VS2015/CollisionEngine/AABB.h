#ifndef _AABB_H_
#define _AABB_H_

#include <cfloat>

class CPolygon;

class AABB
{
public:
	float MinX;
	float MinY;
	float MaxX;
	float MaxY;

	AABB() :
		MinX(FLT_MAX), MinY(FLT_MAX), MaxX(FLT_MIN), MaxY(FLT_MIN)
	{};

	AABB(float minX, float minY, float maxX, float maxY) :
		 MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY)
	{};

	bool isPotentiallyCollidingX(AABB);
	bool isPotentiallyCollidingXY(AABB);
};


#endif