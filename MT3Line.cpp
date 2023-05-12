#include "MT3Line.h"
#include <cmath>
#include "MT3Math.h"

Vector3 Project(const Vector3& v1, const Vector3& v2) {

	return Multiply(Dot(v1, Normalize(v2)), Normalize(v2));

}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {

	float t = Dot(Subtract(point, segment.origin), segment.diff) / std::powf(Length(segment.diff), 2.0f);
	t = Clamp(t, 1.0f, 0.0f);



	return Add(segment.origin ,Multiply(t, segment.diff));

}
