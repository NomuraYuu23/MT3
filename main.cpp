#include <Novice.h>
#include "MT3Vector3.h"

const char kWindowTitle[] = "LE2A_13_ノムラユウ_3次元への拡張";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	//---変数---//
	//ベクトル1
	Vector3 v1 = { 1.0f, 3.0f, -5.0f };
	//ベクトル2
	Vector3 v2 = { 4.0f, -1.0f, 2.0f };
	//スカラー
	float k = 4.0f;

	//加算
	Vector3 resultAdd = Add(v1, v2);
	//減算
	Vector3 resultSubtract = Subtract(v1, v2);
	//スカラー倍
	Vector3 resultMultiply = Multiply(k, v1);
	//内積
	float resultDot = Dot(v1, v2);
	//長さ
	float resultLength = Length(v1);
	//正規化
	Vector3 resultNormalize = Normalize(v2);

	//描画y
	const int kRowHeight = 20;

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, resultAdd, " : Add");
		VectorScreenPrintf(0, kRowHeight, resultSubtract, " : Subtract");
		VectorScreenPrintf(0, kRowHeight * 2, resultMultiply, " : Multiply");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%.02f  : Dot", resultDot);
		Novice::ScreenPrintf(0, kRowHeight * 4, "%.02f  : Length", resultLength);
		VectorScreenPrintf(0, kRowHeight * 5, resultNormalize, " : Normalize");

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
