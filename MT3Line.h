#pragma once
#include "MT3Vector3.h"

//直線
struct Line {
	Vector3 origin;
	Vector3 diff;
};

//半直線
struct Ray {
	Vector3 origin;
	Vector3 diff;
};

//線分
struct Segment {
	Vector3 origin;
	Vector3 diff;
};


Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
