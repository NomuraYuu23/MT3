#include <Novice.h>
#include <assert.h>
#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "MT3Draw3D.h"
#include <cmath>


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