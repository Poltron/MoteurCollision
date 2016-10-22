#ifndef _BROAD_PHASE_SWEEP_AND_PRUNE_H_
#define _BROAD_PHASE_SWEEP_AND_PRUNE_H_

#include "BroadPhase.h"

#include "Polygon.h"
#include "GlobalVariables.h"
#include "World.h"

class CBroadPhaseSweepAndPrune : public IBroadPhase
{
public:
	/// Calcul et remplie le vecteur des AABB avec la convertion des polygons en AABBS.
	void CalculPolygonsAABB(std::vector<AABB>& AABBS);
	void SortPolygonsAABB(std::vector<AABB>& AABBS);

	/// Remplir les paires d'AABB qui collisionnent entre-elles.
	void FillAABBCollisionallyPairs(
		std::vector<AABBPair>& pairsAABBToCheck,
		std::vector<AABB>& AABBS);
	
	/// Tri les paires d'AABB qui collisionnent entre-elles
	void SortAABBCollisionallyPairs(std::vector<AABBPair>& pairsAABBToCheck);

	/// Supprime les paires doublons d'AABB qui collisionnent entre-elles.
	void EraseDuplicateAABBCollisionallyPairs(std::vector<AABBPair>& pairsAABBToCheck);

	virtual void GetCollidingPairsToCheck(
		std::vector<SPolygonPair>& pairsToCheck,
		std::vector<AABBPair>& pairsAABBToCheck) override;

	/// Rempli les paires de polygones qui collisionenent entres eux avec les paires d'AABB qui collisionnent entre-eux.
	void FillCollidingPairsToCheck(
		std::vector<SPolygonPair>& pairsToCheck,
		std::vector<AABBPair>& pairsAABBToCheck);
};

#endif