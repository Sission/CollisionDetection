#pragma once
#include <memory>
#include "Vector3D.h"

#define max(a, b)(((a)<(b))?(a):(b))
#define min(a, b)(((a)>(b))?(a):(b))

class AABB
{
public:
	AABB() : min_coord(Vector3D(0.0f, 0.0f, 0.0f)), max_coord(Vector3D(0.0f, 0.0f, 0.0f)), surface_area(0.0f)
	{
	}

	AABB(Vector3D min_vertex, Vector3D max_vertex) : min_coord(min_vertex), max_coord(max_vertex) //issue: this part have no validation of input coordinates, where min coord should be smaller or equal than max coord
	{
		surface_area = calculateSurfaceArea();
	}

	bool setAABB(Vector3D min_vertex, Vector3D max_vertex)
	{
		if (!(min_vertex.m_x <= max_vertex.m_x && min_vertex.m_y <= max_vertex.m_y && min_vertex.m_z <= max_vertex.m_z))
			return false;
		else
		{
			min_coord = Vector3D(min_vertex.m_x, min_vertex.m_y, min_vertex.m_z);
			max_coord = Vector3D(max_vertex.m_x, max_vertex.m_y, max_vertex.m_z);
			return true;
		}
	}

	float calculateSurfaceArea() const { return 2.0f * (getWidth() * getHeight() + getWidth() * getDepth() + getHeight() * getDepth()); }

	bool operator &(const AABB& other) const //overlap operator
	{
		// y is deliberately first in the list of checks below as it is seen as more likely than things
		// collide on x,z but not on y than they do on y thus we drop out sooner on a y fail

		return max_coord.m_x > other.min_coord.m_x &&
			min_coord.m_x < other.max_coord.m_x&&
			max_coord.m_y > other.min_coord.m_y &&
			min_coord.m_y < other.max_coord.m_y&&
			max_coord.m_z > other.min_coord.m_z &&
			min_coord.m_z < other.max_coord.m_z;
	}

	bool operator >=(const Vector3D& other) const
	{
		return other.m_x >= min_coord.m_x &&
			other.m_x <= max_coord.m_x &&
			other.m_y >= min_coord.m_y &&
			other.m_y <= max_coord.m_y &&
			other.m_z >= min_coord.m_z &&
			other.m_z <= max_coord.m_z;
	}

	bool operator >=(const AABB& other) const //containing operator
	{
		return other.min_coord.m_x >= min_coord.m_x &&
			other.max_coord.m_x <= max_coord.m_x &&
			other.min_coord.m_y >= min_coord.m_y &&
			other.max_coord.m_y <= max_coord.m_y &&
			other.min_coord.m_z >= min_coord.m_z &&
			other.max_coord.m_z <= max_coord.m_z;
	}

	bool operator <=(const AABB& other) const //contained by operator
	{
		return other.min_coord.m_x <= min_coord.m_x &&
			other.max_coord.m_x >= max_coord.m_x &&
			other.min_coord.m_y <= min_coord.m_y &&
			other.max_coord.m_y >= max_coord.m_y &&
			other.min_coord.m_z <= min_coord.m_z &&
			other.max_coord.m_z >= max_coord.m_z;
	}

	AABB operator ||(const AABB& other) const //merge (or union) operator
	{
		return AABB(
			Vector3D(min(min_coord.m_x, other.min_coord.m_x), min(min_coord.m_y, other.min_coord.m_y), min(min_coord.m_z, other.min_coord.m_z)),
			Vector3D(max(max_coord.m_x, other.max_coord.m_x), max(max_coord.m_y, other.max_coord.m_y), max(max_coord.m_z, other.max_coord.m_z))
		);
	}

	AABB operator &&(const AABB& other) const //intersection (or conjunction) operator
	{
		return AABB(
			Vector3D(max(min_coord.m_x, other.min_coord.m_x), max(min_coord.m_y, other.min_coord.m_y), max(min_coord.m_z, other.min_coord.m_z)),
			Vector3D(min(max_coord.m_x, other.max_coord.m_x), min(max_coord.m_y, other.max_coord.m_y), min(max_coord.m_z, other.max_coord.m_z))
		);
	}

	float getWidth() const { return max_coord.m_x - min_coord.m_x; }
	float getHeight() const { return max_coord.m_y - min_coord.m_y; }
	float getDepth() const { return max_coord.m_z - min_coord.m_z; }

	const Vector3D getMinCoordination()
	{
		return min_coord;
	}

	const Vector3D getMaxCoordination()
	{
		return max_coord;
	}

	~AABB()
	{
	}

private:
	Vector3D min_coord;
	Vector3D max_coord;
	float surface_area;
};

