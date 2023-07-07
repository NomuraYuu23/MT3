#pragma once
#include "MT3Vector3.h"

struct OBB
{
	Vector3 center;//中心
	Vector3 otientatuons[3];//座標軸
	Vector3 size;//座標軸方向の長さの半分

};
