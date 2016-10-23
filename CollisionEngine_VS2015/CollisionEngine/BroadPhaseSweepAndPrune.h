#ifndef _BROAD_PHASE_SWEEP_AND_PRUNE_H_
#define _BROAD_PHASE_SWEEP_AND_PRUNE_H_

#include "BroadPhase.h"

#include "Polygon.h"
#include "GlobalVariables.h"
#include "World.h"

class CBroadPhaseSweepAndPrune : public IBroadPhase
{
public:
	virtual void GetCollidingPairsToCheck(
		std::vector<SPolygonPair>& pairsToCheck) override;

private:
	AABB createBox(CPolygonPtr polygon);
};

#endif