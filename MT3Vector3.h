#pragma once

//3次元ベクトル
struct Vector3
{
	float x;
	float y;
	float z;

};

//加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
//減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
//スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);
//内積
float Dot(const Vector3& v1, const Vector3& v2);
//長さ（ノルム）
float Length(const Vector3& v);
//正規化
Vector3 Normalize(const Vector3& v);
//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

//ベクトル描画
void VectorScreenPrintf(int x, int y, const Vector3& v, const char* name);
