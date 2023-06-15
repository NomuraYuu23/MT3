#include "MT3Collision.h"
#include <cmath>

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	//2つの球の中心点間の距離を求める
	float distance = Length(Subtract(s2.center, s1.center));
	//半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius) {
		return true;
	}

	return false;

}

bool IsCollision(const Sphere& sphere, const Plane& plane) {

	//1.平面と球の中心点との距離を求める
	float distance = std::fabsf(Dot(plane.normal,sphere.center) - plane.distance);
	//2.1の距離 <= 球の半径なら衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}
