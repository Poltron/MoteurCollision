#include "BroadPhaseSweepAndPrune.h"
#include "AABB.h"

#include <algorithm>
#include <functional>

void CBroadPhaseSweepAndPrune::CalculPolygonsAABB(std::vector<AABB>& AABBS)
{
	for (size_t x = 0; x < gVars->pWorld->GetPolygonCount(); ++x)
	{
		auto polygonPtr = gVars->pWorld->GetPolygon(x);

		polygonPtr->ToAABB();

		polygonPtr->AABB.polygonIndex = x;
		
		AABBS.push_back(polygonPtr->AABB);
	}
}

void CBroadPhaseSweepAndPrune::SortPolygonsAABB(std::vector<AABB>& AABBS)
{
	std::sort(AABBS.begin(), AABBS.end(), [](AABB leftAABB, AABB rightAABB)
	{
		return leftAABB < rightAABB;
	});
}

void CBroadPhaseSweepAndPrune::GetCollidingPairsToCheck(
	std::vector<SPolygonPair>& pairsToCheck,
	std::vector<AABBPair>& pairsAABBToCheck)
{
	auto world = gVars->pWorld;

	world->AABBS.clear();
	this->CalculPolygonsAABB(world->AABBS);
	this->SortPolygonsAABB(world->AABBS);

	//this->FillAABBCollisionallyPairs(pairsAABBToCheck, world->AABBS);
	//this->SortAABBCollisionallyPairs(pairsAABBToCheck);
	//this->EraseDuplicateAABBCollisionallyPairs(pairsAABBToCheck);

	this->FillCollidingPairsToCheck(pairsToCheck, world->AABBS);
	
}

void CBroadPhaseSweepAndPrune::FillCollidingPairsToCheck(
	std::vector<SPolygonPair>& pairsToCheck,
	std::vector<AABB>& AABBS)
{
	auto world = gVars->pWorld;
	
	for (auto&& LeftAABB : AABBS)
	{
		for (auto&& RightAABB : AABBS)
		{
			if (LeftAABB.IntersectX(RightAABB))
			{
				if (LeftAABB.IntersectY(RightAABB))
					pairsToCheck.push_back(SPolygonPair(
						world->GetPolygon(LeftAABB.polygonIndex),
						world->GetPolygon(RightAABB.polygonIndex)));
			}
			else
				break;
		}
	}
}

/// Mes anciens tests, je les laisse ici pour que vous puissiez visualiser ce que j'ai tenté de faire.

//void CBroadPhaseSweepAndPrune::SortAABBCollisionallyPairs(std::vector<AABBPair>& pairsAABBToCheck)
//{
//	std::sort(pairsAABBToCheck.begin(), pairsAABBToCheck.end(), less_AABB());
//}

//void CBroadPhaseSweepAndPrune::EraseDuplicateAABBCollisionallyPairs(std::vector<AABBPair>& pairsAABBToCheck)
//{
//	for (auto&& LeftAABBPair : pairsAABBToCheck)
//	{
//		int index = 0;
//
//		for (auto&& RightAABBPair : pairsAABBToCheck)
//		{
//			if (LeftAABBPair == RightAABBPair)
//				pairsAABBToCheck.erase(pairsAABBToCheck.begin() + index);
//
//			index++;
//		}
//	}
//}

//void CBroadPhaseSweepAndPrune::FillCollidingPairsToCheck(
//	std::vector<SPolygonPair>& pairsToCheck,
//	std::vector<AABBPair>& pairsAABBToCheck)
//{
//	auto world = gVars->pWorld;
//	
//	for (auto&& pairAABB : pairsAABBToCheck)
//		pairsToCheck.push_back(SPolygonPair(
//			world->GetPolygon(pairAABB.AABBLeft->polygonIndex),
//			world->GetPolygon(pairAABB.AABBRight->polygonIndex)));
//}