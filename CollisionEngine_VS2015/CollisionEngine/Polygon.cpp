#include "Polygon.h"
#include <GL/glu.h>

#include <iostream>

#include "PhysicEngine.h"
#include "AABB.h"

CPolygon::CPolygon(size_t index)
	: m_vertexBufferId(0), m_index(index), density(0.1f)
{
}

CPolygon::~CPolygon()
{
	DestroyBuffers();
}

void CPolygon::Build()
{
	m_lines.clear();

	CreateBuffers();
	BuildLines();
}

void CPolygon::Draw()
{
	// Set transforms (qssuming model view mode is set)
	float transfMat[16] = {	rotation.X.x, rotation.X.y, 0.0f, 0.0f,
							rotation.Y.x, rotation.Y.y, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f,
							position.x, position.y, -1.0f, 1.0f };
	glPushMatrix();
	glMultMatrixf(transfMat);

	// Draw vertices
	BindBuffers();
	glDrawArrays(GL_LINE_LOOP, 0, points.size());
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
}

size_t	CPolygon::GetIndex() const
{
	return m_index;
}

Vec2	CPolygon::TransformPoint(const Vec2& point) const
{
	return position + rotation * point;
}

Vec2	CPolygon::InverseTransformPoint(const Vec2& point) const
{
	return rotation.GetInverse() * (point - position);
}

bool	CPolygon::IsPointInside(const Vec2& point) const
{
	float maxDist = -FLT_MAX;

	for (const Line& line : m_lines)
	{
		Line globalLine = line.Transform(rotation, position);
		float pointDist = globalLine.GetPointDist(point);
		maxDist = Max(maxDist, pointDist);
	}

	return maxDist <= 0.0f;
}



CPolygon::Axis	CPolygon::GetAxis(const CPolygon& poly, const int& index) const
{
	Axis axis;
	
	int previousIndex = index - 1;
	if (previousIndex < 0)
		previousIndex += poly.m_lines.size();

	Line globalLine = poly.m_lines[index].Transform(poly.rotation, poly.position);

	Vec2 normal = globalLine.GetNormal();
	axis.line = Line(globalLine.point, normal); // on crée l'axe actuel
	axis.pointA = globalLine.point;
	axis.pointB = poly.m_lines[previousIndex].Transform(poly.rotation, poly.position).point;

	return axis;
}

CPolygon::MinMaxPolygon	CPolygon::GetMinMaxOfPolygon(const CPolygon& poly, const Line& axis) const
{
	std::vector<float> mults;

	// on parcourt tous les points pour trouver le min et le max des deux polygons
	for (size_t j = 0; j < poly.points.size(); ++j)
	{
		// on parcourt les points de ce polygon
		if (j < poly.points.size())
		{
			Vec2 proj = axis.Project(poly.TransformPoint(poly.points[j])); // projection du point sur l'axe
			float mult = (proj - axis.point) | axis.dir; // point_projete = mult * axis_vecteur_directeur;

			mults.push_back(mult); // stock le resultat
		}
	}

	// on initialise tout avec le premier point
	float minimumPoly = mults[0];
	float maximumPoly = mults[0];
	Vec2 minimumPolyPoint = poly.points[0];
	Vec2 maximumPolyPoint = poly.points[0];

	// on parcourt tous les résultats pour trouver le min et le max
	for (int j = 1; j < mults.size(); ++j)
	{
		float mult = mults[j];

		if (mult < minimumPoly)
		{
			minimumPoly = mult;
			minimumPolyPoint = poly.points[j];
		}
		else if (mult > maximumPoly)
		{
			maximumPoly = mult;
			maximumPolyPoint = poly.points[j];
		}
	}

	minimumPolyPoint = poly.TransformPoint(minimumPolyPoint);
	maximumPolyPoint = poly.TransformPoint(maximumPolyPoint);

	MinMaxPolygon result;
	result.minimumMult = minimumPoly;
	result.minimumPoint = minimumPolyPoint;
	result.maximumMult = maximumPoly;
	result.maximumPoint = maximumPolyPoint;

	return result;
}

CPolygon::MinimalPenetrationInfo	CPolygon::GetMinimalPenetrationInfo(const CPolygon::MinMaxPolygon& thisPoly, const CPolygon::MinMaxPolygon& otherPoly, const Axis& axis) const
{
	float distanceA = thisPoly.maximumMult - otherPoly.minimumMult;
	float distanceB = otherPoly.maximumMult - thisPoly.minimumMult;

	MinimalPenetrationInfo minimalPenetration;
	minimalPenetration.axis = axis;

	if (distanceA < distanceB)
	{
		minimalPenetration.distance = distanceA;
		minimalPenetration.pointMin = otherPoly.minimumPoint;
		minimalPenetration.pointMax = thisPoly.maximumPoint;
	}
	else
	{
		minimalPenetration.distance = distanceB;
		minimalPenetration.pointMin = thisPoly.minimumPoint;
		minimalPenetration.pointMax = otherPoly.maximumPoint;
	}

	return minimalPenetration;
}

