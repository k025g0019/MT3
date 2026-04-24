#pragma once
#include "Matrix.h"
#include "Vector.h"

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
Matrix4x4 MakeTranslationMatrix(const Vector3& translation);
Matrix4x4 MakeScaleMatrix(const Vector3& scale);