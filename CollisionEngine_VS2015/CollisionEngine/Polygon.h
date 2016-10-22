#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <GL/glew.h>
#include <vector>
#include <memory>

#include "Maths.h"
#include "AABB.h"

class CPolygon
{
private:
	friend class CWorld;

	CPolygon(size_t index);
public:
	~CPolygon();

	void ToAABB();

	AABB				AABB;
	Vec2				position;
	Mat2				rotation;
	std::vector<Vec2>	points;

	void				Build();
	void				Draw();
	size_t				GetIndex() const;

	float				GetArea() const;

	Vec2				TransformPoint(const Vec2& point) const;
	Vec2				InverseTransformPoint(const Vec2& point) const;

	// if point is outside then returned distance is negative (and doesn't make sense)
	bool				IsPointInside(const Vec2& point) const;

	bool				CheckCollision(const CPolygon& poly, Vec2& colPoint, Vec2& colNormal, float& colDist) const;//, Vec2& axisA, Vec2& axisB, Vec2& min, Vec2& max) const;

	// Physics
	float				density;
	Vec2				speed;

private:
	void				CreateBuffers();
	void				BindBuffers();
	void				DestroyBuffers();

	void				BuildLines();

	GLuint				m_vertexBufferId;
	size_t				m_index;

	std::vector<Line>	m_lines;

	// narrow phase

	struct Axis
	{
		Line line;
		Vec2 pointA;
		Vec2 pointB;
		bool isOnThisPoly;
	};

	struct MinMaxPolygon
	{
		float minimumMult;
		float maximumMult;
		Vec2 maximumPoint;
		Vec2 minimumPoint;
	};

	struct MinimalPenetrationInfo
	{
		float distance;
		Vec2 pointMin;
		Vec2 pointMax;
		Axis axis;
	};

	Axis					GetAxis(const CPolygon& poly, const int& index) const;
	MinMaxPolygon			GetMinMaxOfPolygon(const CPolygon& poly, const Line& axis) const;
	MinimalPenetrationInfo	GetMinimalPenetrationInfo(const MinMaxPolygon& poly, const MinMaxPolygon& otherPoly, const Axis& axis) const;
};

typedef std::shared_ptr<CPolygon>	CPolygonPtr;

#endif