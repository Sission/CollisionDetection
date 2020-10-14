#include "primitivetest.h"
#include "Vector3D.h"
#include "Vector2D.h"


primitivetest::primitivetest()
{
}


std::vector<Vector3D> primitivetest::maptoplane(Vector3D vertice)

{

	xyplane = Vector3D(vertice.m_x, vertice.m_y, 0);
	xzplane = Vector3D(vertice.m_x, 0, vertice.m_z);
	yzplane = Vector3D(0, vertice.m_y, vertice.m_z);

	map.clear();

	map.push_back(xyplane);
	map.push_back(xzplane);
	map.push_back(yzplane);
	return map;
}


bool primitivetest::xyplanecheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b)

{
	if (!sattest(a[0][0].m_x, a[0][0].m_y, a[1][0].m_x, a[1][0].m_y, a[2][0].m_x, a[2][0].m_y, b[0][0].m_x, b[0][0].m_y, b[1][0].m_x, b[1][0].m_y, b[2][0].m_x, b[2][0].m_y))
		return false;
	else
	{
		if (!sattest(a[0][0].m_x, a[0][0].m_y, a[2][0].m_x, a[2][0].m_y, a[1][0].m_x, a[1][0].m_y, b[0][0].m_x, b[0][0].m_y, b[1][0].m_x, b[1][0].m_y, b[2][0].m_x, b[2][0].m_y))
			return false;
		else
		{
			if (!sattest(a[1][0].m_x, a[1][0].m_y, a[2][0].m_x, a[2][0].m_y, a[0][0].m_x, a[0][0].m_y, b[0][0].m_x, b[0][0].m_y, b[1][0].m_x, b[1][0].m_y, b[2][0].m_x, b[2][0].m_y))
				return false;
			else
			{
				if (!sattest(b[0][0].m_x, b[0][0].m_y, b[1][0].m_x, b[1][0].m_y, b[2][0].m_x, b[2][0].m_y, a[0][0].m_x, a[0][0].m_y, a[1][0].m_x, a[1][0].m_y, a[2][0].m_x, a[2][0].m_y))
					return false;
				else
				{
					if (!sattest(b[0][0].m_x, b[0][0].m_y, b[2][0].m_x, b[2][0].m_y, b[1][0].m_x, b[1][0].m_y, a[0][0].m_x, a[0][0].m_y, a[1][0].m_x, a[1][0].m_y, a[2][0].m_x, a[2][0].m_y))
						return false;
					else
					{
						if (!sattest(b[1][0].m_x, b[1][0].m_y, b[2][0].m_x, b[2][0].m_y, b[0][0].m_x, b[0][0].m_y, a[0][0].m_x, a[0][0].m_y, a[1][0].m_x, a[1][0].m_y, a[2][0].m_x, a[2][0].m_y))
							return false;
						else
							return true; //true means they are intersection.
					}
				}
			}
		}
	}
}

bool primitivetest::xzplanecheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b)
{
	if (!sattest(a[0][0].m_x, a[0][0].m_z, a[1][0].m_x, a[1][0].m_z, a[2][0].m_x, a[2][0].m_z, b[0][0].m_x, b[0][0].m_z, b[1][0].m_x, b[1][0].m_z, b[2][0].m_x, b[2][0].m_z))
		return false;
	else
	{
		if (!sattest(a[0][0].m_x, a[0][0].m_z, a[2][0].m_x, a[2][0].m_z, a[1][0].m_x, a[1][0].m_z, b[0][0].m_x, b[0][0].m_z, b[1][0].m_x, b[1][0].m_z, b[2][0].m_x, b[2][0].m_z))
			return false;
		else
		{
			if (!sattest(a[1][0].m_x, a[1][0].m_z, a[2][0].m_x, a[2][0].m_z, a[0][0].m_x, a[0][0].m_z, b[0][0].m_x, b[0][0].m_z, b[1][0].m_x, b[1][0].m_z, b[2][0].m_x, b[2][0].m_z))
				return false;
			else
			{
				if (!sattest(b[0][0].m_x, b[0][0].m_z, b[1][0].m_x, b[1][0].m_z, b[2][0].m_x, b[2][0].m_z, a[0][0].m_x, a[0][0].m_z, a[1][0].m_x, a[1][0].m_z, a[2][0].m_x, a[2][0].m_z))
					return false;
				else
				{
					if (!sattest(b[0][0].m_x, b[0][0].m_z, b[2][0].m_x, b[2][0].m_z, b[1][0].m_x, b[1][0].m_z, a[0][0].m_x, a[0][0].m_z, a[1][0].m_x, a[1][0].m_z, a[2][0].m_x, a[2][0].m_z))
						return false;
					else
					{
						if (!sattest(b[1][0].m_x, b[1][0].m_z, b[2][0].m_x, b[2][0].m_z, b[0][0].m_x, b[0][0].m_z, a[0][0].m_x, a[0][0].m_z, a[1][0].m_x, a[1][0].m_z, a[2][0].m_x, a[2][0].m_z))
							return false;
						else
							return true; //true means they are intersection.
					}
				}
			}
		}
	}
}

