#include "ShortestDistance.h"

ShortestDistance* ShortestDistance::GetInstance()
{
	static ShortestDistance instance;
	return &instance;
}

float ShortestDistance::PointLineDist(const Vector3& p, const Line& l, Vector3& h, float& t)
{

	float lenSqV = Length(l.diff);
	t = 0.0f;
	if (lenSqV > 0.0f) {
		t = Dot(l.diff, Subtract(p, l.origin)) / lenSqV;
	}

	h = Add(l.origin, Multiply(t, l.diff));

	return Length( Subtract(h, p));

}
