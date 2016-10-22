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

	/// Cette donnée n'a strictement rien à faire ici mais elle me facilite la vie.
	int polygonIndex;

	/// Lorsqu'un AABB est créé, il faut s'assurer que ces bornes sont défini aux valeurs floatantes minimum et maximum.
	/// Cela permet de s'assurer que les AABBS se remplissent correctement dans la méthode PolygonToAABB.
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

	/// Défini seulement si 2 AABBS sont égaux.
	inline bool operator==(const AABB const& rightAABB)
	{
		return	this->MinX == rightAABB.MinX &&
				this->MinY == rightAABB.MinY &&
				this->MaxX == rightAABB.MaxX &&
				this->MaxY == rightAABB.MaxY;
	}

	/// Détermine si l'AABB courant est plus petit que l'AABB passé en paramètre.
	inline bool operator<(const AABB const& rightAABB)
	{
		return this->MinX <= rightAABB.MinX;
	}

	/// Créé un AABB à partir d'un polygon.
	void PolygonToAABB(CPolygon* polygon);

	/// Renvoit si l'AABB "OtherAABB" collisionne avec l'AABB courant.
	bool Intersect(AABB& OtherAABB);
	/// Renvoit si le point à la position "posX" et "posY" collisionne avec l'AABB courant.
	bool Intersect(float posX, float posY);
};


#endif