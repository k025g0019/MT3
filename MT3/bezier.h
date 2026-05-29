#pragma once
#include "Vector&Matrix.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 Bezier(const Vector3& v1, const Vector3& v2, const Vector3& v3, float t);

void DrawBezier(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, uint32_t color);