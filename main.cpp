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

#include "OperatorOverloading.h"
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

	Ball ball{};
	ball.position = { 0.0f, 0.0f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;

	float deltaTime = 1.0f / 60.0f;

	bool isMove = false;

	ConicalPendulum conicalpendulum;
	conicalpendulum.anchor = { 0.0f, 1.0f, 0.0f };
	conicalpendulum.length = 0.8f;
	conicalpendulum.halfApexAngle = 0.7f;
	conicalpendulum.angle = 0.0f;
	conicalpendulum.angularVelocity = 0.0f;

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

		if (isMove) {
			conicalpendulum.angularVelocity = std::sqrt(9.8f / (conicalpendulum.length * std::cos(conicalpendulum.halfApexAngle)));
			conicalpendulum.angle += conicalpendulum.angularVelocity * deltaTime;
		}

		float radius = std::sin(conicalpendulum.halfApexAngle) * conicalpendulum.length;
		float height = std::cos(conicalpendulum.halfApexAngle) * conicalpendulum.length;

		ball.position.x = conicalpendulum.anchor.x + std::cos(conicalpendulum.angle) * radius;
		ball.position.y = conicalpendulum.anchor.y - height;
		ball.position.z = conicalpendulum.anchor.z - std::sin(conicalpendulum.angle) * radius;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldMViewProjectionMatrix, viewportMatrix);
		

		Vector3 start= Transform(Transform(conicalpendulum.anchor, worldMViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(ball.position, worldMViewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		DrawSphere(Sphere(ball.position, ball.radius), worldMViewProjectionMatrix, viewportMatrix, ball.color);

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		
		if (isMove) {
			if (ImGui::Button("stop")) {
				isMove = false;
			}
		}
		else {
			if (ImGui::Button("start")) {
				isMove = true;
			}
		}

		ImGui::DragFloat("Length", &conicalpendulum.length, 0.01f, 0.01f, 2.0f);
		ImGui::DragFloat("HalfApexAngle", &conicalpendulum.halfApexAngle, 0.01f, 0.0f, 1.0f);

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
