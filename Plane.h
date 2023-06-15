#pragma once
#include "MT3Vector3.h"

/// <summary>
/// 平面
/// </summary>
struct Plane
{

	Vector3 normal;//法線
	float distance;//距離

};

Vector3 Perpendicular(const Vector3& vector);
