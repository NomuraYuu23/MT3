#include <Novice.h>
#include <assert.h>
#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "MT3Draw3D.h"
#include <cmath>
#include <numbers>

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfwidth = 2.0f;										//Gridの半分
	const uint32_t ksubdivsion = 10;										//分割数
	const float kGridEvery = (kGridHalfwidth * 2.0f) / float(ksubdivsion);	//１つ分の長さ

	Vector3 worldVertices[2];
	Vector3 screenVertices[2];
	Vector3 ndcVertex;

	//奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= ksubdivsion; ++xIndex) {
		//ワールド座標
		worldVertices[0] = { xIndex * kGridEvery - kGridHalfwidth,0.0f,kGridHalfwidth };
		worldVertices[1] = { xIndex * kGridEvery - kGridHalfwidth,0.0f, -kGridHalfwidth };
		//スクリーンへ変換
		for (uint32_t i = 0; i < 2; ++i) {
			ndcVertex = Transform(worldVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		if (xIndex * kGridEvery - kGridHalfwidth == 0.0f) {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0xAAAAAAFF);
		}

	}

	//左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= ksubdivsion; ++zIndex) {
		//ワールド座標
		worldVertices[0] = { kGridHalfwidth,0.0f,zIndex * kGridEvery - kGridHalfwidth };
		worldVertices[1] = { -kGridHalfwidth,0.0f, zIndex * kGridEvery - kGridHalfwidth };
		//スクリーンへ変換
		for (uint32_t i = 0; i < 2; ++i) {
			ndcVertex = Transform(worldVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		if (zIndex * kGridEvery - kGridHalfwidth == 0.0f) {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0xAAAAAAFF);
		}

	}


}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	const uint32_t kSubdivision = 16; //分割数
	const float kLonEvery = 2.0f * float(std::numbers::pi) / float(kSubdivision);//経度分割1つ分の角度
	const float kLatEvery = float(std::numbers::pi) / float(kSubdivision);//緯度分割1つ分の角度
	//緯度の方向に分割 
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -1.0f * float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;//現在の緯度
		//経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;//現在の経度
			//world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = { sphere.radius * std::cosf(lat) * std::cosf(lon), sphere.radius * std::sinf(lat), sphere.radius * std::cosf(lat) * std::sinf(lon) };
			a = Add(a, sphere.center);
			b = { sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon), sphere.radius * std::sinf(lat + kLatEvery), sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon) };
			b = Add(b, sphere.center);
			c = { sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery), sphere.radius * std::sinf(lat), sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery) };
			c = Add(c, sphere.center);

			//a,b,cをスクリーン座標へ
			a = Transform(a, viewProjectionMatrix);
			a = Transform(a, viewportMatrix);
			b = Transform(b, viewProjectionMatrix);
			b = Transform(b, viewportMatrix);
			c = Transform(c, viewProjectionMatrix);
			c = Transform(c, viewportMatrix);


			//線を引く
			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(a.x), int(a.y), int(c.x), int(c.y), color);

		}
	}

}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	//1.中心点を決める
	Vector3 center = Multiply(plane.distance, plane.normal);
	
	Vector3 perpendiculars[4];
	//2.法線と垂直なベクトルを一つ求める
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	//3.2の逆ベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	//4.2の法線とのクロス積を求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	//5.4の逆ベクトルを求める
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };
	//6.2から5のベクトルを中心点にそれぞれ定数倍して足すと4頂点が出来上がる
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	//線を引く
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);

}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Triangle triangle_{ Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix),
	 Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix),
	 Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix) };

	Novice::DrawLine(int(triangle_.vertices[0].x), int(triangle_.vertices[0].y), int(triangle_.vertices[1].x), int(triangle_.vertices[1].y), color);
	Novice::DrawLine(int(triangle_.vertices[1].x), int(triangle_.vertices[1].y), int(triangle_.vertices[2].x), int(triangle_.vertices[2].y), color);
	Novice::DrawLine(int(triangle_.vertices[2].x), int(triangle_.vertices[2].y), int(triangle_.vertices[0].x), int(triangle_.vertices[0].y), color);

}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Vector3 vertex[8];

	vertex[0] = { aabb.min.x , aabb.max.y, aabb.min.z };
	vertex[1] = { aabb.min.x , aabb.max.y, aabb.max.z };
	vertex[2] = { aabb.max.x , aabb.max.y, aabb.min.z };
	vertex[3] = { aabb.max.x , aabb.max.y, aabb.max.z };

	vertex[4] = { aabb.min.x , aabb.min.y, aabb.min.z };
	vertex[5] = { aabb.min.x , aabb.min.y, aabb.max.z };
	vertex[6] = { aabb.max.x , aabb.min.y, aabb.min.z };
	vertex[7] = { aabb.max.x , aabb.min.y, aabb.max.z };

	for (int i = 0; i < 8; i++) {
		vertex[i] = Transform(Transform(vertex[i], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(vertex[0].x), int(vertex[0].y), int(vertex[1].x), int(vertex[1].y), color);
	Novice::DrawLine(int(vertex[0].x), int(vertex[0].y), int(vertex[2].x), int(vertex[2].y), color);
	Novice::DrawLine(int(vertex[1].x), int(vertex[1].y), int(vertex[3].x), int(vertex[3].y), color);
	Novice::DrawLine(int(vertex[2].x), int(vertex[2].y), int(vertex[3].x), int(vertex[3].y), color);

	Novice::DrawLine(int(vertex[4].x), int(vertex[4].y), int(vertex[5].x), int(vertex[5].y), color);
	Novice::DrawLine(int(vertex[4].x), int(vertex[4].y), int(vertex[6].x), int(vertex[6].y), color);
	Novice::DrawLine(int(vertex[5].x), int(vertex[5].y), int(vertex[7].x), int(vertex[7].y), color);
	Novice::DrawLine(int(vertex[6].x), int(vertex[6].y), int(vertex[7].x), int(vertex[7].y), color);

	Novice::DrawLine(int(vertex[0].x), int(vertex[0].y), int(vertex[4].x), int(vertex[4].y), color);
	Novice::DrawLine(int(vertex[1].x), int(vertex[1].y), int(vertex[5].x), int(vertex[5].y), color);
	Novice::DrawLine(int(vertex[2].x), int(vertex[2].y), int(vertex[6].x), int(vertex[6].y), color);
	Novice::DrawLine(int(vertex[3].x), int(vertex[3].y), int(vertex[7].x), int(vertex[7].y), color);

}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	Vector3 vertex[8];

	vertex[0] = { 
		obb.center.x + (-obb.size.x * obb.otientatuons[0].x + obb.size.y * obb.otientatuons[0].y + -obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (-obb.size.x * obb.otientatuons[1].x + obb.size.y * obb.otientatuons[1].y + -obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (-obb.size.x * obb.otientatuons[2].x + obb.size.y * obb.otientatuons[2].y + -obb.size.z * obb.otientatuons[2].z) };

	vertex[1] = { 
		obb.center.x + (-obb.size.x * obb.otientatuons[0].x + obb.size.y * obb.otientatuons[0].y + obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (-obb.size.x * obb.otientatuons[1].x + obb.size.y * obb.otientatuons[1].y + obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (-obb.size.x * obb.otientatuons[2].x + obb.size.y * obb.otientatuons[2].y + obb.size.z * obb.otientatuons[2].z) };

	vertex[2] = { 
		obb.center.x + (obb.size.x * obb.otientatuons[0].x + obb.size.y * obb.otientatuons[0].y + -obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (obb.size.x * obb.otientatuons[1].x + obb.size.y * obb.otientatuons[1].y + -obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (obb.size.x * obb.otientatuons[2].x + obb.size.y * obb.otientatuons[2].y + -obb.size.z * obb.otientatuons[2].z) };

	vertex[3] = { 
		obb.center.x + (obb.size.x * obb.otientatuons[0].x + obb.size.y * obb.otientatuons[0].y + obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (obb.size.x * obb.otientatuons[1].x + obb.size.y * obb.otientatuons[1].y + obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (obb.size.x * obb.otientatuons[2].x + obb.size.y * obb.otientatuons[2].y + obb.size.z * obb.otientatuons[2].z) };

	vertex[4] = { 
		obb.center.x + (-obb.size.x * obb.otientatuons[0].x + -obb.size.y * obb.otientatuons[0].y + -obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (-obb.size.x * obb.otientatuons[1].x + -obb.size.y * obb.otientatuons[1].y + -obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (-obb.size.x * obb.otientatuons[2].x + -obb.size.y * obb.otientatuons[2].y + -obb.size.z * obb.otientatuons[2].z) };

	vertex[5] = { 
		obb.center.x + (-obb.size.x * obb.otientatuons[0].x + -obb.size.y * obb.otientatuons[0].y + obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (-obb.size.x * obb.otientatuons[1].x + -obb.size.y * obb.otientatuons[1].y + obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (-obb.size.x * obb.otientatuons[2].x + -obb.size.y * obb.otientatuons[2].y + obb.size.z * obb.otientatuons[2].z) };

	vertex[6] = { 
		obb.center.x + (obb.size.x * obb.otientatuons[0].x + -obb.size.y * obb.otientatuons[0].y + -obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (obb.size.x * obb.otientatuons[1].x + -obb.size.y * obb.otientatuons[1].y + -obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (obb.size.x * obb.otientatuons[2].x + -obb.size.y * obb.otientatuons[2].y + -obb.size.z * obb.otientatuons[2].z) };

	vertex[7] = { 
		obb.center.x + (obb.size.x * obb.otientatuons[0].x + -obb.size.y * obb.otientatuons[0].y + obb.size.z * obb.otientatuons[0].z) ,
		obb.center.y + (obb.size.x * obb.otientatuons[1].x + -obb.size.y * obb.otientatuons[1].y + obb.size.z * obb.otientatuons[1].z) ,
		obb.center.z + (obb.size.x * obb.otientatuons[2].x + -obb.size.y * obb.otientatuons[2].y + obb.size.z * obb.otientatuons[2].z) };

	for (int i = 0; i < 8; i++) {
		vertex[i] = Transform(Transform(vertex[i], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(vertex[0].x), int(vertex[0].y), int(vertex[1].x), int(vertex[1].y), color);
	Novice::DrawLine(int(vertex[0].x), int(vertex[0].y), int(vertex[2].x), int(vertex[2].y), color);
	Novice::DrawLine(int(vertex[1].x), int(vertex[1].y), int(vertex[3].x), int(vertex[3].y), color);
	Novice::DrawLine(int(vertex[2].x), int(vertex[2].y), int(vertex[3].x), int(vertex[3].y), color);

	Novice::DrawLine(int(vertex[4].x), int(vertex[4].y), int(vertex[5].x), int(vertex[5].y), color);
	Novice::DrawLine(int(vertex[4].x), int(vertex[4].y), int(vertex[6].x), int(vertex[6].y), color);
	Novice::DrawLine(int(vertex[5].x), int(vertex[5].y), int(vertex[7].x), int(vertex[7].y), color);
	Novice::DrawLine(int(vertex[6].x), int(vertex[6].y), int(vertex[7].x), int(vertex[7].y), color);

	Novice::DrawLine(int(vertex[0].x), int(vertex[0].y), int(vertex[4].x), int(vertex[4].y), color);
	Novice::DrawLine(int(vertex[1].x), int(vertex[1].y), int(vertex[5].x), int(vertex[5].y), color);
	Novice::DrawLine(int(vertex[2].x), int(vertex[2].y), int(vertex[6].x), int(vertex[6].y), color);
	Novice::DrawLine(int(vertex[3].x), int(vertex[3].y), int(vertex[7].x), int(vertex[7].y), color);

}

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	//分割数
	const size_t kSubdivision = 16;

	//線描画
	for (size_t i = 0; i < kSubdivision; i++)
	{

		float t = i * (1.0f / kSubdivision);

		Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t);
		Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, t);
		Vector3 p1 = Lerp(p0p1, p1p2, t);

		t += (1.0f / kSubdivision);

		p0p1 = Lerp(controlPoint0, controlPoint1, t);
		p1p2 = Lerp(controlPoint1, controlPoint2, t);
		Vector3 p2 = Lerp(p0p1, p1p2, t);


		p1 = Transform(Transform(p1, viewProjectionMatrix), viewportMatrix);
		p2 = Transform(Transform(p2, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(p1.x), int(p1.y), int(p2.x), int(p2.y), color);

	}

	//球の描画
	const float r = 0.01f;
	Sphere sphere[3] = {
		{controlPoint0, r},
		{controlPoint1, r},
		{controlPoint2, r},
	};

	for (size_t i = 0; i < 3; i++) {
		DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, BLACK);
	}

}

// 曲線描画テスト
void CatmullRomSplineDraw(
	const std::vector<Vector3>& controlPoints, size_t segmentCount,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color) {

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointDrawing;
	// 線分の数+1個分の頂点座標を計算
	for (size_t i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i;
		Vector3 pos = CatmullRomSpline(controlPoints, t);
		// 描画リストに追加
		pointDrawing.push_back(pos);
	}

	for (std::vector<Vector3>::iterator itr = pointDrawing.begin(); itr < pointDrawing.end() - 1;
		++itr) {
		
		Vector3 p0 = Transform(Transform(*itr, viewProjectionMatrix), viewportMatrix);
		Vector3 p1 = Transform(Transform(*(itr + 1), viewProjectionMatrix), viewportMatrix);
		


		Novice::DrawLine(int(p0.x), int(p0.y), int(p1.x), int(p1.y), color);
	}

	//球の描画
	const float r = 0.01f;
	std::vector<Sphere> Spheres;
	for (size_t i = 0; i < controlPoints.size(); i++)
	{
		Spheres.push_back(Sphere(controlPoints[i], r));
	}

	for (size_t i = 0; i < Spheres.size(); i++) {
		DrawSphere(Spheres[i], viewProjectionMatrix, viewportMatrix, BLACK);
	}

}