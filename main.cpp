#include <Novice.h>
#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "OperatorOverloading.h"

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
#include <vector>

#include "Spring.h"
#include "Ball.h"
#include "Pendulum.h"
#include "ConicalPendulum.h"

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
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 translate = {};
	Vector3 cameraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };

	//Plane plane;
	//plane.normal = Normalize({ -0.2f, 1.2f, -0.3f });
	////plane.normal = Normalize({ 0.0f, 1.0f, 0.0f });
	//plane.distance = 0.0f;

	//Ball ball{};
	//ball.position = { 0.8f, 1.2f, 0.3f };
	//ball.mass = 2.0f;
	//ball.radius = 0.05f;
	//ball.color = WHITE;
	//ball.acceleration = { 0.0f,-9.8f, 0.0f };

	//// 反発係数
	//float e = 0.8f;

	//float deltaTime = 1.0f / 60.0f;

	//bool isMove = false;

	//// カプセル
	//Capsule capsule;
	//capsule.radius = ball.radius;
	//capsule.segment.origin = ball.position;
	//capsule.segment.diff = ball.position;

	//// 余分
	//float extra = 0.0001f;

	// カプセル1
	Capsule capsule1;
	capsule1.radius = 0.1f;
	capsule1.segment.origin = { 0.0f, 0.0f, 0.0f };
	capsule1.segment.diff = { 0.0f, 0.2f, 0.0f };

	Capsule capsule2;
	capsule2.radius = 0.1f;
	capsule2.segment.origin = { 0.0f, 1.0f, 0.0f };
	capsule2.segment.diff = { 0.0f, 0.2f, 0.0f };

	unsigned int color = WHITE;

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

		//各種行列に計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldMViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		//計算

		//if (isMove) {
		//	capsule.segment.origin = ball.position;
		//	ball.velocity = Add(ball.velocity, ball.acceleration * deltaTime);
		//	ball.position = Add(ball.position, ball.velocity * deltaTime);
		//	capsule.segment.diff = ball.position;
		//	if (IsCollision(capsulecapsule, plane)) {

		//		//isMove = false;//デバッグ
		//		
		//		float distance = Dot(plane.normal, ball.position) - plane.distance;
		//		if (distance >= 0 ) {
		//			ball.position = ball.position + (plane.normal * (ball.radius - distance + extra));
		//		}
		//		else{
		//			ball.position = ball.position + (plane.normal * (distance + ball.radius + extra));
		//		}

		//		ball.velocity = Reflect(ball.velocity, plane.normal) * e;

		//	}
		//}

		if (IsCollision(capsule1, capsule2)) {
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
		
		//DrawSphere(Sphere(ball.position, ball.radius), worldMViewProjectionMatrix, viewportMatrix, ball.color);

		//デバッグ
		DrawSphere(Sphere(capsule1.segment.origin, capsule1.radius), worldMViewProjectionMatrix, viewportMatrix, color);
		DrawSphere(Sphere(Add(capsule1.segment.diff,capsule1.segment.origin), capsule1.radius), worldMViewProjectionMatrix, viewportMatrix, color);

		DrawSphere(Sphere(capsule2.segment.origin, capsule2.radius), worldMViewProjectionMatrix, viewportMatrix, color);
		DrawSphere(Sphere(Add(capsule2.segment.diff, capsule2.segment.origin), capsule2.radius), worldMViewProjectionMatrix, viewportMatrix, color);

		//DrawPlane(plane, worldMViewProjectionMatrix, viewportMatrix, WHITE);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		
		ImGui::DragFloat3("origin1", &capsule1.segment.origin.x, 0.01f);
		ImGui::DragFloat3("diff1", &capsule1.segment.diff.x, 0.01f);

		ImGui::DragFloat3("origin2", &capsule2.segment.origin.x, 0.01f);
		ImGui::DragFloat3("diff2", &capsule2.segment.diff.x, 0.01f);

		//if (isMove) {
		//	if (ImGui::Button("stop")) {
		//		isMove = false;
		//	}
		//}
		//else {
		//	if (ImGui::Button("start")) {
		//		isMove = true;
		//	}
		//}
		//if (ImGui::Button("reset")) {
		//	ball.position = { 0.8f, 1.2f, 0.3f };
		//	ball.acceleration = { 0.0f,-9.8f, 0.0f };
		//	ball.velocity = { 0.0f,0.0f,0.0f };
		//	isMove = false;
		//}

		//ImGui::DragFloat("e", &e, 0.01f, 0.0f, 1.0f);

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
