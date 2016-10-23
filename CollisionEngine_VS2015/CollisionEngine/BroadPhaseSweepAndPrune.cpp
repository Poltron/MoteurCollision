#include "BroadPhaseSweepAndPrune.h"
#include "AABB.h"

#include <algorithm>
#include <functional>

void CBroadPhaseSweepAndPrune::GetCollidingPairsToCheck(
	std::vector<SPolygonPair>& pairsToCheck)
{
	std::vector<AABB> boxes;
	CWorld* world = gVars->pWorld;

	for (size_t i = 0; i < world->GetPolygonCount(); ++i)
	{
		AABB aabb = createBox(world->GetPolygon(i));
		boxes.push_back(aabb);
	}

	for (size_t i = 0; i < world->GetPolygonCount(); ++i)
	{
		for (size_t j = i + 1; j < world->GetPolygonCount(); ++j)
		{
			if (boxes[i].isPotentiallyCollidingXY(boxes[j]))
				pairsToCheck.push_back(SPolygonPair(gVars->pWorld->GetPolygon(i), gVars->pWorld->GetPolygon(j)));
		}
	}
	
	boxes.clear();
}


AABB CBroadPhaseSweepAndPrune::createBox(CPolygonPtr polygon)
{
	AABB aabb;

	Vec2 point = polygon->TransformPoint(polygon->points[0]);

	aabb.MinX = point.x;
	aabb.MaxX = point.x;
	aabb.MinY = point.y;
	aabb.MaxY = point.y;

	for (size_t i = 1; i < polygon->points.size(); ++i)
	{
		Vec2 point = polygon->TransformPoint(polygon->points[i]);

		if (point.x < aabb.MinX)
			aabb.MinX = point.x;
		else if (point.x > aabb.MaxX)
			aabb.MaxX = point.x;

		if (point.y < aabb.MinY)
			aabb.MinY = point.y;
		else if (point.y > aabb.MaxY)
			aabb.MaxY = point.y;
	}

	return aabb;
}