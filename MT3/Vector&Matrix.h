#pragma once
#include "Matrix.h"
#include "Vector.h"

struct Sphere {
	Vector3 center;
	float radius;
};

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 MakeTranslationMatrix(const Vector3& translation);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//3次元アファン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
                uint32_t color);
