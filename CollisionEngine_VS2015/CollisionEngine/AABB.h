#ifndef _AABB_H_
#define _AABB_H_

#include <cfloat>

class CPolygon;

struct AABB
{
	float MinX;
	float MinY;
	float MaxX;
	float MaxY;

	/// Cette donn�e n'a strictement rien � faire ici mais elle me facilite la vie.
	int polygonIndex;

	/// Lorsqu'un AABB est cr��, il faut s'assurer que ces bornes sont d�fini aux valeurs floatantes minimum et maximum.
	/// Cela permet de s'assurer que les AABBS se remplissent correctement dans la m�thode PolygonToAABB.
	AABB() :
		MinX(FLT_MIN),
		MinY(FLT_MIN),
		MaxX(FLT_MAX),
		MaxY(FLT_MAX)
	{
	}

	AABB(float minX, float minY, float maxX, float maxY) :
		MinX(minX),
		MinY(minY),
		MaxX(maxX),
		MaxY(maxY)
	{
	};

	/// D�fini seulement si 2 AABBS sont �gaux.
	inline bool operator==(const AABB const& rightAABB)
	{
		return	this->MinX == rightAABB.MinX &&
				this->MinY == rightAABB.MinY &&
				this->MaxX == rightAABB.MaxX &&
				this->MaxY == rightAABB.MaxY;
	}

	/// D�termine si l'AABB courant est plus petit que l'AABB pass� en param�tre.
	inline bool operator<(const AABB const& rightAABB)
	{
		return this->MinX <= rightAABB.MinX;
	}

	/// Cr�� un AABB � partir d'un polygon.
	void PolygonToAABB(CPolygon* polygon);

	/// Renvoit si l'AABB "OtherAABB" collisionne avec l'AABB courant.
	bool Intersect(AABB& OtherAABB);
	/// Renvoit si le point � la position "posX" et "posY" collisionne avec l'AABB courant.
	bool Intersect(float posX, float posY);
};


#endif