bool	CPolygon::CheckCollision(const CPolygon& poly, Vec2& colPoint, Vec2& colNormal, float& colDist) const//, Vec2& axisA, Vec2& axisB, Vec2& min, Vec2& max) const
{
	MinimalPenetrationInfo minimalPenetration;
	minimalPenetration.distance = FLT_MAX;

	// on parcourt tous les axes de ce polygon
	for (size_t i = 0; i < m_lines.size(); ++i)
	{
		Axis axis = GetAxis(*this, i);
		axis.isOnThisPoly = true;

		MinMaxPolygon thisPoly = GetMinMaxOfPolygon(*this, axis.line);
		MinMaxPolygon otherPoly = GetMinMaxOfPolygon(poly, axis.line);

		float distanceA = thisPoly.maximumMult - otherPoly.minimumMult;
		float distanceB = otherPoly.maximumMult - thisPoly.minimumMult;

		// si il n'y a pas d'overlap, alors pas de collision
		if (distanceA < 0 || distanceB < 0)
			return false;

		// il y a collision !
		// on cherche maintenant à trouver la plus petite pénétration

		MinimalPenetrationInfo axisMinimalPenetration = GetMinimalPenetrationInfo(thisPoly, otherPoly, axis);

		if (axisMinimalPenetration.distance < minimalPenetration.distance)
		{
			minimalPenetration = axisMinimalPenetration;
		}
	}

	// on parcourt tous les axes de l'autre polygone
	for (size_t i = 0; i < poly.m_lines.size(); ++i)
	{
		Axis axis = GetAxis(poly, i);
		axis.isOnThisPoly = false;

		MinMaxPolygon thisPoly = GetMinMaxOfPolygon(*this, axis.line);
		MinMaxPolygon otherPoly = GetMinMaxOfPolygon(poly, axis.line);

		float distanceA = thisPoly.maximumMult - otherPoly.minimumMult;
		float distanceB = otherPoly.maximumMult - thisPoly.minimumMult;

		// si il n'y a pas d'overlap, alors pas de collision
		if (distanceA < 0 || distanceB < 0)
			return false;

		// il y a collision !
		// on cherche maintenant à trouver la plus petite pénétration

		MinimalPenetrationInfo axisMinimalPenetration = GetMinimalPenetrationInfo(thisPoly, otherPoly, axis);

		if (axisMinimalPenetration.distance < minimalPenetration.distance)
		{
			minimalPenetration = axisMinimalPenetration;
		}
	}

	colDist = minimalPenetration.distance;
	/*min = minimalPenetration.pointMin;
	max = minimalPenetration.pointMax;
	axisA = minimalPenetration.axis.pointA;
	axisB = minimalPenetration.axis.pointB;*/

	if ((minimalPenetration.axis.pointA.x != minimalPenetration.pointMin.x || minimalPenetration.axis.pointA.y != minimalPenetration.pointMin.y) && (minimalPenetration.axis.pointB.x != minimalPenetration.pointMin.x || minimalPenetration.axis.pointB.y != minimalPenetration.pointMin.y))
		colPoint = minimalPenetration.pointMin;
	else
		colPoint = minimalPenetration.pointMax;

	colNormal = minimalPenetration.axis.line.dir;

	if (!minimalPenetration.axis.isOnThisPoly)
	{
		colPoint = colPoint + colNormal * colDist;
		colNormal = Vec2(-colNormal.x, -colNormal.y);
	}

	/*
		Vec2 collisionPointTmp;

		Vec2 pointMin = (indexBestMin >= points.size()) ? poly.TransformPoint(poly.points[indexBestMin % points.size()]) : TransformPoint(points[indexBestMin]);
		Vec2 pointMax = (indexBestMax >= points.size()) ? poly.TransformPoint(poly.points[indexBestMax % points.size()]) : TransformPoint(points[indexBestMax]);

		Vec2 linePointA = poly.m_lines[bestAxis % m_lines.size()].Transform(poly.rotation, poly.position).point;
		Line linePointBTmp = poly.m_lines[(bestAxis + m_lines.size() - 1) % (int)m_lines.size()];
		Vec2 linePointB = linePointBTmp.Transform(poly.rotation, poly.position).point;
		Vec2 notColPoint;

		if ((linePointA.x != pointMin.x || linePointA.y != pointMin.y) && (linePointB.x != pointMin.x || linePointB.y != pointMin.y))
		{
			colPoint = pointMin;
			notColPoint = pointMax;
		}
		else
		{
			colPoint = pointMax;
			notColPoint = pointMin;
		}

		Vec2 projectedColPoint = poly.m_lines[bestAxis % m_lines.size()].Transform(poly.rotation, poly.position).Project(colPoint);
		colDist = (projectedColPoint - colPoint).GetLength();
		colPoint = projectedColPoint;

		colNormal = poly.m_lines[bestAxis % m_lines.size()].Transform(rotation, position).GetNormal();
		colNormal = Vec2(-colNormal.x, -colNormal.y);

		min = pointMin;
		max = pointMax;
		axisA = linePointA;
		axisB = linePointB;
	*/

	return true;
}

void CPolygon::CreateBuffers()
{
	DestroyBuffers();

	float* vertices = new float[3 * points.size()];
	for (size_t i = 0; i < points.size(); ++i)
	{
		vertices[3 * i] = points[i].x;
		vertices[3 * i + 1] = points[i].y;
		vertices[3 * i + 2] = 0.0f;
	}

	glGenBuffers(1, &m_vertexBufferId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * points.size(), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}

void CPolygon::BindBuffers()
{
	if (m_vertexBufferId != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
	}
}


void CPolygon::DestroyBuffers()
{
	if (m_vertexBufferId != 0)
	{
		glDeleteBuffers(1, &m_vertexBufferId);
		m_vertexBufferId = 0;
	}
}

void CPolygon::BuildLines()
{
	for (size_t index = 0; index < points.size(); ++index)
	{
		const Vec2& pointA = points[index];
		const Vec2& pointB = points[(index + 1) % points.size()];

		Vec2 lineDir = (pointA - pointB).Normalized();

		m_lines.push_back(Line(pointB, lineDir));
	}
}


void CPolygon::ToAABB()
{
	this->AABB.PolygonToAABB(this);
}
