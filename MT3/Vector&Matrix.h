#pragma once
#include "Matrix.h"
#include "Vector.h"

struct Sphere {
	Vector3 center;
	float radius;
};

struct Segment {
	Vector3 start;
	Vector3 end;
};

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 MakeTranslationMatrix(const Vector3& translation);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//3次元アファン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
                uint32_t color);

Vector3 Project(const Vector3& v1, const Vector3& v2);
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
