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
	Vector3 cameraTranslate = { 0.0f,4.9f,-20.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };

	//OBB
	Vector3 obbRotate = { 0.0f, 0.0f, 45.0f };
	OBB obb{
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.83f,0.26f,0.24f
	};

	Sphere s2;
	s2.center = { -5.0f,0.1f,0.0f};
	s2.radius = 0.2f;

	float s2Vector = 0.02f;

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

		//計算



		Vector3 s2PreCenter = s2.center;
		s2.center.x += s2Vector;

		Capsule c2;
		c2.segment.origin = s2PreCenter;
		c2.segment.diff = Subtract(s2.center, s2PreCenter);
		c2.radius = s2.radius;
		
		Vector3 p = {};
		float t = 0.0f;

		float dist = 0.0f;

		if (IsCollision(c2, obb, p, t, dist)) {
			s2.center = p;
			s2.center = Subtract(s2.center, Multiply(dist, Vector3(1.0f, 0.0f, 0.0f)));
			s2.center = Add(s2.center, Multiply(s2Vector * t, Vector3( 0.0f, 1.0f, 0.0f)));
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldMViewProjectionMatrix, viewportMatrix);
		
		//DrawSphere(Sphere(ball.position, ball.radius), worldMViewProjectionMatrix, viewportMatrix, ball.color);
		DrawOBB(obb, worldMViewProjectionMatrix, viewportMatrix, color);
		DrawSphere(s2, worldMViewProjectionMatrix, viewportMatrix, color);

		//デバッグ

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("center", &obb.center.x, 0.01f);
		ImGui::SliderAngle("obbRotateX", &obbRotate.x, 0.01f);
		ImGui::SliderAngle("obbRotateY", &obbRotate.y, 0.01f);
		ImGui::SliderAngle("obbRotateZ", &obbRotate.z, 0.01f);
		ImGui::DragFloat3("size", &obb.size.x, 0.01f);

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
