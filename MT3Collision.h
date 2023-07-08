#pragma once
#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "Sphere.h"
#include "Plane.h"
#include "MT3Line.h"
#include "Triangle.h"
#include "AABB.h"
#include "OBB.h"

/// <summary>
/// 球と球
/// </summary>
/// <param name="s1"></param>
/// <param name="s2"></param>
/// <returns></returns>
bool IsCollision(const Sphere& s1, const Sphere& s2);

/// <summary>
/// 球と平面
/// </summary>
/// <param name="sphere"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const Plane& plane);

/// <summary>
/// 直線と平面
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Line& line, const Plane& plane);

/// <summary>
/// 半直線と平面
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Ray& line, const Plane& plane);

/// <summary>
/// 線分と平面
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Segment& line, const Plane& plane);

/// <summary>
/// 直線と三角形
/// </summary>
/// <param name="line"></param>
/// <param name="triangle"></param>
/// <returns></returns>
bool IsCollision(const Line& line, const Triangle& triangle);

/// <summary>
/// 半直線と三角形
/// </summary>
/// <param name="line"></param>
/// <param name="triangle"></param>
/// <returns></returns>
bool IsCollision(const Ray& line, const Triangle& triangle);

/// <summary>
/// 線分と三角形
/// </summary>
/// <param name="line"></param>
/// <param name="triangle"></param>
/// <returns></returns>
bool IsCollision(const Segment& line, const Triangle& triangle);

/// <summary>
/// AABBとAABB
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

/// <summary>
/// AABBと球
/// </summary>
/// <param name="aabb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Sphere& sphere);

/// <summary>
/// AABBと直線
/// </summary>
/// <param name="aabb"></param>
/// <param name="line"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Line& line);

/// <summary>
/// AABBと半直線
/// </summary>
/// <param name="aabb"></param>
/// <param name="ray"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Ray& ray);

/// <summary>
/// AABBと線分
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment);

/// <summary>
/// OBBと球
/// </summary>
/// <param name="obb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Sphere& sphere);


/// <summary>
/// OBBと直線
/// </summary>
/// <param name="obb"></param>
/// <param name="line"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Line& line);

/// <summary>
/// OBBと半直線
/// </summary>
/// <param name="obb"></param>
/// <param name="ray"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Ray& ray);

/// <summary>
/// OBBと線分
/// </summary>
/// <param name="obb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Segment& segment);
