#pragma once
#include "Vector3D.h"
#include <vector>


class primitivetest
{

public:
	primitivetest();
	std::vector<Vector3D> maptoplane(Vector3D a);
	bool xyplanecheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b);
	bool xzplanecheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b);
	bool yzplanecheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b);
	bool finalcheck(std::vector<std::vector<Vector3D>> a, std::vector<std::vector<Vector3D>> b);
	double getDropfeet(double x0, double y0, double x1, double y1, double x2, double y2);
	void test();
	bool sattest(double xa0, double ya0, double xa1, double ya1, double xa2, double ya2, double xb0, double yb0, double xb1, double yb1, double xb2, double yb2);
	~primitivetest();


private:
	Vector3D xyplane;
	Vector3D xzplane;
	Vector3D yzplane;
	Vector3D sample;
	double xa0;
	double ya0;
	double xa1;
	double ya1;
	double xa2;
	double ya2;
	double k;
	std::vector<Vector3D> map;
	std::vector<Vector3D> vectice_list;
	std::vector<Vector3D> vectice_list2;
	std::vector<std::vector<Vector3D>> mapa_list;
	std::vector<std::vector<Vector3D>> mapb_list;
	double dropfeet;
	double mina;
	double maxa;
	double minb;
	double maxb;
	bool result1;
	bool result2;
	bool result3;
	bool result4;
	bool result5;
	bool result6;

};

