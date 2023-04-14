#include <Novice.h>
#include "MT3Vector3.h"
#include <math.h>

//加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };

	return result;

}

//減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

	return result;

}

//スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {

	Vector3 result;

	result = { scalar * v.x, scalar * v.y, scalar * v.z };

	return result;

}

//内積
float Dot(const Vector3& v1, const Vector3& v2) {

	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;

}

//長さ（ノルム）
float Length(const Vector3& v) {

	float result;

	result = sqrtf(Dot(v, v));

	return result;

}


//正規化
Vector3 Normalize(const Vector3& v) {

	Vector3 result;
	float norm;

	norm = Length(v);

	if (v.x != 0.0) {
		result.x = v.x / norm;
	}
	else {
		result.x = 0.0f;
	}

	if (v.y != 0.0) {
		result.y = v.y / norm;
	}
	else {
		result.y = 0.0f;
	}


	if (v.z != 0.0) {
		result.z = v.z / norm;
	}
	else {
		result.z = 0.0f;
	}

	return result;

}

//ベクトル描画
void VectorScreenPrintf(int x, int y, const Vector3& v, const char* name) {

	Novice::ScreenPrintf(x, y, "%-7.02f%-7.02f%-7.02f%s", v.x, v.y, v.z, name);

}
