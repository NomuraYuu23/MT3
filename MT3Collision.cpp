#include "MT3Collision.h"
#include <cmath>
#include <algorithm>

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
	float distance = std::fabsf(Dot(plane.normal, sphere.center) - plane.distance);
	//2.1の距離 <= 球の半径なら衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}

bool IsCollision(const Line& line, const Plane& plane) {

	//垂直判定のため、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	//衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	//float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	return true;

}

bool IsCollision(const Ray& line, const Plane& plane) {

	//垂直判定のため、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	//衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	if (t < 0) {
		return false;
	}

	return true;

}

bool IsCollision(const Segment& line, const Plane& plane) {

	//垂直判定のため、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	//衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	if (t < 0 || t > 1.0f) {
		return false;
	}

	return true;

}

/// <summary>
/// 直線と三角形
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Line& line, const Triangle& triangle) {

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	//平面を求める
	Plane plane = { Normalize(Cross(v01, v12)),  0.0f };
	plane.distance = plane.normal.x * v20.x + plane.normal.y * v20.y + plane.normal.z * v20.z;

	//平面との衝突確認
	//垂直判定のため、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	//衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	Vector3 v1p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[1]);
	Vector3 v2p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[2]);
	Vector3 v0p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[0]);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	//三角形との衝突確認
	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;

}

/// <summary>
/// 半直線と三角形
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Ray& line, const Triangle& triangle) {

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	//平面を求める
	Plane plane = { Normalize(Cross(v01, v12)),  0.0f };
	plane.distance = plane.normal.x * v20.x + plane.normal.y * v20.y + plane.normal.z * v20.z;

	//平面との衝突確認
	//垂直判定のため、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	//衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	if (t < 0) {
		return false;
	}

	Vector3 v1p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[1]);
	Vector3 v2p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[2]);
	Vector3 v0p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[0]);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	//三角形との衝突確認
	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;

}

/// <summary>
/// 線分と三角形
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Segment& line, const Triangle& triangle) {

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);

	//平面を求める
	Plane plane = { Normalize(Cross(v01, v12)),  0.0f };
	plane.distance = plane.normal.x * v20.x + plane.normal.y * v20.y + plane.normal.z * v20.z;

	//平面との衝突確認
	//垂直判定のため、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	//衝突していない
	if (dot == 0.0f) {
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

	if (t < 0 || t > 1.0f) {
		return false;
	}

	Vector3 v1p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[1]);
	Vector3 v2p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[2]);
	Vector3 v0p = Subtract(Add(line.origin, Multiply(t, line.diff)), triangle.vertices[0]);

	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	//三角形との衝突確認
	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f) {
		return true;
	}

	return false;

}

/// <summary>
/// AABBとAABB
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
		return true;
	}

	return false;

}

/// <summary>
/// AABBと球
/// </summary>
/// <param name="aabb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Sphere& sphere) {

	//最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
		std::clamp(sphere.center.y, aabb.min.y, aabb.max.y) ,
		std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };

	//最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}

	return false;

}

/// <summary>
/// AABBと直線
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Line& line) {

	Vector3 tMin = {
		(aabb.min.x - line.origin.x) / line.diff.x,
		(aabb.min.y - line.origin.y) / line.diff.y,
		(aabb.min.z - line.origin.z) / line.diff.z };

	Vector3 tMax = {
		(aabb.max.x - line.origin.x) / line.diff.x,
		(aabb.max.y - line.origin.y) / line.diff.y,
		(aabb.max.z - line.origin.z) / line.diff.z };

	Vector3 tNear = { std::min(tMin.x, tMax.x) ,
		std::min(tMin.y, tMax.y) ,
		std::min(tMin.z, tMax.z) };

	Vector3 tFar = { std::max(tMin.x, tMax.x) ,
		std::max(tMin.y, tMax.y) ,
		std::max(tMin.z, tMax.z) };

	float tMin_ = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tMax_ = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tMin_ <= tMax_) {
		return true;
	}

	return false;

}

/// <summary>
/// AABBと半直線
/// </summary>
/// <param name="ray"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Ray& ray) {


	Vector3 tMin = {
		(aabb.min.x - ray.origin.x) / ray.diff.x,
		(aabb.min.y - ray.origin.y) / ray.diff.y,
		(aabb.min.z - ray.origin.z) / ray.diff.z };

	Vector3 tMax = {
		(aabb.max.x - ray.origin.x) / ray.diff.x,
		(aabb.max.y - ray.origin.y) / ray.diff.y,
		(aabb.max.z - ray.origin.z) / ray.diff.z };

	Vector3 tNear = { std::min(tMin.x, tMax.x) ,
		std::min(tMin.y, tMax.y) ,
		std::min(tMin.z, tMax.z) };

	Vector3 tFar = { std::max(tMin.x, tMax.x) ,
		std::max(tMin.y, tMax.y) ,
		std::max(tMin.z, tMax.z) };

	float tMin_ = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tMax_ = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tMin_ <= tMax_) {

		if (tMax_ > 0.0f) {
			return true;
		}
	}

	return false;

}

