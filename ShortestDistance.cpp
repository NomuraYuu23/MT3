#include "ShortestDistance.h"

ShortestDistance* ShortestDistance::GetInstance()
{
	static ShortestDistance instance;
	return &instance;
}

float ShortestDistance::PointLineDist(const Vector3& p, const Line& l, Vector3& h, float& t)
{

	float lenSqV = Length(Normalize(l.diff));
	t = 0.0f;
	if (lenSqV > 0.0f) {
		t = Dot(l.diff, Subtract(p, l.origin)) / lenSqV;
	}

	h = Add(l.origin, Multiply(t, l.diff));

	return Length( Subtract(h, p));

}

float ShortestDistance::PointSegmentDist(const Vector3& p, const Segment& seg, Vector3& h, float& t)
{

	const Vector3 e = Add(seg.origin, seg.diff);

	// 垂線の長さ、垂線の足の座標およびtを算出
	float len = PointLineDist(p, Line(seg.origin, Subtract(e, seg.origin)), h, t);

	if (Dot(Subtract(p, seg.origin), Subtract(e, seg.origin)) < 0.0f) {
		// 始点側の外側
		h = seg.origin;
		return Length(Subtract(seg.origin, p));
	}
	else if (Dot(Subtract(p, e), Subtract(seg.origin, e)) < 0.0f) {
		// 終点側の外側
		h = e;
		return Length(Subtract(e, p));
	}

	return len;

}

