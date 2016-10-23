#include "AABB.h"
#include "Polygon.h"

bool AABB::isPotentiallyCollidingX(AABB otherBox)
{
	return !(MaxX - otherBox.MinX <= 0 || otherBox.MaxX - MinX <= 0);
}

bool AABB::isPotentiallyCollidingXY(AABB otherBox)
{
	return !(MaxX - otherBox.MinX <= 0 || otherBox.MaxX - MinX <= 0) &&
		   !(MaxY - otherBox.MinY <= 0 || otherBox.MaxY - MinY <= 0);
}