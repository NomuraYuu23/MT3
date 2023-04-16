#pragma once

//行列4x4
struct Matrix4x4{

	float m[4][4];

};

//加算
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
//減算
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
//積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
//転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
//単位行列の作成
Matrix4x4 MakeIdentity4x4();
//ベクトル描画
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix);
