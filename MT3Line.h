#pragma once
#include "MT3Vector3.h"

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};


Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