/// <summary>
/// AABBと線
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment) {

	Vector3 tMin = {
		(aabb.min.x - segment.origin.x) / segment.diff.x,
		(aabb.min.y - segment.origin.y) / segment.diff.y,
		(aabb.min.z - segment.origin.z) / segment.diff.z };

	Vector3 tMax = {
		(aabb.max.x - segment.origin.x) / segment.diff.x,
		(aabb.max.y - segment.origin.y) / segment.diff.y,
		(aabb.max.z - segment.origin.z) / segment.diff.z };

	Vector3 tNear = { std::min(tMin.x, tMax.x) ,
		std::min(tMin.y, tMax.y) ,
		std::min(tMin.z, tMax.z) };

	Vector3 tFar = { std::max(tMin.x, tMax.x) ,
		std::max(tMin.y, tMax.y) ,
		std::max(tMin.z, tMax.z) };

	float tMin_ = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tMax_ = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tMin_ <= tMax_) {

		if (tMin_ < 1.0f && tMax_ > 0.0f) {
			return true;
		}
	}

	return false;

}

/// <summary>
/// OBBと球
/// </summary>
/// <param name="obb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Sphere& sphere) {

	Matrix4x4 obbWorldMatrix = {
		obb.otientatuons[0].x,obb.otientatuons[1].x, obb.otientatuons[2].x, 0,
		obb.otientatuons[0].y,obb.otientatuons[1].y, obb.otientatuons[2].y, 0,
		obb.otientatuons[0].z,obb.otientatuons[1].z, obb.otientatuons[2].z, 0,
		obb.center.x, obb.center.y, obb.center.y, 1 };

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = Transform(sphere.center, obbWorldMatrixInverse);

	AABB aabbOBBLocal{
		-obb.size.x,-obb.size.y, -obb.size.z,
		obb.size.x,obb.size.y, obb.size.z };

	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };

	//ローカル空間で衝突
	return IsCollision(aabbOBBLocal, sphereOBBLocal);

}

/// <summary>
/// OBBと直線
/// </summary>
/// <param name="obb"></param>
/// <param name="line"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Line& line) {

	Matrix4x4 obbWorldMatrix = {
		obb.otientatuons[0].x,obb.otientatuons[1].x, obb.otientatuons[2].x, 0,
		obb.otientatuons[0].y,obb.otientatuons[1].y, obb.otientatuons[2].y, 0,
		obb.otientatuons[0].z,obb.otientatuons[1].z, obb.otientatuons[2].z, 0,
		obb.center.x, obb.center.y, obb.center.y, 1 };

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 localOrigin = Transform(line.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(line.origin,line.diff), obbWorldMatrixInverse);

	AABB localAABB{
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{obb.size.x, obb.size.y, obb.size.z},
	};

	Line localLine;
	localLine.origin = localOrigin;
	localLine.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localLine);

}

/// <summary>
/// OBBと半直線
/// </summary>
/// <param name="obb"></param>
/// <param name="ray"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Ray& ray) {

	Matrix4x4 obbWorldMatrix = {
	obb.otientatuons[0].x,obb.otientatuons[1].x, obb.otientatuons[2].x, 0,
	obb.otientatuons[0].y,obb.otientatuons[1].y, obb.otientatuons[2].y, 0,
	obb.otientatuons[0].z,obb.otientatuons[1].z, obb.otientatuons[2].z, 0,
	obb.center.x, obb.center.y, obb.center.y, 1 };

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 localOrigin = Transform(ray.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(ray.origin, ray.diff), obbWorldMatrixInverse);

	AABB localAABB{
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{obb.size.x, obb.size.y, obb.size.z},
	};

	Line localRay;
	localRay.origin = localOrigin;
	localRay.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localRay);

}

/// <summary>
/// OBBと線分
/// </summary>
/// <param name="obb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Segment& segment) {

	Matrix4x4 obbWorldMatrix = {
	obb.otientatuons[0].x,obb.otientatuons[1].x, obb.otientatuons[2].x, 0,
	obb.otientatuons[0].y,obb.otientatuons[1].y, obb.otientatuons[2].y, 0,
	obb.otientatuons[0].z,obb.otientatuons[1].z, obb.otientatuons[2].z, 0,
	obb.center.x, obb.center.y, obb.center.y, 1 };

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 localOrigin = Transform(segment.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(segment.origin, segment.diff), obbWorldMatrixInverse);

	AABB localAABB{
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{obb.size.x, obb.size.y, obb.size.z},
	};

	Line localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localSegment);
	
}

