#include <Novice.h>
#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "MT3RenderingPipeline.h"
#include <numbers>
#include <cmath>

const char kWindowTitle[] = "LE2A_13_ノムラユウ_Noviceで3次元";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//---変数---//

	//クロス積
	Vector3 v1 = { 1.2f, -3.9f, 2.5f };
	Vector3 v2 = { 2.8f, 0.4f, -1.3f };
	Vector3 cross = Cross(v1, v2);

	//3D描画
	Vector3 rotate = {};
	Vector3 translate = {};

	float moveSpeed = 0.1f;
	int angle = 0;

	Vector3 cameraPosition = { 0.0f,0.0f,-10.0f }; 
	Vector3 kLocalVertices[3] = { { -0.5f,-std::sqrtf(1.0f) / 2.0f,0.0f } ,{ 0.0f,std::sqrtf(1.0f) / 2.0f,0.0f} ,{0.5f,-std::sqrtf(1.0f) / 2.0f,0.0f}};


	//Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);
	//Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.0f);
	//Matrix4x4 viewportMatrix = MakeViewportMatrix(100.0f, 200.f, 600.0f, 300.0f, 0.0f, 1.0f);

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

		//移動と回転
		if (keys[DIK_W]) {
			translate.z += moveSpeed;
		}
		if (keys[DIK_S]) {
			translate.z -= moveSpeed;
		}
		if (keys[DIK_A]) {
			translate.x -= moveSpeed;
		}
		if (keys[DIK_D]) {
			translate.x += moveSpeed;
		}

		angle++;
		if (angle == 360) {
			angle = 0;
		}


		rotate.y = angle / 180.0f * float(std::numbers::pi);

		//各種行列に計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldMViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i){
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldMViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, cross, "Cross");
		
		Vector3 triangleCross = Cross(Subtract(screenVertices[1], screenVertices[0]), Subtract(screenVertices[2], screenVertices[1]));
		if (Dot(cameraPosition, triangleCross) <= 0.0f){
			Novice::DrawTriangle(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
				int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid);
		}

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
