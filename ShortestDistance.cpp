#include "ShortestDistance.h"
#include <cmath>

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

float ShortestDistance::LineLineDist(const Line& l1, const Line& l2, Vector3& p1, Vector3& p2, float& t1, float t2)
{

	// 2直線が平行?
	if (std::fabsf(Dot(Normalize(l1.diff), Normalize(l2.diff))) == 1.0f) {

		float len = PointLineDist(l1.origin, l2, p2, t2);
		p1 = l1.origin;
		t1 = 0.0f;
		return len;

	}

	// 2直線はねじれ関係
	float dotV1V2 = Dot(l1.diff, l2.diff);
	float dotV1V1 = Length(Normalize(l1.diff));
	float dotV2V2 = Length(Normalize(l2.diff));
	Vector3 VecL1L2 = Subtract(l1.origin, l2.origin) ;

	t1 = (dotV1V2 * Dot(l2.diff, VecL1L2) - dotV2V2 * Dot(l1.diff, VecL1L2)) / (dotV1V1 * dotV2V2 - dotV1V2 * dotV1V2);
	p1 = Add(Multiply(1.0f - t1, l1.origin), Multiply(t1, Add(l1.origin, l1.diff)));
	t2 = Dot(l2.diff, Subtract(p1, l2.origin)) / dotV2V2;
	p2 = Add(Multiply(1.0f - t2, l2.origin), Multiply(t2, Add(l2.origin, l2.diff)));

	return Length(Subtract(p2,p1));

}