/// <summary>
/// OBBとOBB
/// </summary>
/// <param name="obb"></param>
/// <param name="obb2"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const OBB& obb2) {

	//軸
	Vector3 axis[15];

	//面法線
	axis[0] = obb.otientatuons[0];
	axis[1] = obb.otientatuons[1];
	axis[2] = obb.otientatuons[2];
	axis[3] = obb2.otientatuons[0];
	axis[4] = obb2.otientatuons[1];
	axis[5] = obb2.otientatuons[2];

	//クロス積
	axis[6] = Cross(obb.otientatuons[0], obb2.otientatuons[0]);
	axis[7] = Cross(obb.otientatuons[0], obb2.otientatuons[1]);
	axis[8] = Cross(obb.otientatuons[0], obb2.otientatuons[2]);
	axis[9] = Cross(obb.otientatuons[1], obb2.otientatuons[0]);
	axis[10] = Cross(obb.otientatuons[1], obb2.otientatuons[1]);
	axis[11] = Cross(obb.otientatuons[1], obb2.otientatuons[2]);
	axis[12] = Cross(obb.otientatuons[2], obb2.otientatuons[0]);
	axis[13] = Cross(obb.otientatuons[2], obb2.otientatuons[1]);
	axis[14] = Cross(obb.otientatuons[2], obb2.otientatuons[2]);

	//頂点
	Vector3 obbVertex[8];

	obbVertex[0] = {
		obb.center.x + obb.size.x,
		obb.center.y + obb.size.y,
		obb.center.z + obb.size.z };

	obbVertex[1] = {
		obb.center.x + obb.size.x,
		obb.center.y + obb.size.y,
		obb.center.z - obb.size.z };

	obbVertex[2] = {
		obb.center.x + obb.size.x,
		obb.center.y - obb.size.y,
		obb.center.z + obb.size.z };

	obbVertex[3] = {
		obb.center.x + obb.size.x,
		obb.center.y - obb.size.y,
		obb.center.z - obb.size.z };

	obbVertex[4] = {
		obb.center.x - obb.size.x,
		obb.center.y + obb.size.y,
		obb.center.z + obb.size.z };

	obbVertex[5] = {
		obb.center.x - obb.size.x,
		obb.center.y + obb.size.y,
		obb.center.z - obb.size.z };

	obbVertex[6] = {
		obb.center.x - obb.size.x,
		obb.center.y - obb.size.y,
		obb.center.z + obb.size.z };

	obbVertex[7] = {
		obb.center.x - obb.size.x,
		obb.center.y - obb.size.y,
		obb.center.z - obb.size.z };

	Vector3 obb2Vertex[8];

	obb2Vertex[0] = {
		obb2.center.x + obb2.size.x,
		obb2.center.y + obb2.size.y,
		obb2.center.z + obb2.size.z };

	obb2Vertex[1] = {
		obb2.center.x + obb2.size.x,
		obb2.center.y + obb2.size.y,
		obb2.center.z - obb2.size.z };

	obb2Vertex[2] = {
		obb2.center.x + obb2.size.x,
		obb2.center.y - obb2.size.y,
		obb2.center.z + obb2.size.z };

	obb2Vertex[3] = {
		obb2.center.x + obb2.size.x,
		obb2.center.y - obb2.size.y,
		obb2.center.z - obb2.size.z };

	obb2Vertex[4] = {
		obb2.center.x - obb2.size.x,
		obb2.center.y + obb2.size.y,
		obb2.center.z + obb2.size.z };

	obb2Vertex[5] = {
		obb2.center.x - obb2.size.x,
		obb2.center.y + obb2.size.y,
		obb2.center.z - obb2.size.z };

	obb2Vertex[6] = {
		obb2.center.x - obb2.size.x,
		obb2.center.y - obb2.size.y,
		obb2.center.z + obb2.size.z };

	obb2Vertex[7] = {
		obb2.center.x - obb2.size.x,
		obb2.center.y - obb2.size.y,
		obb2.center.z - obb2.size.z };

	//1.頂点を軸に対して射影
	for (int a = 0; a < 15; a++) {
		float min1 = 0.0f;
		float max1 = 0.0f;
		float min2 = 0.0f;
		float max2 = 0.0f;

		for (int v = 0; v < 8; v++) {
			//一時保存
			float tmp = 0.0f;
			//obb
			tmp = Dot(axis[a], obbVertex[v]);
			//2.射影した点の最大値と最小値を求める
			if (v == 0 || min1 > tmp) {
				min1 = tmp;
			}
			if (v == 0 || max1 < tmp) {
				max1 = tmp;
			}

			//obb2
			tmp = Dot(axis[a], obb2Vertex[v]);
			//2.射影した点の最大値と最小値を求める
			if (v == 0 || min2 > tmp) {
				min2 = tmp;
			}
			if (v == 0 || max2 < tmp) {
				max2 = tmp;
			}
		}

		//3.差分の形状を分離軸に射影した長さ
		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float LonSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < LonSpan) {
			//分離しているので分離軸
			return false;
		}

	}

	return true;

}
