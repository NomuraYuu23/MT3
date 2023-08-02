#pragma once

#include "MT3Vector3.h"

struct ConicalPendulum {
	Vector3 anchor; // アンカーポイント
	float length;   // 紐の長さ
	float halfApexAngle; // 円錐の頂角の半分
	float angle;    //　現在の角度
	float angularVelocity; //角速度
};
