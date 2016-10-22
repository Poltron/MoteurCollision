#include "AABB.h"
#include "Polygon.h"

void AABB::PolygonToAABB(CPolygon* polygon)
{
	for (auto&& point : polygon->points)
	{
		if (point.x > this->MaxX)
			this->MaxX = point.x;
		if (point.x < this->MinX)
			this->MinX = point.x;

		if (point.y > this->MaxY)
			this->MaxY = point.y;
		if (point.y < this->MinY)
			this->MinY = point.y;
	}
}

bool AABB::Intersect(AABB& OtherAABB)
{
	return	this->Intersect(OtherAABB.MinX, OtherAABB.MinY) ||
			this->Intersect(OtherAABB.MinX, OtherAABB.MaxY) ||
			this->Intersect(OtherAABB.MaxX, OtherAABB.MinY) ||
			this->Intersect(OtherAABB.MaxX, OtherAABB.MaxY);
}

bool AABB::IntersectX(AABB& OtherAABB)
{
	return	this->IntersectPointX(OtherAABB.MinX) ||
			this->IntersectPointX(OtherAABB.MaxX);
}

bool AABB::IntersectY(AABB& OtherAABB)
{
	return	this->IntersectPointY(OtherAABB.MinY) ||
			this->IntersectPointY(OtherAABB.MaxY);
}

bool AABB::Intersect(float posX, float posY)
{
	return	this->IntersectPointX(posX) &&
			this->IntersectPointY(posX);
}

bool AABB::IntersectPointX(float posX)
{
	return	posX >= this->MinX &&
			posX <= this->MaxX;
}

bool AABB::IntersectPointY(float posY)
{
	return	posY >= this->MinY &&
			posY <= this->MaxY;
}