bool primitivetest::yzplanecheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b)
{
	if (!sattest(a[0][0].m_z, a[0][0].m_y, a[1][0].m_z, a[1][0].m_y, a[2][0].m_z, a[2][0].m_y, b[0][0].m_z, b[0][0].m_y, b[1][0].m_z, b[1][0].m_y, b[2][0].m_z, b[2][0].m_y))
		return false;
	else
	{
		if (!sattest(a[0][0].m_z, a[0][0].m_y, a[2][0].m_z, a[2][0].m_y, a[1][0].m_z, a[1][0].m_y, b[0][0].m_z, b[0][0].m_y, b[1][0].m_z, b[1][0].m_y, b[2][0].m_z, b[2][0].m_y))
			return false;
		else
		{
			if (!sattest(a[1][0].m_z, a[1][0].m_y, a[2][0].m_z, a[2][0].m_y, a[0][0].m_z, a[0][0].m_y, b[0][0].m_z, b[0][0].m_y, b[1][0].m_z, b[1][0].m_y, b[2][0].m_z, b[2][0].m_y))
				return false;
			else
			{
				if (!sattest(b[0][0].m_z, b[0][0].m_y, b[1][0].m_z, b[1][0].m_y, b[2][0].m_z, b[2][0].m_y, a[0][0].m_z, a[0][0].m_y, a[1][0].m_z, a[1][0].m_y, a[2][0].m_z, a[2][0].m_y))
					return false;
				else
				{
					if (!sattest(b[0][0].m_z, b[0][0].m_y, b[2][0].m_z, b[2][0].m_y, b[1][0].m_z, b[1][0].m_y, a[0][0].m_z, a[0][0].m_y, a[1][0].m_z, a[1][0].m_y, a[2][0].m_z, a[2][0].m_y))
						return false;
					else
					{
						if (!sattest(b[1][0].m_z, b[1][0].m_y, b[2][0].m_z, b[2][0].m_y, b[0][0].m_z, b[0][0].m_y, a[0][0].m_z, a[0][0].m_y, a[1][0].m_z, a[1][0].m_y, a[2][0].m_z, a[2][0].m_y))
							return false;
						else
							return true; //true means they are intersection.
					}
				}
			}
		}
	}
}



bool primitivetest::sattest(double xa0, double ya0, double xa1, double ya1, double xa2, double ya2, double xb0, double yb0, double xb1, double yb1, double xb2, double yb2)
{
	double dropfeeta1 = getDropfeet(xa0, ya0, xa1, ya1, xa2, ya2);
	double dropfeeta2 = getDropfeet(xa0, ya0, xa1, ya1, xa1, ya1);
	mina = dropfeeta1 > dropfeeta2 ? dropfeeta2 : dropfeeta1;
	maxa = dropfeeta1 > dropfeeta2 ? dropfeeta1 : dropfeeta2;
	double dropfeetb1 = getDropfeet(xa0, ya0, xa1, ya1, xb0, yb0);
	double dropfeetb2 = getDropfeet(xa0, ya0, xa1, ya1, xb1, yb1);
	double dropfeetb3 = getDropfeet(xa0, ya0, xa1, ya1, xb2, yb2);
	minb = dropfeeta1 > dropfeeta2 ? dropfeeta2 : dropfeeta1;
	minb = minb > dropfeetb3 ? dropfeetb3 : minb;
	maxb = dropfeeta1 > dropfeeta2 ? dropfeeta1 : dropfeeta2;
	maxb = maxb > dropfeetb3 ? maxb : dropfeetb3;
	if (minb >= maxa || mina >= maxb)
	{
		return false; // no intersection
	}
	else
		return true;

}

double primitivetest::getDropfeet(double x0, double y0, double x1, double y1, double x2, double y2)
{
	k = (y1 - x1) / (y0 - x0);
	dropfeet=(x2 * k - y2) / (k + 1 / k);
	return dropfeet;
}



bool primitivetest::finalcheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b)
{
	/*if (xyplanecheck(a, b)&& xzplanecheck(a, b)&& yzplanecheck(a, b))
	{
		return true;
	}
	else
		return false;*/
	return (xyplanecheck(a, b) && xzplanecheck(a, b) && yzplanecheck(a, b));
}




primitivetest::~primitivetest()
{
}
