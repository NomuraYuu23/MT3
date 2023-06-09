#include "MT3Collision.h"

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	//2つの球の中心点間の距離を求める
	float distance = Length(Subtract(s2.center, s1.center));
	//半径の合計よりも短ければ衝突
	if (distance <= s1.radius + s2.radius) {
		return true;
	}

	return false;

}
