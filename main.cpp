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
#include <vector>

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

	Vector3 translates[3] = {
		{0.2f, 1.0f, 0.0f},
		{0.4f, 0.0f, 0.0f},
		{0.3f, 0.0f, 0.0f},
	};

	Vector3 rotates[3] = {
		{0.0f, 0.0f, -6.8f},
		{0.0f, 0.0f, -1.4f},
		{0.0f, 0.0f,  0.0f},
	};

	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};



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


		Matrix4x4 affineMatrix[3];
		//アフィン変換
		for (size_t i = 0; i < 3; i++)
		{
			affineMatrix[i] = MakeAffineMatrix(scales[i], rotates[i], translates[i]);
			if (i !=  0) {
				affineMatrix[i] = Multiply(affineMatrix[i], affineMatrix[i - 1]);
			}
		}

		Sphere spheres[3] = {
			{Transform(Vector3{ 0.0f, 0.0f, 0.0f },affineMatrix[0]), 0.05f},
			{Transform(Vector3{ 0.0f, 0.0f, 0.0f },affineMatrix[1]), 0.05f},
			{Transform(Vector3{ 0.0f, 0.0f, 0.0f },affineMatrix[2]), 0.05f},
		};

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldMViewProjectionMatrix, viewportMatrix);
		

		DrawSphere(spheres[0], worldMViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(spheres[1], worldMViewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(spheres[2], worldMViewProjectionMatrix, viewportMatrix, BLUE);

		Vector3 start = Transform(Transform(spheres[0].center, worldMViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(spheres[1].center, worldMViewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		start = Transform(Transform(spheres[1].center, worldMViewProjectionMatrix), viewportMatrix);
		end = Transform(Transform(spheres[2].center, worldMViewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		//DrawBezier(controlPoint[0], controlPoint[1], controlPoint[2],
		//	worldMViewProjectionMatrix, viewportMatrix, color);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);

		ImGui::DragFloat3("Translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("Rotates[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("Scales[0]", &scales[0].x, 0.01f);

		ImGui::DragFloat3("Translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("Rotates[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("Scales[1]", &scales[1].x, 0.01f);

		ImGui::DragFloat3("Translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("Rotates[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("Scales[2]", &scales[2].x, 0.01f);

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
