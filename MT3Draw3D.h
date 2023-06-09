#pragma once

#include "MT3Vector3.h"
#include "MT3Matrix4x4.h"
#include "Sphere.h"

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, unsigned int color);
