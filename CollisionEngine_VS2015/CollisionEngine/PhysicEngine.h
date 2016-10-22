#ifndef _PHYSIC_ENGINE_H_
#define _PHYSIC_ENGINE_H_

#include <vector>
#include <unordered_map>
#include "Maths.h"
#include "Polygon.h"
#include "AABB.h"

class IBroadPhase;

struct AABBPair
{
	AABBPair(AABB* AABBLeft, AABB* AABBRight) : AABBLeft(AABBLeft), AABBRight(AABBRight) {}

	inline bool operator<(const AABBPair const& rightAABBPair)
	{
		return AABBLeft < AABBRight;
	}

	inline bool operator==(const AABBPair const& rightAABBPair)
	{
		return	(AABBLeft == rightAABBPair.AABBLeft &&
				AABBRight == rightAABBPair.AABBRight) ||

				(AABBLeft == rightAABBPair.AABBRight &&
				AABBRight == rightAABBPair.AABBLeft);
	}

	AABB*	AABBLeft;
	AABB*	AABBRight;
};

struct less_AABB
{
	inline bool operator() (const AABBPair& leftAABBPair, const AABBPair& rightAABBPair)
	{
		return (leftAABBPair.AABBLeft < rightAABBPair.AABBRight);
	}
};

struct SPolygonPair
{
	SPolygonPair(CPolygonPtr _polyA, CPolygonPtr _polyB) : polyA(_polyA), polyB(_polyB){}

	CPolygonPtr	polyA;
	CPolygonPtr	polyB;
};

struct SCollision
{
	SCollision() = default;
	SCollision(CPolygonPtr _polyA, CPolygonPtr _polyB, Vec2	_point, Vec2 _normal, float _distance)
		: polyA(_polyA), polyB(_polyB), point(_point), normal(_normal), distance(_distance){}

	CPolygonPtr	polyA, polyB;

	Vec2	point;
	Vec2	normal;
	float	distance;
};

class CPhysicEngine
{
public:
	void	Reset();
	void	Activate(bool active);

	void	DetectCollisions();

	void	Step(float deltaTime);

	template<typename TFunctor>
	void	ForEachCollision(TFunctor functor)
	{
		for (const SCollision& collision : m_collidingPairs)
		{
			functor(collision);
		}
	}

private:
	friend class CPenetrationVelocitySolver;

	void						CollisionBroadPhase();
	void						CollisionNarrowPhase();

	bool						m_active = true;

	// Collision detection
	IBroadPhase*				m_broadPhase;
	std::vector<SPolygonPair>	m_pairsToCheck;
	std::vector<AABBPair>		m_pairsAABBToCheck;
	std::vector<SCollision>		m_collidingPairs;
};

#endif