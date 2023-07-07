#include <Novice.h>
#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "OBB.h"
#include "MT3RenderingPipeline.h"
#include "MT3Draw3D.h"
#include "MT3Line.h"
#include "MT3Collision.h"
#include <numbers>
#include <cmath>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_13_ノムラユウ_Noviceで3次元";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//---変数---//

	//3D描画
	Vector3 rotate = {0.0f, 0.0f, 0.0f};
	Vector3 translate = {};
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };

	//Segment segment{ {-0.7f, 0.3f, 0.0f}, {2.0f, -0.5f, 0.0f} };
	//Vector3 point{ -1.5f, 0.6f, 0.6f };

	//Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	//Vector3 closestPoint = ClosestPoint(point, segment);
	
	//Sphere sphere1{ point, 1.0f };
	//Sphere sphere2{ closestPoint, 1.0f };

	//Plane plane{ { 0.0f, 1.0f, 0.0f }, 1.0f};

	//Triangle triangle{ { -1.0f, 0.0f, 0.0f ,
	//					 0.0f, 1.0f, 0.0f ,
	//					 1.0f, 0.0f, 0.0f } };
	
	//plane.distance = Dot(point, plane.normal);

	//AABB aabb1{
	//	.min{-0.5f, -0.5f, -0.5f},
	//	.max{ 0.5f,  0.5f,  0.5f},
	//};

	//AABB aabb2{
	//.min{ 0.2f,  0.2f,  0.2f},
	//.max{ 1.0f,  1.0f,  1.0f},
	//};

	//OBB
	Vector3 obbRotate = { 0.0f, 0.0f, 0.0f };
	OBB obb{
		-1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.5f,0.5f,0.5f
	};
	Sphere sphere{
		0.0f,0.0f,0.0f,
		0.5f
	};

	unsigned int color = 0xFFFFFFFF;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//OBB軸
		Matrix4x4 rotateMatirx = Multiply(MakeRotateXMatrix(obbRotate.x), Multiply(MakeRotateYMatrix(obbRotate.y), MakeRotateZMatrix(obbRotate.z)));

		obb.otientatuons[0].x = rotateMatirx.m[0][0];
		obb.otientatuons[0].y = rotateMatirx.m[0][1];
		obb.otientatuons[0].z = rotateMatirx.m[0][2];

		obb.otientatuons[1].x = rotateMatirx.m[1][0];
		obb.otientatuons[1].y = rotateMatirx.m[1][1];
		obb.otientatuons[1].z = rotateMatirx.m[1][2];

		obb.otientatuons[2].x = rotateMatirx.m[2][0];
		obb.otientatuons[2].y = rotateMatirx.m[2][1];
		obb.otientatuons[2].z = rotateMatirx.m[2][2];

		//各種行列に計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldMViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (IsCollision(obb, sphere)) {
			color = RED;
		}
		else {
			color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldMViewProjectionMatrix, viewportMatrix);

		//DrawSphere(sphere1, worldMViewProjectionMatrix, viewportMatrix,WHITE);
		//DrawSphere(sphere2, worldMViewProjectionMatrix, viewportMatrix, WHITE);

		//Vector3 start = Transform(Transform(segment.origin, worldMViewProjectionMatrix), viewportMatrix);
		//Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), worldMViewProjectionMatrix), viewportMatrix);
		//Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		//DrawPlane(plane, worldMViewProjectionMatrix, viewportMatrix, WHITE);
		//DrawTriangle(triangle, worldMViewProjectionMatrix, viewportMatrix, WHITE);

		//DrawAABB(aabb1, worldMViewProjectionMatrix, viewportMatrix, color);
		//DrawAABB(aabb2, worldMViewProjectionMatrix, viewportMatrix, WHITE);

		DrawSphere(sphere, worldMViewProjectionMatrix, viewportMatrix, WHITE);
		DrawOBB(obb, worldMViewProjectionMatrix, viewportMatrix, color);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		//ImGui::DragFloat3("sphere1Center", &sphere1.center.x, 0.01f);
		//ImGui::DragFloat("sphere1Radius", &sphere1.radius, 0.01f);
		//ImGui::DragFloat3("sphere2Center", &sphere2.center.x, 0.01f);
		//ImGui::DragFloat("sphere2Radius", &sphere2.radius, 0.01f);

		//ImGui::DragFloat3("segmentOrigin", &segment.origin.x, 0.01f);
		//ImGui::DragFloat3("segmentDiff", &segment.diff.x, 0.01f);
		
		//ImGui::DragFloat3("plane.Normal", &plane.normal.x, 0.01f);
		//plane.normal = Normalize(plane.normal);
		//ImGui::DragFloat("plane.Distance", &plane.distance, 0.01f);

		//ImGui::DragFloat3("triangle.vertices[0]", &triangle.vertices[0].x, 0.01f);
		//ImGui::DragFloat3("triangle.vertices[1]", &triangle.vertices[1].x, 0.01f);
		//ImGui::DragFloat3("triangle.vertices[2]", &triangle.vertices[2].x, 0.01f);

		//ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		//ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		//aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		//aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		//aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		//aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		//aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		//aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);

		/*
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);
		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);
		*/

		ImGui::DragFloat3("obb.center", &obb.center.x, 0.01f);
		ImGui::SliderAngle("rotateX", &obbRotate.x, 0.01f);
		ImGui::SliderAngle("rotateY", &obbRotate.y, 0.01f);
		ImGui::SliderAngle("rotateZ", &obbRotate.z, 0.01f);
		ImGui::DragFloat3("obb.otientatuons[0]", &obb.otientatuons[0].x, 0.01f);
		ImGui::DragFloat3("obb.otientatuons[1]", &obb.otientatuons[1].x, 0.01f);
		ImGui::DragFloat3("obb.otientatuons[2]", &obb.otientatuons[2].x, 0.01f);
		ImGui::DragFloat3("obb.size", &obb.size.x, 0.01f);
		ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere.radius", &sphere.radius, 0.01f);





		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
